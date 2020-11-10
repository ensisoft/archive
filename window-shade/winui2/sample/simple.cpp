
// $Id: simple.cpp,v 1.3 2008/02/24 14:06:50 enska Exp $

#include <boost/bind.hpp>
#include <cassert>
#include <ui.h>

void quit(const winui::evtarg& arg)
{
    int ret = MessageBox(
        arg.hwnd,
        "Are you sure you want to close?",
        "Simple",
        MB_YESNO | MB_ICONEXCLAMATION);
    if (ret == IDYES)
        PostQuitMessage(0);
}

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPTSTR, int)
{
    using namespace winui;
    
    basic_window<sdiwindow, dynevent, blank> window;
    window.bind(WM_CLOSE, quit);
    window.open();

    MSG m;
    while (GetMessage(&m, 0, 0, 0))
    {
        TranslateMessage(&m);
        DispatchMessage(&m);
    }
    return 0;
}
