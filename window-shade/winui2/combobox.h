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

// $Id: combobox.h,v 1.4 2008/02/24 15:24:57 enska Exp $

#ifndef WINUI_COMBOBOX_H
#define WINUI_COMBOBOX_H

#include "basic_control.h"
#include <string>
#include <stdexcept>
#include <boost/scoped_array.hpp>

namespace winui
{
    CONTROL_TEMPLATE_ARGS
    class combobox : PUBLIC_CONTROL_BASE
    {
    public:
        inline
        void add_string(const std::string& str)
        {
            SendMessage(hwnd_, CB_ADDSTRING, 0, reinterpret_cast<LPARAM>(str.c_str()));
        }
        inline
        void del_string(int index)
        {
            range_check(index);
            SendMessage(hwnd_, CB_DELETESTRING, index, 0);
        }
        inline
        void sel_string(const std::string& str)
        {
            SendMessage(hwnd_, CB_SELECTSTRING, -1, reinterpret_cast<LPARAM>(str.c_str()));
        }
        inline
        std::string get_string() const
        {
            if (sel_index() == -1)
                return "";
            return get_string(sel_index());
        }
        inline
        std::string get_string(int index) const
        {
            range_check(index);
            int len = SendMessage(hwnd_, CB_GETLBTEXTLEN, index, 0);
            boost::scoped_array<char> buf (new char[len+1]);
            SendMessage(hwnd_, CB_GETLBTEXT, index, reinterpret_cast<LPARAM>(buf.get()));
            return std::string(buf.get(), len);
        }
        inline
        int str_count() const
        {
            return SendMessage(hwnd_, CB_GETCOUNT, 0, 0);
        }
        inline
        int sel_index() const
        {
            return SendMessage(hwnd_, CB_GETCURSEL, 0, 0);
        }
        inline
        void clear()
        {
            SendMessage(hwnd_, CB_RESETCONTENT, 0, 0);
        }
    private:
        void range_check(int index) const
        {
            if (index < 0 || index > str_count())
                throw std::range_error("combobox: no such index");
        }
    };

} // winui

#endif // WINUI_COMBOBOX_H
