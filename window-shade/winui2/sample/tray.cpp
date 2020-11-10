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

// $Id: tray.cpp,v 1.4 2008/02/24 14:06:50 enska Exp $

#include <ui.h>
#include <boost/bind.hpp>
#include "resource.h"

using namespace winui;

class hello_dialog
{
public:
    hello_dialog()
    {
        dlg_.connect(&ok_,  IDOK);
        dlg_.connect(&txt_, IDC_HELLO);
        ok_.evtclick = boost::bind(&hello_dialog::close, this);
    }
    void show()
    {
        dlg_.evtcreate = boost::bind(&hello_dialog::create, this);
        dlg_.parent    = GetDesktopWindow();
        dlg_.resid     = IDHELLO;
        dlg_.open();
    }
private:
    void close()
    {
        dlg_.close();
    }
    void create()
    {
        txt_.text("hello world\r\nthe application will now exit");
        dlg_.text("hello_dialog");
    }
    pushbutton   ok_;
    label        txt_;
    modal_dialog dlg_;
};


void icon_clicked(const winui::evtarg& args)
{
    if (args.lParam != WM_LBUTTONUP)
        return;

    hello_dialog dlg;
    dlg.show();
    PostQuitMessage(0);
}

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPTSTR, int)
{
    enum { WM_MY_MESSAGE = WM_USER + 1 };

    // create a message only window
    basic_window<msgwindow, dynevent> msg;
    trayicon ico;

    msg.bind(WM_MY_MESSAGE, icon_clicked);
    msg.open();

    ico.add("hello world tray icon", msg.hwnd(), WM_MY_MESSAGE, IDI_ICON1);

    MSG m;
    while (GetMessage(&m, 0, 0, 0))
    {
        TranslateMessage(&m);
        DispatchMessage(&m);
    }
}
