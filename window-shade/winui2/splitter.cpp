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

// $Id: splitter.cpp,v 1.7 2008/02/24 15:24:57 enska Exp $

#include "splitter.h"
#include <boost/bind.hpp>
#include <cassert>

namespace winui
{

splitter::splitter() : captured_(false), window_(0)
{
    splitter_.evtlbuttondown = boost::bind(&splitter::mouse_capture, this, _1);
    splitter_.evtlbuttonup   = boost::bind(&splitter::mouse_release, this, _1);
    splitter_.evtmousemove   = boost::bind(&splitter::mouse_move,    this, _1);
}

splitter::~splitter()
{
}

void splitter::split(window& wnd, bool vertical)
{
    if (splitter_.exists())
        return;

    SIZE client = wnd.client_size();

    splitter_.parent   = wnd.hwnd();
    dragbar_.parent    = wnd.hwnd();
    splitter_.wndstyle = WS_CHILD;
    dragbar_.wndstyle  = WS_CHILD;

    HCURSOR cursor = 0;
    int x, y;

    if (vertical)
    {
        x = 2;
        y = client.cy;
        cursor = LoadCursor(NULL, IDC_SIZEWE);
    }
    else
    {
        x = client.cx;
        y = 2;
        cursor = LoadCursor(NULL, IDC_SIZENS);
    }

    dragbar_.wndcls.hCursor  = cursor;
    splitter_.wndcls.hCursor = cursor;

    splitter_.open();
    dragbar_.open();
    dragbar_.hide();

    splitter_.size(x, y);
    dragbar_.size(x, y);

    window_   = &wnd;
    vertical_ = vertical;
}

void splitter::moveto(int pos)
{
    if (!splitter_.exists())
        return;
    if (vertical_)
    {
        splitter_.position(pos, 0);
        dragbar_.position(pos, 0);
    }
    else
    {
        splitter_.position(0, pos);
        dragbar_.position(pos, 0);
    }
    rehash();
}

void splitter::adjust()
{
    if (!splitter_.exists())
        return;
    SIZE client = window_->client_size();
    if (vertical_)
    {
        splitter_.size(2, client.cy);
        dragbar_.size(2, client.cy);
    }
    else
    {
        splitter_.size(client.cx, 2);
        dragbar_.size(client.cx, 2);
    }
    rehash();
}

void splitter::mouse_capture(const evtarg&)
{
    // need to use SetCapture
    // otherwise we dont get mousemove events when the 
    // mouse pointer leaves the splitter window area
    dragbar_.show();

    splitter_pos_ = splitter_.position();
    GetCursorPos(&mouse_pos_);
    SetCapture(splitter_.hwnd());
    captured_ = true;

}

void splitter::mouse_release(const evtarg&)
{
    dragbar_.hide();

    splitter_.position(dragbar_.position());

    ReleaseCapture();
    captured_ = false;
    rehash();
}

void splitter::mouse_move(const evtarg&)
{
    if (!captured_)
        return;

    POINT pt;
    GetCursorPos(&pt);

    POINT diff;
    diff.x = pt.x - mouse_pos_.x;
    diff.y = pt.y - mouse_pos_.y;
 
    POINT tmp = splitter_pos_;
    tmp.x += diff.x;
    tmp.y += diff.y;

    if (vertical_) 
        dragbar_.position(tmp.x, 0);
    else 
        dragbar_.position(0, tmp.y);
}

void splitter::rehash() const
{
    if (!evtresize)
        return;
    
    // calculate new sizes

    splitter_evt_arg arg;

    SIZE client = window_->client_size();
    SIZE splitt = splitter_.size();
    POINT pt = splitter_.position();
    if (vertical_)
    {
        SIZE left, right;
        POINT pos_left, pos_right;

        pos_left.x = 0;
        pos_left.y = 0;

        left.cy = client.cy;
        left.cx = pt.x;
        right.cy = client.cy;
        right.cx = client.cx - splitt.cx - left.cx;

        pos_right.x = left.cx + splitt.cx;
        pos_right.y = 0;
                    
        arg.size_first  = left;
        arg.size_second = right;
        arg.pos_first  = pos_left;
        arg.pos_second = pos_right;
    }
    else
    {
        SIZE top, bottom;
        POINT pos_top, pos_bottom;
        
        pos_top.x = 0;
        pos_top.y = 0;
        
        top.cy = pt.y;
        top.cx = client.cx;
        bottom.cy = client.cy - splitt.cy - top.cy;
        bottom.cx = client.cx;

        pos_bottom.x = 0;
        pos_bottom.y = top.cy + splitt.cy;

        arg.size_first  = top;
        arg.size_second = bottom;
        arg.pos_first   = pos_top;
        arg.pos_second  = pos_bottom;
    }
    evtresize(arg);
}

} // winui

