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

// $Id: listbox.h,v 1.4 2008/02/24 15:24:57 enska Exp $

#ifndef WINUI_LISTBOX_H
#define WINUI_LISTBOX_H

#include "basic_control.h"
#include <string>
#include <vector>
#include <stdexcept>
#include <boost/scoped_array.hpp>

namespace winui
{
    CONTROL_TEMPLATE_ARGS
    class listbox : PUBLIC_CONTROL_BASE
    {
    public:
        inline
        void add_string(const std::string& str)
        {
            SendMessage(hwnd_, LB_ADDSTRING, 0, reinterpret_cast<LPARAM>(str.c_str()));
        }
        inline
        void del_string(int index)
        {
            range_check(index);
            SendMessage(hwnd_, LB_DELETESTRING, index, 0);
        }
        inline
        void sel_string(const std::string& str)
        {
            SendMessage(hwnd_, LB_SELECTSTRING, -1, reinterpret_cast<LPARAM>(str.c_str()));
        }
        inline
        std::string get_string(int index)
        {
            range_check(index);
            int len = SendMessage(hwnd_, LB_GETTEXTLEN, index, 0);

            boost::scoped_array<char> buf(new char[len+1]);
            SendMessage(hwnd_, LB_GETTEXT, index, reinterpret_cast<LPARAM>(buf.get()));
            return std::string(buf.get(), len);
        }
        inline
        int str_count() const
        {
            return SendMessage(hwnd_, LB_GETCOUNT, 0, 0);
        }
        inline
        void clear()
        {
            SendMessage(hwnd_, LB_RESETCONTENT, 0, 0);
        }
        inline
        void clear_selection()
        {
            SendMessage(hwnd_, LB_SETCURSEL, -1, 0);
        }
        inline
        int sel_index() const
        {
            // If sent to a multiple-selection list box, 
            // LB_GETCURSEL returns the index of the item 
            // that has the focus rectangle. If no items are selected, it returns zero.
            return SendMessage(hwnd_, LB_GETCURSEL, 0, 0);
        }          
        void get_selection_array(std::vector<int>& indices)
        {
            // If the list box is a single-selection list box the return value is LB_ERR
            LRESULT ret = SendMessage(hwnd_, LB_GETSELCOUNT, 0, 0);
            if (ret == 0) return;
            if (ret == LB_ERR)
            {
                int sel = sel_index();
                if (sel != LB_ERR && sel != 0)
                    indices.push_back(sel);
            }
            else
            {
                indices.resize(ret);
                SendMessage(hwnd_, LB_GETSELITEMS, ret, reinterpret_cast<LPARAM>(&indices[0]));
            }
        }
    private:
        void range_check(int index) const
        {
            if (index < 0 || index > str_count())
                throw std::range_error("listbox: no such index");
        }
    };

} // winui

#endif // WINUI_LISTBOX_H
