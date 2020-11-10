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


// $Id: msgwindow.h,v 1.2 2008/02/24 15:24:57 enska Exp $

#ifndef WINUI_MSGWINDOW_H
#define WINUI_MSGWINDOW_H

#include <windows.h>
#include <cassert>
#include <string>
#include "exception.h"

namespace winui
{
    // Message only window implementation.
    class msgwindow
    {
    public:
        // You can use this to give a specific 
        // name to the window class. If this is NULL when window
        // is crated a name will be constructed.
        void set_class_name(const char* myname)
        {
            classname = myname;
        }
    protected:
       ~msgwindow() {}
        msgwindow() : classname(NULL) {}

        enum { WM_RETURN_VAL = 0 };
        enum { WM_STARTUP = WM_USER + 1000 };

        inline
        void destroy(HWND hwnd)
        {
            BOOL ret = DestroyWindow(hwnd);
            assert(ret == TRUE);
            ret = 0;
        }
        HWND create(WNDPROC proc, void* arg)
        {
            std::string str = create_class_name(typeid(msgwindow).name());
            const char* ptr = classname ? classname : str.c_str();
                
            WNDCLASSEX wndcls;
            memset(&wndcls, 0, sizeof(WNDCLASSEX));
            wndcls.cbSize        = sizeof(WNDCLASSEX);
            wndcls.hInstance     = GetModuleHandle(NULL);
            wndcls.lpszClassName = ptr;
            wndcls.lpfnWndProc   = proc;
            if (!RegisterClassEx(&wndcls))
            {
                int err = GetLastError();
                throw uiexception("failed to register class (RegisterClassEx)", err);
            }
            HWND hwnd = CreateWindow(
                wndcls.lpszClassName,
                NULL,
                0, 0, 0, 0, 0, 
                HWND_MESSAGE,
                NULL,
                wndcls.hInstance,
                NULL);
            if (!hwnd)
                throw uiexception("failed to create window (CreateWindow)", GetLastError());

            SendMessage(hwnd, WM_STARTUP,0, reinterpret_cast<LPARAM>(arg));
            return hwnd;
        }
        static inline
        LRESULT default_handle_msg(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp)
        {
            return DefWindowProc(hwnd, msg, wp, lp);
        }
    private:
        const char* classname;
    };

} // winui

#endif // WINUI_MSGWINDOW_H


