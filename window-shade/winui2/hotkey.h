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

// $Id: hotkey.h,v 1.6 2008/02/24 15:24:57 enska Exp $

#ifndef WINUI_HOTKEY_H
#define WINUI_HOTKEY_H

#include "basic_control.h"
#include <cassert>

namespace winui
{
    CONTROL_TEMPLATE_ARGS
    class hotkey : PUBLIC_CONTROL_BASE
    {
    public:
        /// return the modifier in a format compatible with 
        /// RegisterHotKey
        /// in winuser.h (for RegisterHotKey) SHIFT is 0x04 and ALT is 0x01
        //  whereas in commctrl.h SHIFT is 0x01 and ALT is 0x04
        inline
        int modifier() const
        {
            LRESULT lret = SendMessage(hwnd_, HKM_GETHOTKEY, 0, 0);
            int val = lret >> 8;
            int ret = 0;
            if (val & HOTKEYF_SHIFT)  ret |= MOD_SHIFT;
            if (val & HOTKEYF_CONTROL)ret |= MOD_CONTROL;
            if (val & HOTKEYF_ALT)    ret |= MOD_ALT;
            return ret;
        }
        inline
        int virtkey() const
        {
            LRESULT lret = SendMessage(hwnd_, HKM_GETHOTKEY, 0, 0);
            return lret & 0xFF;
        }
        inline
        void set(int modifier, int virtkey)
        {
            int mod = 0;
            if (modifier & MOD_SHIFT)   mod |= HOTKEYF_SHIFT;
            if (modifier & MOD_CONTROL) mod |= HOTKEYF_CONTROL;
            if (modifier & MOD_ALT)     mod |= HOTKEYF_ALT;
            int val = mod << 8 | (0xFF & virtkey);
            SendMessage(hwnd_, HKM_SETHOTKEY, val, 0);
        }
        inline
        void set(int hotkey)
        {
            set(hotkey >> 8, hotkey & 0xFF);
        }
        inline
        int as_int() const
        {
            int mod = modifier();
            int vk  = virtkey();
            return mod << 8 | vk;
        }
    private:
    };

} // winui

#endif // WINUI_HOTKEY_H

