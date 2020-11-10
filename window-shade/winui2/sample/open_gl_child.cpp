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


// $Id: open_gl_child.cpp,v 1.4 2008/02/24 14:06:50 enska Exp $

#include <ui.h>
#include <commctrl.h>
#include <boost/bind.hpp>
#include "pyramid.h"
#include "resource.h"

#pragma comment(lib, "comctl32.lib")

using namespace winui;

void quit()
{
    PostQuitMessage(0);
}

class window
{
public:
    window() :
      rotation_x_(0),
      rotation_y_(0),
      rotation_z_(0),
      speed_x_(0),
      speed_y_(0.3),
      speed_z_(0)
    {
        InitCommonControls();

        x_.evtchange = boost::bind(&window::xspeed, this);
        y_.evtchange = boost::bind(&window::yspeed, this);
        z_.evtchange = boost::bind(&window::zspeed, this);
        
        dlg_.connect(&x_, IDC_SLIDER1);
        dlg_.connect(&y_, IDC_SLIDER2);
        dlg_.connect(&z_, IDC_SLIDER3);
                
        dlg_.evtclose = quit;
        dlg_.resid    = IDD_DIALOG1;
        dlg_.open();

        x_.set_range(0, 10);
        y_.set_range(0, 10);
        z_.set_range(0, 10);
        y_.setpos(3);

        pyramid_.wndstyle   = WS_CHILD;
        pyramid_.wndstyleex = WS_EX_CLIENTEDGE;
        pyramid_.wndcls.hbrBackground = NULL;
        pyramid_.parent     = dlg_.hwnd();
        pyramid_.open();
        pyramid_.position(20, 20);
        pyramid_.size(200, 200);
        pyramid_.gl_init(pyramid_.hwnd());
        pyramid_.gl_resize(200, 200);
    }
    void render()
    {
        pyramid_.gl_render(rotation_x_, rotation_y_, rotation_z_);
        rotation_x_ += speed_x_;
        rotation_y_ += speed_y_;
        rotation_z_ += speed_z_;
    }
private:
    void xspeed()
    {
        speed_x_ = x_.getpos() / 10.0;
    }
    void yspeed()
    {
        speed_y_ = y_.getpos() / 10.0;
    }
    void zspeed()
    {
        speed_z_ = z_.getpos() / 10.0;
    }

    void destroy()
    {
        PostQuitMessage(0);
    }

    nonmodal_dialog dlg_;
    trackbar x_;
    trackbar y_;
    trackbar z_;
    basic_window<sdiwindow, noevents, pyramid> pyramid_;

    GLfloat rotation_x_;
    GLfloat rotation_y_;
    GLfloat rotation_z_;
    GLfloat speed_x_;
    GLfloat speed_y_;
    GLfloat speed_z_;
};

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPTSTR, int)
{
    ::window w;

    while (true)
    {
        MSG m;
        if (PeekMessage(&m, NULL, 0, 0, PM_REMOVE))
        {
            if (m.message == WM_QUIT)
                break;

            TranslateMessage(&m);
            DispatchMessage(&m);
        }
        w.render();
    }
    return 0;
}
