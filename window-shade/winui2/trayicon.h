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

// $Id:

#ifndef WINUI_TRAYICON_H
#define WINUI_TRAYICON_H

#include <shellapi.h>
#include <string>
#include <cassert>
#include <algorithm>
#include <boost/noncopyable.hpp>

#undef min
#undef MIN

namespace winui
{
    // Minimalistic trayicon class.
    class trayicon : boost::noncopyable
    {
    public:
       ~trayicon()
        {
            rem();
        }
        trayicon() : handle_(0) {}
        void add(const std::string& tooltip,
            HWND hwnd,      // the handle where the message is to be sent
            UINT msg,       // the message id the trayicon will generate when clicked
            int  iconId)    // the icon resource id
        {
            assert(hwnd && "invalid handle");
            if (handle_)
                rem();
            NOTIFYICONDATA icondata = {0};
            icondata.cbSize = sizeof(icondata);
            icondata.hWnd   = hwnd;
            icondata.uFlags = NIF_ICON | NIF_TIP | NIF_MESSAGE;
            icondata.uCallbackMessage = msg;
            icondata.hIcon  = LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(iconId));
            // msdn: szTip
            // Pointer to a null-terminated string with the text for a standard ToolTip. 
            // It can have a maximum of 64 characters including the terminating NULL.
            // For Version 5.0 and later, szTip can have a maximum of 128 characters, including the terminating NULL.
            lstrcpyn(icondata.szTip, tooltip.c_str(), std::min<int>(tooltip.length() + 1, 64));
            Shell_NotifyIcon(NIM_ADD, &icondata);
            handle_ = hwnd;
        }
        void rem()
        {
            if (!handle_) return;
            NOTIFYICONDATA icondata = {0};
            icondata.cbSize = sizeof(icondata);
            icondata.hWnd   = handle_;
            Shell_NotifyIcon(NIM_DELETE, &icondata);
            handle_ = 0;
        }
        bool visible() const
        {
            return handle_ != NULL;
        }
    private:
        HWND handle_;
    };

} // winui

#endif // WINUI_TRAYICON_H

