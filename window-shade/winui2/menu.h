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

// $Id: menu.h,v 1.10 2008/02/24 14:06:50 enska Exp $

#ifndef WINUI_MENU_H
#define WINUI_MENU_H

#include "common.h"
#include "control.h"
#include "events.h"

#include <cassert>
#include <map>
#include <boost/bind.hpp>
#include <boost/noncopyable.hpp>

namespace winui
{
    // a menu is not a window by any means.
    // i.e. as such the normal window functions do not work on menus.
    class menu : public control, boost::noncopyable
    {
    public:
       ~menu() {}
        menu() : menu_(0) {}

        void connect(HWND parent, int id)
        {
            assert(menu_ == 0);
            assert(id == MENUID);
            menu_ = GetMenu(parent);
            assert(menu_ && "the window has no menu");
        }
        void disconnect() {}

        LRESULT handle_message_from_parent(UINT msg, WPARAM wp, LPARAM lp)
        {
            assert(msg == WM_COMMAND);
            container::iterator it = events_.find(LOWORD(wp));
            if (it != events_.end())
            {
                it->second();
                return TRUE;
            }
            return FALSE;
        }

        // add a menu item handler
        void add(int menuid, const controlevent0& evt)
        {
            // add a new event object for a specific menu item
            // identified by menuid
            events_[menuid] = evt;
        }
        void check_menu_item(int id, bool checked)
        {
            assert(menu_);
            UINT check = checked ? MF_CHECKED : MF_UNCHECKED;
            CheckMenuItem(menu_, id, check);
        }
        bool is_checked(int id) const
        {
            assert(menu_);
            MENUITEMINFO info = {};
            info.cbSize = sizeof(MENUITEMINFO);
            info.fMask  = MIIM_STATE;
            GetMenuItemInfo(menu_, id, FALSE, &info);
            return info.fState == MFS_CHECKED;
        }
    private:
        typedef std::map<int, controlevent0> container;
        HMENU     menu_;
        container events_;
    };
 
} // winui

#endif // WINUI_MENU_H
