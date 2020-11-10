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

// $Id: editbox.h,v 1.6 2008/02/24 15:24:57 enska Exp $

#ifndef WINUI_EDITBOX_H
#define WINUI_EDITBOX_H

#include "basic_control.h"
#include <string>
#include <vector>
#include <boost/scoped_array.hpp>

namespace winui
{
    CONTROL_TEMPLATE_ARGS
    class editbox : PUBLIC_CONTROL_BASE
    {
    public: 
        inline
        void set_str(const std::string& str)
        {   
            SendMessage(hwnd_, WM_SETTEXT, 0, reinterpret_cast<LPARAM>(str.c_str()));
        }
        inline
        std::string get_str() const
        {
            string ret
            int len = SendMessage(hwnd_, WM_GETTEXTLENGTH, 0, 0);
            boost::scoped_array<char> buf(new char[len+1]);
            SendMessage(hwnd_, WM_GETTEXT, len+1, reinterpret_cast<LPARAM>(buf.get()));
            return std::string(buf.get(), len);
        }
        inline
        bool empty() const
        {
            return SendMessage(hwnd_, WM_GETTEXTLENGTH, 0, 0) == 0;
        }
    private:
    };
    
} // winui

#endif // WINUI_EDITBOX_H
