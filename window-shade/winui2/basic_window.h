//
// Copyright (c) 2004 Sami Väisänen
//
// Permission is hereby granted, free of charge, to any person
// obtaining a copy of this software and associated documentation
// files (the "Software"), to deal in the Software without
// restriction, including without limitation the rights to use,
// copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the
// Software is furnished to do so, subject to the following
// conditions:
//
// The above copyright notice and this permission notice shall be
// included in all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
// EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
// OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
// NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
// HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
// WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
// OTHER DEALINGS IN THE SOFTWARE.
//

// $Id: basic_window.h,v 1.2 2008/02/24 15:24:57 enska Exp $

#ifndef WINUI_BASIC_WINDOW_H
#define WINUI_BASIC_WINDOW_H

#include <string>
#include <cassert>
#include <map>
#include <boost/noncopyable.hpp>
#include "window.h"
#include "control.h"
#include "events.h"
#include "blank.h"

namespace winui
{
    template<typename WindowImpl,
             typename WindowEvents  = null_events,
             typename WindowPainter = blank>
    class basic_window : private boost::noncopyable,
        public WindowImpl, 
        public WindowEvents, 
        public WindowPainter, 
        public window
    {
    public:
        typedef basic_window<WindowImpl, WindowEvents, WindowPainter> window_type;
        basic_window() {}
       ~basic_window()
        {
            if (!window::hwnd_)
                return;
            controls_.clear();
            WindowImpl::destroy(window::hwnd_);
        }
        
        // Open this window.
        void open()
        {
            assert(!window::hwnd_);
            window::hwnd_ = WindowImpl::create(basic_window_proc, this);
        }
        
        // Close this window.
        void close()
        {
            if (!window::hwnd_)
                return;
            controls_.clear();
            WindowImpl::destroy(window::hwnd_);
            window::hwnd_ = 0;
        }

        // Connect a control widget to this window. 
        // Control pointer should be a valid non NULL pointer to the widget 
        // and id should be the resource id specified for the control. 
        void connect(control* ctrl, int id)
        {
            assert(ctrl);
            controls_[id] = ctrl;
            if (window::hwnd_)
                ctrl->connect(window::hwnd_, id);
        }
        
        // Disconnect a control widget from this window.
        // The widget is identified by its resource id. 
        void disconnect(int id)
        {
            container::iterator it = controls_.find(id);
            if (it == controls_.end())
                return;
            control* ctrl = it->second;
            if (window::hwnd_)
                ctrl->disconnect();
            controls_.erase(it);
        }

    private:
        void connect()
        {
            // connect all children. For each control give the handle to the parent,
            // i.e. "this" window.
            assert(window::hwnd_);
            for (container::iterator it = controls_.begin(); it != controls_.end(); ++it)
                it->second->connect(window::hwnd_, it->first);
        }
        void disconnect()
        {
            // disconnect all children.
            for (container::iterator it = controls_.begin(); it != controls_.end(); ++it)
                it->second->disconnect();
        }
        control* find(int id)
        {
            container::iterator it = controls_.find(id);
            if (it == controls_.end())
                return NULL;
            return it->second;
        }
        
        static LRESULT CALLBACK basic_window_proc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp)
        {
            window_type* wnd = NULL; 
            LONG_PTR lptr    = GetWindowLongPtr(hwnd, GWL_USERDATA);
            if (lptr) wnd    = reinterpret_cast<window_type*>(lptr);
            if (wnd == NULL && msg != WindowImpl::WM_STARTUP)
                return WindowImpl::default_handle_msg(hwnd, msg, wp, lp);
            
            LRESULT ret = 0;
            switch (msg)
            {
                case WindowImpl::WM_STARTUP:
                    SetWindowLongPtr(hwnd, GWL_USERDATA, static_cast<LONG_PTR>(lp));
                    wnd = reinterpret_cast<window_type*>(lp);
                    assert(!wnd->window::hwnd_ && "this view has already been started. is there a conflict between WM_USER messages?");
                    wnd->window::hwnd_ = hwnd;
                    wnd->connect();
                    ret = TRUE;
                    break;

                // WM_DESTROY is sent to the window when window is being destroyed. At this point the
                // window no longer exists on the screen.
                // application should return 0 if it processes this message.
                case WM_DESTROY:
                    SetWindowLongPtr(hwnd, GWL_USERDATA, 0);
                    wnd->disconnect();
                    wnd->window::hwnd_ = 0;
                    ret = TRUE;
                    break;

                // WM_COMMAND is sent to the parent when a control sends a notification event
                // or a menu item is selected, or an accelerator keystroke is translated.
                // if the message is from a control LPARAM is the handle to the control. 
                // In case of an accelerator or menu item the LPARAM is 0.
                // application should return 0 if it processes this message.
                case WM_COMMAND:
                    {
                        int id = lp ? GetDlgCtrlID(reinterpret_cast<HWND>(lp)) : MENUID;
                        if (control* child = wnd->find(id))
                            ret = child->handle_message_from_parent(msg, wp, lp);
                    }
                    break;

                // A scroll message is sent to the window either by a builtin scrollbar control
                // or then by a slider (trackbar) control
                case WM_HSCROLL:
                case WM_VSCROLL:
                    if (lp)
                    {
                        int id = GetDlgCtrlID(reinterpret_cast<HWND>(lp));
                        if (control* child = wnd->find(id))
                            ret = child->handle_message_from_parent(msg, wp, lp);
                    }
                    break;

                case WM_CTLCOLOREDIT:
                case WM_CTLCOLORBTN:
                case WM_CTLCOLORSTATIC:
                case WM_CTLCOLORLISTBOX:
                    {
                        // lparam is the handle to the control
                        assert(lp);
                        int id = GetDlgCtrlID(reinterpret_cast<HWND>(lp));
                        assert(id);
                        if (control* child = wnd->find(id))
                        {
                            ret = child->handle_message_from_parent(msg, wp, lp);
                            if (ret != FALSE)
                                return ret;
                        }
                    }
                    break;

                // WM_ERASEBKGND is sent when the window background must be erased before painting. 
                // this message is then followed by WM_PAINT covering the invalid window rectangle.
                // application should return _nonzero_ if it prcesses this message.
                case WM_ERASEBKGND:
                    ret = wnd->WindowPainter::erase(hwnd);
                    break;

                // WM_PAINT is sent when the application needs to paint a part of its window area.
                // application should return 0 if it processes this message.
                case WM_PAINT:
                    {
                        PAINTSTRUCT ps = {};
                        BeginPaint(hwnd, &ps);
                        ret = wnd->WindowPainter::paint(hwnd, ps);
                        EndPaint(hwnd, &ps);
                    }
                    break;
                    
                case WM_CLOSE:
                    // this is a ghay feature of windows. when X is clicked on a window
                    // Windows sends WM_CLOSE. If the application doesn't handle WM_CLOSE 
                    // but its passed to whatever default window procedure the default behaviour is to call
                    // DestroyWindow. 
                    // however returning FALSE/TRUE when the window is a dialog class window doesn't result in
                    // the window being destroyed automatically.
                    // So handle the message here and unify the behaviour regardless of the window type.
                    ret = TRUE;
                    break;
            }
            if (wnd)
            {
                // if we can pass the message to the event handlers
                // and if message was not handled above but the handler
                // wants to return a value return that
                evtarg evt = {hwnd, wp, lp, 0, false};
                wnd->WindowEvents::translate(msg, evt);
                if (evt.useRet)
                    return evt.retVal; 
            }
            if (ret == FALSE)
            {
                // only pass a message not handled above to the
                // default handler
                return WindowImpl::default_handle_msg(hwnd, msg, wp, lp);
            }
            return WindowImpl::WM_RETURN_VAL;
        }
    private:
        typedef std::map<int, control*> container;
        container controls_;
    };


} // winui

#endif // WINUI_BASIC_WINDOW_H
