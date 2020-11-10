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

// $Id: events.h,v 1.13 2008/02/24 14:06:50 enska Exp $

#ifndef WINUI_EVENTS_H
#define WINUI_EVENTS_H

#include <windows.h>
#include <boost/function.hpp>
#include <map>

namespace winui
{
    // Event argument bundle.
    // The meaning of WPARAM and LPARAM depends on the message type.
    struct evtarg {
        HWND    hwnd;
        WPARAM  wParam; 
        LPARAM  lParam; 
        LRESULT retVal;
        bool    useRet; // use retVal and suppress default window proc
    };  
    typedef boost::function0<void> controlevent0;
    typedef boost::function1<void, evtarg&> controlevent1;

    // Empty event policy. Does nothing.
    class null_events
    {
    protected:
       ~null_events() {}
        void translate(UINT, const evtarg&) {}
    };

    

    inline
    void invoke(const controlevent1& evt, evtarg& a)
    {
        if (evt) evt(a);
    }
    inline
    void invoke(const controlevent0& evt)
    {
        if (evt) evt();
    }
    

    // Mouse event policy. 
    class mouse_events
    {
    public:
        controlevent1 evtlbuttondown;   // Left mouse button was pressed.
        controlevent1 evtlbuttonup;     // Left mouse button was released.
        controlevent1 evtldblclick;     // Left mouse button was double clicked. (CS_DBLCLKS needs to be set)
        controlevent1 evtmousemove;     // Mouse was moved.
        controlevent1 evtmouseleave;    // Mouse left a window area.
        controlevent1 evtmousewheel;    // Mouse was scrolled. Note: there should not be internal message forwarding
                                        // of this message since windows propagates it up the window hierarchy untill
                                        // it finds a window that processes the message.
    protected:
       ~mouse_events() {}
        void translate(UINT m, evtarg& arg)
        {
            // window needs to have CS_DBLCLKS style for this
            switch (m)
            {
                case WM_LBUTTONDOWN:   invoke(evtlbuttondown, arg); break;
                case WM_LBUTTONUP:     invoke(evtlbuttonup,   arg); break;
                case WM_LBUTTONDBLCLK: invoke(evtldblclick,   arg); break;
                case WM_MOUSEMOVE:     invoke(evtmousemove,   arg); break;
                case WM_MOUSELEAVE:    invoke(evtmouseleave,  arg); break;
                case 0x020A:           invoke(evtmousewheel,  arg); break;  // WM_MOUSEWHEEL
            }
        }
    private:
    };
 
    // Keyboard event policy.
    class keyboard_events
    {
    public:
        controlevent1 evtchar;          // Key press event was translated into a input character.
        controlevent1 evtkeydown;       // Key was pressed down
        controlevent1 evtkeyup;         // Key that was supressed was released.
    protected:
       ~keyboard_events() {}
        void translate(UINT m, evtarg& arg)
        {
            switch (m)
            {
                case WM_CHAR:    invoke(evtchar,    arg); break;
                case WM_KEYDOWN: invoke(evtkeydown, arg); break;
                case WM_KEYUP:   invoke(evtkeyup,   arg); break;
            }
        }
    private:
    };

    // General window event policy. 
    class window_events
    {
        enum { WM_STARTUP = WM_USER+1}; // todo: put this in some good place
    public:
        controlevent0 evtkillfocus;  // Window lost focus.
        controlevent0 evtsetfocus;   // Window gained focus.
        controlevent0 evtdestroy;    // Window is being destroyed.
        controlevent0 evtclose;      // Window is about to close.
        controlevent0 evtcreate;     // Window was created.
        controlevent0 evtsize;       // Window was resized.
        controlevent1 evtvscroll;    // Window was scrolled (vertically).
        controlevent1 evthscroll;    // Window was scrolled (horizontally).
    protected:
       ~window_events() {}
        void translate(UINT m, evtarg& arg)
        {
            switch (m)
            {
                case WM_KILLFOCUS:  invoke(evtkillfocus);      break;
                case WM_SETFOCUS:   invoke(evtsetfocus);       break;
                case WM_DESTROY:    invoke(evtdestroy);        break;
                case WM_CLOSE:      invoke(evtclose);          break;
                case WM_STARTUP:    invoke(evtcreate);         break;
                case WM_INITDIALOG: invoke(evtcreate);         break;
                case WM_SIZE:       invoke(evtsize);           break;
                case WM_VSCROLL:    invoke(evtvscroll, arg);   break;
                case WM_HSCROLL:    invoke(evthscroll, arg);   break;

            }
        }
    };

