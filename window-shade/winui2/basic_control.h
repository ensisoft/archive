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

// $Id: basic_control.h,v 1.2 2008/02/24 15:24:57 enska Exp $

#ifndef WINUI_BASIC_CONTROL_H
#define WINUI_BASIC_CONTROL_H

#include <cassert>
#include <boost/noncopyable.hpp>
#include "control.h"
#include "window.h"
#include "events.h"

namespace winui
{

#define CONTROL_TEMPLATE_ARGS                                   \
    template<typename WindowEvents    = null_events,            \
             typename ControlEvents   = null_events,            \
             typename ControlCreator  = control_from_resource,  \
             typename ControlPainter  = control_sys_paint>

#define PUBLIC_CONTROL_BASE                     \
    public basic_control<WindowEvents,          \
                         ControlEvents,         \
                         ControlCreator,        \
                         ControlPainter>

    struct control_sys_paint {};
    struct control_from_resource {};

    template<typename WindowEvents    = null_events,
             typename ControlEvents   = null_events,
             typename ControlCreator  = control_from_resource,
             typename ControlPainter  = control_sys_paint>
    class basic_control : private boost::noncopyable, 
        public discriminator<WindowEvents,  3>,
        public discriminator<ControlEvents, 4>,
        public ControlCreator,
        public ControlPainter,
        public window, public control
    {
    public:
        typedef basic_control<WindowEvents, ControlEvents, ControlCreator, ControlPainter> control_type;
       ~basic_control()
        {
            if (window::hwnd_)
                disconnect();
            if (brush_) DeleteObject(brush_);
            if (font_)  DeleteObject(font_);
        }
        basic_control() : old_(NULL), background_(0), foreground_(0), 
                          brush_(0), font_(0)
        {
            background_ = GetSysColor(COLOR_3DFACE);
            foreground_ = GetSysColor(COLOR_WINDOWTEXT);
        }

        void connect(HWND parent, int id)
        {
            assert(parent);
            assert(id);
            assert(!window::hwnd_);
            // despite of the name GetDlgItem works with 
            // any parent-child window relationship.
            // once we get the handle sub class the control
            HWND hwnd = GetDlgItem(parent, id);
            assert(hwnd && "incorrect resource id?");
            
            // hook the control
            SetWindowLongPtr(hwnd, GWL_USERDATA, reinterpret_cast<LONG_PTR>(this));
            old_ = SetWindowLongPtr(hwnd, GWL_WNDPROC, reinterpret_cast<LONG_PTR>(basic_control_proc));
            window::hwnd_ = hwnd;
        }
        void disconnect()
        {
            assert(window::hwnd_);
            // unhook the control
            SetWindowLongPtr(hwnd_, GWL_USERDATA, NULL);
            SetWindowLongPtr(hwnd_, GWL_WNDPROC,  old_);
            hwnd_ = 0;
            old_  = 0;
        }
        LRESULT handle_message_from_parent(UINT msg, WPARAM wp, LPARAM lp)
        {
            evtarg arg = {0, wp, lp, 0, false};
            switch (msg)
            {
                case WM_CTLCOLOREDIT:
                case WM_CTLCOLORBTN:
                case WM_CTLCOLORSTATIC:
                case WM_CTLCOLORLISTBOX:
                    {
                        assert(wp);
                        HDC hdc = reinterpret_cast<HDC>(wp);
                        SetTextColor(hdc, foreground_);
                        SetBkMode(hdc, TRANSPARENT);
                        if (brush_)
                        {
                            BOOL ret = DeleteObject(brush_);
                            assert( ret == TRUE );
                        }
                        brush_ = CreateSolidBrush(background_);
                        return (LRESULT)brush_;
                    }
                    break;
                case WM_COMMAND:
                    ControlEvents::translate(msg, arg);
                    return TRUE;
                case WM_HSCROLL:
                case WM_VSCROLL:
                    ControlEvents::translate(msg, arg);
                    return TRUE;
            }
            return FALSE;
        }
        
        int get_font(LOGFONT& f) const
        {
            return GetObject((HFONT)SendMessage(window::hwnd_, WM_GETFONT, 0, 0), sizeof(LOGFONT), &f);
        }
        int set_font(const LOGFONT& f) 
        {
            HFONT font = CreateFontIndirect(&f);
            if (!font) return 0;
            SendMessage(window::hwnd_, WM_SETFONT, reinterpret_cast<WPARAM>(font), TRUE);
            if (font_)
            {
                BOOL ret = DeleteObject(font_);
                assert( ret == TRUE );
                ret = 0;
            }
            font_ = font;
            return 1;
        }
        void set_foreground_color(COLORREF foreground)
        {
            foreground_ = foreground;
        }
        void set_background_color(COLORREF background)
        {
            background_ = background;
        }
    private:
        static LRESULT CALLBACK basic_control_proc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp)
        {
            LONG_PTR lptr       = GetWindowLongPtr(hwnd, GWL_USERDATA);
            control_type* this_ = reinterpret_cast<control_type*>(lptr);
            assert(this_);
            
            // pass the message to the event handlers
            // the return values are ignored
            evtarg arg = {0, wp, lp, 0, false};
            this_->WindowEvents::translate(msg, arg);

            // handle the message as usual.
            return CallWindowProc(reinterpret_cast<WNDPROC>(this_->old_), hwnd, msg, wp, lp);
        }

        LONG_PTR old_; // old window procedure
        COLORREF background_;
        COLORREF foreground_;
        HBRUSH   brush_;
        HFONT    font_;
    };

} // winui

#endif // WINUI_BASIC_CONTROL_H
