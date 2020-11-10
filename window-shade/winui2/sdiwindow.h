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

// $Id: sdiwindow.h,v 1.10 2008/02/24 14:06:50 enska Exp $

#ifndef WINUI_SDIWINDOW_H
#define WINUI_SDIWINDOW_H

#include <windows.h>
#include "exception.h"
#include "common.h"

namespace winui
{

    // SDI window implementation policy, 
    // which creates simple top level overlapped window.
    class sdiwindow
    {
    public:
        HWND parent;            // Window parent (if any).
        WNDCLASSEX wndcls;      // The wndclass that has the attributes when registering window class.
        int wndstyle;           // Window style.
        int wndstyleex;         // Extended window style
    protected:
       ~sdiwindow() {}
        sdiwindow() : parent(0), 
                      wndstyle(WS_OVERLAPPEDWINDOW),
                      wndstyleex(0)
        {
            memset(&wndcls, 0, sizeof(WNDCLASSEX));
            // set some reasonable default values here.
            // the client code has the power to change these.
            wndcls.cbSize        = sizeof(WNDCLASSEX);
            wndcls.hInstance     = GetModuleHandle(NULL);
            wndcls.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
            wndcls.hCursor       = LoadCursor(NULL, IDC_ARROW);
            wndcls.style         = CS_VREDRAW | CS_HREDRAW | CS_DBLCLKS;
            classname_ = create_class_name(typeid(sdiwindow).name());
        }

        enum { WM_STARTUP    = WM_USER + 1,
               WM_RETURN_VAL = 0 };
            
        HWND create(WNDPROC proc, void* arg)
        {
            wndcls.lpfnWndProc   = proc;
            wndcls.lpszClassName = classname_.c_str();
            if (!RegisterClassEx(&wndcls))
            {
                int err = GetLastError();
                throw uiexception("failed to register window class (RegisterClassEx)", err);
            }
            HWND hwnd = CreateWindowEx(wndstyleex,
                wndcls.lpszClassName,
                wndcls.lpszClassName,
                wndstyle,
                CW_USEDEFAULT,
                CW_USEDEFAULT,
                CW_USEDEFAULT,
                CW_USEDEFAULT,
                parent,
                NULL, 
                wndcls.hInstance,
                NULL);

            if (!hwnd)
                throw uiexception("failed to create window (CreateWindow)", GetLastError());

            // create a WM_STARTUP message to 
            // indicate succesful startup
            SendMessage(hwnd, WM_STARTUP, 0, reinterpret_cast<LPARAM>(arg));
            UpdateWindow(hwnd);
            ShowWindow(hwnd, SW_SHOW);
            SetFocus(hwnd);
            return hwnd;
        }
        void destroy(HWND hwnd)
        {
            BOOL ret = FALSE;
            ret = DestroyWindow(hwnd);
            assert(ret == TRUE);
            ret = 0;
        }
        static
        LRESULT default_handle_msg(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp)
        {
            return DefWindowProc(hwnd, msg, wp, lp);
        }
    private:
        std::string classname_;
    };

} // winui

#endif // WINUI_SDIWINDOW_H
