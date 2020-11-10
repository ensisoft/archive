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

// $Id: open_gl_hello.cpp,v 1.5 2008/02/24 14:06:50 enska Exp $

#include <ui.h>
#include <boost/bind.hpp>
#include "pyramid.h"

using namespace winui;

void quit(const winui::evtarg&)
{
    PostQuitMessage(0);
}


int WINAPI WinMain(HINSTANCE, HINSTANCE, LPTSTR, int)
{
    // create a view
    basic_window<sdiwindow, dynevent, pyramid> window;

    window.bind(WM_CLOSE, quit);
    window.open();
    window.size(600, 500);
    window.text("OpenGL window");
    
    window.gl_init(window.hwnd());
    window.gl_resize(600, 500);

    GLfloat rotation = 0;

    while (true)
    {
        MSG m;
        if (PeekMessage(&m, NULL, 0, 0, PM_REMOVE))
        {
            if (m.message == WM_QUIT)
                break;

            TranslateMessage(&m);
            DispatchMessage(&m);
        }
        window.gl_render(0, rotation, 0);
        rotation += 0.1f;
    }
    return 0;
}
