
// $Id: split.cpp,v 1.8 2008/02/24 14:06:50 enska Exp $

#include <string>
#include <sstream>
#include <ui.h>
#include <boost/bind.hpp>
#include <boost/noncopyable.hpp>

using namespace winui;

// This paint policy draws the client area coordinates in the middle of the window.
class paint_coord
{
public:
protected:
   ~paint_coord() {}
    LRESULT paint(HWND hwnd, const PAINTSTRUCT& ps)
    {
        RECT rc;
        GetClientRect(hwnd, &rc);

        std::stringstream ss;
        std::string str;
        ss << rc.right << "x" << rc.bottom;
        ss >> str;

        SetBkMode(ps.hdc, TRANSPARENT);
        DrawText(ps.hdc, str.c_str(), str.length(), &rc, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
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
        window_.text("3-way split");
        window_.evtclose = boost::bind(&application::close_window, this);
        window_.evtsize  = boost::bind(&application::resize_window,  this);
        
        vert_split_.evtresize = boost::bind(&application::v_resplit, this, _1);
        hort_split_.evtresize = boost::bind(&application::h_resplit, this, _1);

        left_.parent    = window_.hwnd();
        left_.wndstyle  = WS_CHILD;
        left_.wndstyleex = WS_EX_CLIENTEDGE;
        left_.open();

        right_.parent    = window_.hwnd();
        right_.wndstyle  = WS_CHILD;
        right_.open();
        
        top_.parent    = right_.hwnd();
        top_.wndstyle  = WS_CHILD;
        top_.wndstyleex=WS_EX_CLIENTEDGE;
        top_.open();

        bottom_.parent     = right_.hwnd();
        bottom_.wndstyle   = WS_CHILD;
        bottom_.wndstyleex = WS_EX_CLIENTEDGE;
        bottom_.open();

        vert_split_.split(window_, true);
        vert_split_.moveto(100);
        
        hort_split_.split(right_, false);
        hort_split_.moveto(200);
    }
private:
    void close_window()
    {
        PostQuitMessage(0);
    }
    void resize_window()
    {
        vert_split_.adjust();
        hort_split_.adjust();
    }
    void v_resplit(const winui::splitter_evt_arg& arg)
    {
        left_.position(arg.pos_first);
        left_.size(arg.size_first);
        
        right_.position(arg.pos_second);
        right_.size(arg.size_second);
        
        SIZE top = top_.size();
        SIZE bot = bottom_.size();

        top_.size(arg.size_second.cx, top.cy);
        bottom_.size(arg.size_second.cx, bot.cy);
    }
    void h_resplit(const winui::splitter_evt_arg& arg)
    {
        top_.position(arg.pos_first);
        top_.size(arg.size_first);

        bottom_.position(arg.pos_second);
        bottom_.size(arg.size_second);
    }

    basic_window<sdiwindow, allevents> window_;             // top level window (with borders, titlebar etc.)
    basic_window<sdiwindow, noevents, paint_coord> left_;   // left panel child window
    basic_window<sdiwindow, noevents, paint_coord> right_;  // right panel child window
    basic_window<sdiwindow, noevents, paint_coord> top_;    // top panel of right child window
    basic_window<sdiwindow, noevents, paint_coord> bottom_; // bottom panel of right child window
    winui::splitter vert_split_; // vertical splitter (between left - right)
    winui::splitter hort_split_; // horizontal splitter (between top - bottom)
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
