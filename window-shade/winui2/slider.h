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

// $Id: slider.h,v 1.4 2008/02/24 14:06:50 enska Exp $

#ifndef WINUI_SLIDER_H
#define WINUI_SLIDER_H

#include "basic_control.h"
#include <commctrl.h>

namespace winui
{
    CONTROL_TEMPLATE_ARGS
    class slider : PUBLIC_CONTROL_BASE
    {
    public:
        inline
        void set_range(int min, int max)
        {
            SendMessage(hwnd_, TBM_SETRANGE, TRUE, static_cast<LPARAM>(MAKELONG(min, max)));
        }
        inline
        int getpos() const
        {
            // get the current slider position
            LRESULT ret = SendMessage(hwnd_, TBM_GETPOS, 0, 0);
            return static_cast<int>(ret);
        }
        inline
        void setpos(int p)
        {
            // set the slider position
            SendMessage(hwnd_, TBM_SETPOS, TRUE, p);
        }
    private:
    };

} // winui

#endif // WINUI_SLIDER_H
