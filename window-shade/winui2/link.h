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

// $Id: link.h,v 1.6 2008/02/24 15:24:57 enska Exp $

#ifndef WINUI_LINK_H
#define WINUI_LINK_H

#include "basic_control.h"
#include "events.h"
#include <windows.h>
#include <boost/bind.hpp>

namespace winui
{
    // a hyperlink control
    // in order for this to function correctly
    // the "notify" attribute must be set to true in the designer
    class hyperlink : public control
    {
    public:
        COLORREF color_active;
        COLORREF color_visited;
        controlevent0 evtclick;

       ~hyperlink() {}
        hyperlink() : color_active(0), color_visited(0), hand_(0), old_(0)
        {
            color_active  = RGB(0, 0, 0xff);
            color_visited = RGB(0x66, 0, 0);
            hand_         = LoadCursor(NULL, IDC_HAND);
            label_.evtlbuttonup  = boost::bind(&hyperlink::mouse_click, this, _1);
            label_.evtmousemove  = boost::bind(&hyperlink::mouse_move,  this, _1);
            label_.evtmouseleave = boost::bind(&hyperlink::mouse_leave, this, _1);
        }
        void connect(HWND parent, int id)
        {
            label_.connect(parent, id);
            LOGFONT f = {};
            label_.get_font(f);
            
            f.lfUnderline = TRUE;
            label_.set_font(f);
            label_.set_foreground_color(color_active);
        }
        void disconnect()
        {
            label_.disconnect();
        }
        LRESULT handle_message_from_parent(UINT msg, WPARAM wp, LPARAM lp)
        {
            return label_.handle_message_from_parent(msg, wp, lp);
        }
    private:
        void mouse_click(const evtarg&)
        {
            label_.set_foreground_color(color_visited);
            label_.update();
            if (evtclick)
                evtclick();
        }
        void mouse_move(const evtarg&)
        {
            if (!old_)
                old_ = SetCursor(hand_);
            else
                SetCursor(hand_);
        }
        void mouse_leave(const evtarg&)
        {
            SetCursor(old_);
            old_ = 0;
        }

        HCURSOR hand_;
        HCURSOR old_;
        basic_control<mouse_events> label_;
    };

} // winui

#endif // WINUI_LINK_H
