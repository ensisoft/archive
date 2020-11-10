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

// $Id: sdihello.cpp,v 1.8 2008/02/24 15:24:57 enska Exp $

#include <string>
#include <sstream>
#include <ui.h>
#include <boost/bind.hpp>
#include <boost/noncopyable.hpp>

using namespace winui;

class hello_world
{
protected:
   ~hello_world() {}
    LRESULT paint(HWND hwnd, const PAINTSTRUCT& ps)
    {
        const char* str = "Hello World!";
        RECT rect;
        GetClientRect(hwnd, &rect);
        FillRect(ps.hdc, &rect,GetSysColorBrush(COLOR_WINDOW));
        SetBkMode(ps.hdc, TRANSPARENT);
        SetTextColor(ps.hdc, RGB(0xFF, 0, 0));
        DrawText(ps.hdc, str, strlen(str),&rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
        return TRUE;
    }
    LRESULT erase(HWND)
    {
        return FALSE;
    }
};

void quit()
{
    PostQuitMessage(0);
}


int WINAPI WinMain(HINSTANCE, HINSTANCE, LPTSTR, int)
{
    basic_window<sdiwindow, window_events, hello_world> window;
    window.open();
    window.size(500, 500);
    window.text("Hello World");
    window.evtclose = quit;

    MSG m;
    while (GetMessage(&m, 0, 0, 0))
    {
        TranslateMessage(&m);
        DispatchMessage(&m);
    }
    return 0;
}
