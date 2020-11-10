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


// $Id: blank.h,v 1.5 2008/02/24 15:24:57 enska Exp $

#ifndef WINUI_BLANK_H
#define WINUI_BLANK_H

#include <windows.h>

namespace winui
{
    // this is a dummy (blank/do nothing) paint policy.
    // Can be used in cases that require a paint policy interface conforming type
    // but no actual output is required. (For instance container windows).
    class blank
    {
    protected:
       ~blank() {}
        LRESULT paint(HWND, const PAINTSTRUCT&) 
        {
            return FALSE;
        }
        LRESULT erase(HWND)
        {
            return FALSE;
        }
    };

} // winui

#endif // WINUI_BLANK_H
