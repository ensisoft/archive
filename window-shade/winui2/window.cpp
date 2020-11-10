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

// $Id: window.cpp,v 1.7 2008/02/24 14:06:50 enska Exp $

#include "window.h"
#include <cassert>
#include <boost/scoped_array.hpp>

namespace  winui
{

window::window() : hwnd_(0)
{
}

window::window(const window& w)
{
    hwnd_ = w.hwnd_;
}

window& window::operator=(const window& w)
{
    hwnd_ = w.hwnd_;
    return *this;
}

window::~window()
{
}

void window::update()
{
    // send WM_PAINT message to the window procedure directly (bypass the msg queue)
    // but only if the update region is not empty.
    assert(hwnd_);
    UpdateWindow(hwnd_); 
}

void window::invalidate()
{
    assert(hwnd_);
    InvalidateRect(hwnd_, NULL, TRUE);
}

void window::hide()
{
    assert(hwnd_);
    ShowWindow(hwnd_, SW_HIDE);
}

void window::show()
{    
    assert(hwnd_);
    ShowWindow(hwnd_, SW_SHOW);
}

void window::enable()
{
    assert(hwnd_);
    EnableWindow(hwnd_, TRUE);
}

void window::disable()
{
    assert(hwnd_);
    EnableWindow(hwnd_, FALSE);
}

void window::position(int x, int y)
{    
    assert(hwnd_);

    SIZE s = size();
    MoveWindow(hwnd_, x, y, s.cx, s.cy, TRUE);
}

void window::position(const POINT& pos)
{
    // convinience function
    position(pos.x, pos.y);
}

void window::size(int cx, int cy)
{
    assert(hwnd_);

    POINT p = position();
    MoveWindow(hwnd_, p.x, p.y, cx, cy, TRUE);
}

void window::size(const SIZE& s)
{
    // convinience function
    size(s.cx, s.cy);
}

void window::focus() const
{
    assert(hwnd_);
    SetFocus(hwnd_);
}

bool window::exists() const
{
    return hwnd_ != NULL;
}

bool window::is_visible() const
{
    assert(hwnd_);
    return IsWindowVisible(hwnd_) != 0;
}

bool window::is_enabled() const
{
    assert(hwnd_);
    return IsWindowEnabled(hwnd_) != 0;
}

bool window::is_focused() const
{
    assert(hwnd_);
    return GetFocus() == hwnd_;
}

POINT window::position() const
{
    assert(hwnd_);

    RECT r = {};
    GetWindowRect(hwnd_, &r);
    HWND parent = GetParent(hwnd_);
    if (!parent)
        parent = HWND_DESKTOP;

    POINT p;
    p.x = r.left;
    p.y = r.top;
    ScreenToClient(parent, &p);
    return p;
}

SIZE window::size() const
{
    assert(hwnd_);

    RECT r = {};
    GetWindowRect(hwnd_, &r);
    SIZE s;
    s.cx = r.right - r.left;
    s.cy = r.bottom - r.top;
    return s;
}

SIZE window::client_size() const
{
    assert(hwnd_);
    RECT r = {};
    GetClientRect(hwnd_, &r);
    SIZE s;
    s.cx = r.right  - r.left;
    s.cy = r.bottom - r.top;
    return s;
}

RECT window::rectangle() const
{
    assert(hwnd_);
    RECT r;
    GetWindowRect(hwnd_, &r);
    return r;
}

RECT window::client_rectangle() const
{
    // return the client rectangle
    assert(hwnd_);
    RECT r;
    GetClientRect(hwnd_, &r);
    return r;
}

std::string window::text() const
{
    assert(hwnd_);
    if (!hwnd_)
        return "";
    
    int len = GetWindowTextLength(hwnd_);
    if (len == 0)
        return "";

    boost::scoped_array<char> buff(new char[len]);

    GetWindowText(hwnd_, buff.get(), len);
    return std::string(buff.get(), len);
}

void window::text(const std::string& str)
{
    assert(hwnd_);
    SetWindowText(hwnd_, str.c_str());
}

HWND window::hwnd()
{
    return hwnd_;
}

} // winui

