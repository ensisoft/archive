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

// $Id: mdichild.h,v 1.7 2008/02/24 14:06:50 enska Exp $

#ifndef WINUI_MDICHILD_H
#define WINUI_MDHCHILD_H

#include "common.h"
#include "exception.h"
#include <cassert>
#include <string>

namespace winui
{
    // MDI window implementation policy,
    // which creates a MDI child window inside the MDI client window.
    class mdichild 
    {
    public:
        WNDCLASSEX wndcls;      // window class
        HWND client;            // a handle to the MDI client window. MUST NOT BE NULL. 
    protected:
       ~mdichild() {}
        mdichild() : client(0)
        {
            memset(&wndcls, 0, sizeof(wndcls));
                
            wndcls.cbSize        = sizeof(WNDCLASSEX);
            wndcls.hInstance     = GetModuleHandle(NULL);
            wndcls.hbrBackground = (HBRUSH)(COLOR_WINDOW +1);
            wndcls.hCursor       = LoadCursor(NULL, IDC_ARROW);
            wndcls.style         = CS_VREDRAW | CS_HREDRAW | CS_DBLCLKS;
            classname_ = create_class_name(typeid(mdichild).name());
        }

        enum { WM_RETURN_VAL = 0 };
        enum { WM_STARTUP = WM_USER + 1};

        HWND create(WNDPROC proc, void* arg)
        {
            // need MDI client handle
            assert(client);

            wndcls.lpfnWndProc   = proc;
            wndcls.lpszClassName = classname_.c_str();
            if (!RegisterClassEx(&wndcls))
            {
                int err = GetLastError();
                throw uiexception("failed to register window class (RegisterClassEx)", err);
            }

            MDICREATESTRUCT mdi = {};
            mdi.szClass = wndcls.lpszClassName;
            mdi.hOwner  = wndcls.hInstance;
            mdi.x = mdi.cx = CW_USEDEFAULT;
            mdi.y = mdi.cy = CW_USEDEFAULT;

            // when creating a new child window 
            // we actually send a message to the CLIENT window to do the creation.
            LPARAM  ret = SendMessage(client, WM_MDICREATE, 0, reinterpret_cast<LPARAM>(&mdi));
            if (!ret)
                throw uiexception("MDI child window creation failed", GetLastError());
            
            HWND child = reinterpret_cast<HWND>(ret);
            // child window created OK.
            // indicate a succesful startup
            SendMessage(child, WM_STARTUP, 0, reinterpret_cast<LPARAM>(arg));
            ShowWindow(child, SW_SHOWNORMAL);
            SetFocus(child);
            return child;
        }
        void destroy(HWND hwnd)
        {
            SendMessage(client, WM_MDIDESTROY, reinterpret_cast<WPARAM>(hwnd), 0);
            client = 0;
        }
        static 
        LRESULT default_handle_msg(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp)
        {
            return DefMDIChildProc(hwnd, msg, wp, lp);
        }
    private:
        std::string classname_;

    };

} // winui

#endif // WINUI_MDICHILD_H
