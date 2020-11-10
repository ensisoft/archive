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

// $Id: button.h,v 1.7 2008/02/24 15:24:57 enska Exp $

#ifndef WINUI_BUTTON_H
#define WINUI_BUTTON_H

#include "basic_control.h"

namespace winui
{
    CONTROL_TEMPLATE_ARGS
    class button : PUBLIC_CONTROL_BASE
    {   
    public:
        inline
        void check(bool val)
        {
            SendMessage(hwnd_,  BM_SETCHECK, val, 0);
        }
        inline
        bool is_checked() const
        {
            return SendMessage(hwnd_, BM_GETCHECK, 0, 0) == BST_CHECKED;
        }
        inline
        void set_image(HANDLE img, int type)
        {
            SendMessage(hwnd_, BM_SETIMAGE, static_cast<WPARAM>(type), reinterpret_cast<LPARAM>(img));
        }
    private:
    };

} // winui

#endif // WINUI_BUTTON_H