    template<typename T, int>
    struct discriminator : public T {};

    //
    // Event policy aggregates mouse, keyboard and gneral events into one
    // event translator construct.
    // A custom event policy needs to implement:
    //    void translate(UINT, WPARAM, LPARAM)
    template<typename Keyboard,
             typename Mouse,
             typename General>
    class event_binder : 
       public discriminator<Keyboard, 0>,
       public discriminator<Mouse,    1>,
       public discriminator<General,  2>
    {
    protected:
       ~event_binder() {}
        void translate(UINT msg, evtarg& args)
        {
            Keyboard::translate(msg, args);
            Mouse::translate(msg, args);
            General::translate(msg, args);
        }
    };

    //
    // Dynevent allows the translation (binding)
    // of arbitrary WM_? messages to functions.
    // Dynevent can be used to gain a fine grained control
    // over messages and handler functions.
    // In order to use the dynevent a view must be configured to use it
    //    typedef view<dialogimpl<modal>, dynevent> > myview;
    // 
    // Each message should have its own handler function bound explicitly.
    //    myview.bind(WM_FOCUS, my_focus_function);
    //
    class dynevent
    {
    public:
        void bind(UINT msg, const controlevent1& evt)
        {
            // add an event 
            events_[msg] = evt;
        }
        void bind(UINT msg, controlevent1& evt)
        {
            events_[msg] = evt;
        }
    protected:
       ~dynevent() {}
        void translate(UINT m, evtarg& eArg)
        {
            // lookup the event objec from the map
            std::map<UINT, controlevent1>::iterator it = events_.find(m);
            if (it == events_.end() || it->second.empty())
                return;
            it->second(eArg);
        }
    private:
        std::map<UINT, controlevent1> events_;
    };

    // This event type provides no events.
    typedef event_binder<null_events, null_events, null_events> noevents;

    // This event type provides all events from all different categories
    typedef event_binder<keyboard_events, mouse_events, window_events> allevents;

    // This type provides keyboard events only.
    typedef event_binder<keyboard_events, null_events, null_events> kbevents;

    // This type provides mouse events only.
    typedef event_binder<null_events, mouse_events, null_events> mouseevents;

    // This type provides general window events only.
    typedef event_binder<null_events, null_events, window_events> wndevents;

    // Button events works with all kinds of buttons.
    // Pushbuttons, radio buttons and checkbox buttons.
    class button_events 
    {
    public:
        controlevent0 evtclick;
    protected:
       ~button_events() {}
        void translate(UINT msg, evtarg& arg)
        {
            if (msg != WM_COMMAND) return;
            if (HIWORD(arg.wParam) == BN_CLICKED && evtclick)
                evtclick();
        }
    };

    // Slider, i.e. trackbar events
    class slider_events 
    {
    public:
        controlevent0 evtchange;
    protected:
       ~slider_events() {}
        void translate(UINT msg, evtarg& arg)
        {
            // a slider control is fairly talkative.
            // there are TB_BOTTOM, TB_ENDTRACK, TB_LINEDOWN... messages
            // but for now we simply care about a change in the position.
            if ((msg == WM_HSCROLL || msg == WM_VSCROLL) && evtchange)
                evtchange();
        }
    };
    
    // Box events work with "box" controls, i.e. listnbox and combobox.
    class box_events
    {
    public:
        controlevent0 evtselect;
        controlevent0 evtdblclick;
    protected:
       ~box_events() {}
        void translate(UINT msg, evtarg& arg)
        {
            if (msg != WM_COMMAND) return;
            int code = HIWORD(arg.wParam);
            if ((code == CBN_SELCHANGE || code == LBN_SELCHANGE) && evtselect)
                evtselect();
            if ((code == CBN_DBLCLK || code == LBN_DBLCLK) && evtdblclick)
                evtdblclick();
        }
    };

} // winui
 
#endif // WINUI_EVENTS_H
