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


// $Id: treeview.h,v 1.3 2008/02/24 15:24:57 enska Exp $

#ifndef WINUI_TREEVIEW_H
#define WINUI_TREEVIEW_H

#include "basic_control.h"
#include <commctrl.h>
#include <string>

namespace winui
{
    CONTROL_TEMPLATE_ARGS
    class treeview : PUBLIC_CONTROL_BASE
    {
    public:
        inline
        HTREEITEM add_node(const std::string & text)
        {
            TVINSERTSTRUCT ins = {};
            ins.hInsertAfter   = TVI_ROOT;
            ins.item.mask      = TVIF_TEXT;
            ins.item.pszText   = const_cast<char*>(text.c_str());

            return TreeView_InsertItem(hwnd_, &ins);
        }
        inline
        HTREEITEM add_node(const std::string & text, HTREEITEM parent)
        {
            TVINSERTSTRUCT ins = {};
            ins.hParent        = parent;
            ins.hInsertAfter   = TVI_LAST;
            ins.item.mask      = TVIF_TEXT;
            ins.item.pszText   = const_cast<char*>(text.c_str());
            
            return TreeView_InsertItem(hwnd_, &ins);
        }
    private:

    };

} // winui

#endif // WINUI_TREEEVIEW_H
