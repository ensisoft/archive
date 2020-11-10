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

// $Id: control.h,v 1.6 2008/02/24 15:24:57 enska Exp $

#ifndef WINUI_CONTROL_H
#define WINUI_CONTROL_H

#include <windows.h>

namespace winui
{
    // Control (buttons, etc) interface. This is used internally
    // By the control containers to manage control objects.
    class control
    {
    public:
        virtual ~control() {}
        virtual void connect(HWND parent, int id) = 0;
        virtual void disconnect() = 0;
        
        // the return value should be 0 if message is not handled.
        // otherwise non zero.
        virtual LRESULT handle_message_from_parent(UINT msg, WPARAM wp, LPARAM lp) = 0;
    };

} // winui

#endif // WINUI_CONTROL
