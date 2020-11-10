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

// $Id: hello.cpp,v 1.7 2008/02/24 15:24:57 enska Exp $

#include <ui.h>
#include <boost/bind.hpp>
#include "resource.h"

class hello_dialog
{
public:
    hello_dialog()
    {
        // connect our control object to the dialog object
        dlg_.connect(&ok_,  IDOK);
        dlg_.connect(&txt_, IDC_HELLO);
        dlg_.connect(&www_, IDC_WWW);
     
        // bind (add) an event (message) handler
        // so that when ok button is clicked our close member function is called
        ok_.evtclick  = boost::bind(&hello_dialog::close_dont_ask, this);
        www_.evtclick = boost::bind(&hello_dialog::www_click, this);
    }
    void show()
    {
        dlg_.evtclose  = boost::bind(&hello_dialog::close,  this);
        dlg_.evtcreate = boost::bind(&hello_dialog::create, this);
        dlg_.resid = IDHELLO; // dialog resource id
        dlg_.open();
    }
private:
    void create()
    {
        // this will get called just before the dialog is show
        // change the text of the static control
        txt_.text("hello world");
        dlg_.text("hello dialog");
        //www_.text("http://www.ensisoft.com");
    }
    void close()
    {
        // handle ok button click
        // do nothing except close the window
        int ret = MessageBox(dlg_.hwnd(), "Are you sure you want to close?", "Hello", MB_YESNO | MB_ICONEXCLAMATION);
        if (ret == IDYES)
            dlg_.close();
    }
    void www_click()
    {
        ShellExecute(
            dlg_.hwnd(),
            "open",
            "http://www.ensisoft.com",
            NULL,
            NULL,
            SW_SHOWNORMAL);
    }

    void close_dont_ask()
    {
        dlg_.close();
    }

    winui::modal_dialog dlg_;  
    winui::pushbutton   ok_;     
    winui::label        txt_;         
    winui::hyperlink    www_;
};


int APIENTRY WinMain(HINSTANCE, HINSTANCE, LPTSTR, int)
{
    hello_dialog dlg;
    dlg.show();
    return 0;
}
