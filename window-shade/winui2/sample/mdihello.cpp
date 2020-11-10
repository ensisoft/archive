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

// $Id: mdihello.cpp,v 1.5 2008/02/24 14:06:50 enska Exp $

#include <ui.h>
#include <vector>
#include <boost/bind.hpp>
#include <boost/noncopyable.hpp>

using namespace winui;

class hello_world
{
protected:
   ~hello_world() {}
    LRESULT paint(HWND hwnd, const PAINTSTRUCT& ps)
    {
        const char* str = "Hello World!";
        RECT rect;
        GetClientRect(hwnd, &rect);
        FillRect(ps.hdc, &rect, (HBRUSH)GetStockObject(COLOR_WINDOW));
        SetBkMode(ps.hdc, TRANSPARENT);
        SetTextColor(ps.hdc, RGB(0xFF, 0, 0));
        DrawText(ps.hdc, str, strlen(str), &rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
        return TRUE;
    }
    LRESULT erase(HWND)
    {
        return FALSE;
    }

};


class application : boost::noncopyable
{
public:
    application()
    {
        // the default style will be WS_OVERLAPPED which is just fine for us.
        // neither will we have a menu
        window_.open();
        window_.size(500, 500);
        window_.text("Hello World!");
        window_.evtclose = boost::bind(&application::close, this);

        // create a single child for now
        std::auto_ptr<child_window> child(new child_window);
        child_windows_.push_back(child.get());

        child->client = window_.client();
        child->open();
        child->text("Look ma' a MDI child window!");
        child.release();
    }
   ~application()
    {
        for (container::iterator it = child_windows_.begin(); it != child_windows_.end(); ++it)
            delete *it;
    }
private:
    void close()
    {
        // the window has been destroyed => exit
        PostQuitMessage(0);
    }

    basic_window<mdiclient, window_events, blank> window_;
  
    // for the sake of simplicity we'll reuse the same paint policy as in the sdi sample
    typedef basic_window<mdichild, noevents, hello_world> child_window;
    typedef std::vector<child_window*> container;

    container child_windows_;    
};


int WINAPI WinMain(HINSTANCE, HINSTANCE, LPTSTR, int)
{
    application app;

    MSG m;
    while (GetMessage(&m, 0, 0, 0))
    {
        TranslateMessage(&m);
        DispatchMessage(&m);
    }
    return 0;
}
