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

// $Id: ui.h,v 1.15 2008/02/24 15:24:57 enska Exp $

#ifndef WINUI_UI_H
#define WINUI_UI_H

#include "control.h"
#include "dialog.h"
#include "editbox.h"
#include "window.h"
#include "listbox.h"
#include "button.h"
#include "combobox.h"
#include "link.h"
#include "slider.h"
#include "menu.h"
#include "events.h"
#include "msgwindow.h"
#include "sdiwindow.h"
#include "mdiclient.h"
#include "mdichild.h"
#include "trayicon.h"
#include "hotkey.h"
#include "blank.h"
#include "splitter.h"
#include "treeview.h"
#include "basic_window.h"
#include "basic_control.h"

namespace winui
{
    typedef button<null_events, button_events> pushbutton;
    typedef button<null_events, button_events> checkbox;
    typedef button<null_events, button_events> radiobutton;
    typedef slider<null_events, slider_events> trackbar;
    typedef combobox<null_events, box_events> combo;
    typedef listbox<null_events, box_events> list;
    typedef editbox<> edit;
    typedef basic_control<> label;
    typedef basic_control<> static_text;
    typedef treeview<> tree;
    typedef basic_window<modal_dialog_window,    window_events, blank> modal_dialog;
    typedef basic_window<modeless_dialog_window, window_events, blank> nonmodal_dialog;

} // winui

#endif // WINUI_UI_H
