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

// $Id: menu.cpp,v 1.4 2008/02/24 14:06:50 enska Exp $

#include "resource.h"
#include <ui.h>
#include <boost/bind.hpp>
#include <cassert>

using namespace winui;

void destroy_window(const winui::evtarg&)
{
    PostQuitMessage(0);
}

void exit_application()
{
    PostQuitMessage(0);
}

void about()
{
    MessageBox(NULL, "Simple demonstration of a window with a menu", "About", MB_OK);
}

void file_new()
{
    MessageBox(NULL, "File new", "Menu event", MB_OK);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPTSTR, int)
{
    menu m;
    m.add(ID_FILE_EXIT, exit_application);
    m.add(ID_FILE_NEW_, file_new);
    m.add(ID_ABOUT,     about);

    basic_window<sdiwindow, dynevent> window;
    window.bind(WM_CLOSE, destroy_window);
    window.wndcls.lpszMenuName = MAKEINTRESOURCE(IDR_MENU1);
    window.connect(&m, MENUID);
    window.open();
    window.size(500, 500);

    HACCEL accel = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDR_ACCELERATOR1));
    assert(accel);

    MSG msg;
    while (GetMessage(&msg, 0, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, accel, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }
    return 0;
}

