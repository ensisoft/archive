
// $Id: shade.cpp,v 1.2 2007/01/01 19:08:09 enska Exp $
#define WINVER 0x0501
#define _WIN32_WINNT 0x0501
#define  IMPORT_API

#include <ui.h>
#include <exception>
#include <boost/bind.hpp>
#include <win/library.h>
#include "res/resource.h"
#include "libshade.h"
#include "registry.h"
#include "settings.h"
#include "shade.h"

using namespace shade;
using namespace winui;
using namespace std;

settings gSettings;
trayicon gIcon;
view<msgimpl, dynevent> gMsg;
container gWindows;

void click_icon(const evtarg& args)
{
    if (args.l != WM_RBUTTONUP)
        return;

    static HMENU root = LoadMenu(GetModuleHandle(NULL), MAKEINTRESOURCE(IDR_MENU1));
    HMENU menu = GetSubMenu(root, 0);

    POINT pt;
    GetCursorPos(&pt);

    SetForegroundWindow(gMsg.hwnd()); // http://support.microsoft.com/kb/q135788/

    UINT cmd = TrackPopupMenu(menu,
        TPM_BOTTOMALIGN | TPM_RIGHTALIGN | TPM_RETURNCMD,
        pt.x,
        pt.y,
        0,
        gMsg.hwnd(),
        NULL);

    switch (cmd)
    {
        case ID_MENU_ABOUT:
            MessageBox(gMsg.hwnd(),
                "Window Shade (c) 2005, 2006 Sami Väisänen",
                "Window Shade",
                MB_OK | MB_ICONINFORMATION);
            break;
        case ID_MENU_EXIT:
            PostQuitMessage(0);
            break;
        case ID_MENU_CONFIGURE:
            {
                EnableMenuItem(menu, ID_MENU_CONFIGURE, MF_GRAYED);
                settings_dlg dlg(&gSettings, gMsg.hwnd());
                dlg.show(gMsg.hwnd());
                EnableMenuItem(menu, ID_MENU_CONFIGURE, MF_ENABLED);

                if (gSettings.hideicon)
                    gIcon.rem();
            }
            break;
    }
}

void restore_icon(const evtarg& args)
{
    if (gSettings.hideicon)
    {
        gIcon.add("Window Shade",
            gMsg.hwnd(),
            WM_ICON_CLICKED,
            IDI_ICON1);
        gSettings.hideicon = false;
    }
}

void set_ontop(HWND hwnd)
{
    DWORD style = GetWindowLong(hwnd, GWL_EXSTYLE);
    if (style & WS_EX_TOPMOST)
    {
        SetWindowLong(hwnd, GWL_EXSTYLE, style & ~WS_EX_TOPMOST);
        SetWindowPos(hwnd, 
            HWND_NOTOPMOST, 
            0, 0, 0, 0, 
            SWP_NOMOVE | SWP_NOSIZE | SWP_FRAMECHANGED);
    }
    else
    {
    	SetWindowLong(hwnd, GWL_EXSTYLE, style | WS_EX_TOPMOST);
    	SetWindowPos(hwnd, 
            HWND_TOPMOST, 
            0, 0, 0, 0, 
            SWP_NOMOVE | SWP_NOSIZE | SWP_FRAMECHANGED);
    }    
}

void set_transparency(HWND hwnd)
{
    DWORD style = GetWindowLong(hwnd, GWL_EXSTYLE);
    if (style & WS_EX_LAYERED)
    {
        SetWindowLongPtr(hwnd, GWL_EXSTYLE, style & ~WS_EX_LAYERED);
    }
    else
    {   
        SetWindowLongPtr(hwnd, GWL_EXSTYLE, WS_EX_LAYERED);
        SetLayeredWindowAttributes(hwnd,
            0,
            255 - gSettings.alpha,
            LWA_ALPHA);
    }
}

void shade_window(HWND hwnd)
{
    RECT wndsize;
    GetWindowRect(hwnd, &wndsize);

    int height = wndsize.bottom - wndsize.top;
    
    container::iterator it = gWindows.find(hwnd);
    if (it != gWindows.end() && height == it->second.shaded_height)
    {
        // unshade
        RemoveProp(hwnd, "_shaded_");
        MoveWindow(hwnd, 
            wndsize.left, 
            wndsize.top, 
            wndsize.right - wndsize.left, 
            it->second.original_height, 
            TRUE);
    }
    else
    {
        if (IsZoomed(hwnd))
        {
            WINDOWPLACEMENT placement = {};
            placement.length = sizeof(placement);

            GetWindowPlacement(hwnd, &placement);
            placement.showCmd = SW_RESTORE;
            SetWindowPlacement(hwnd, &placement);
        }

        SetProp(hwnd, "_shaded_", (HANDLE)TRUE);
        MoveWindow(hwnd, 
            wndsize.left, 
            wndsize.top, 
            wndsize.right - wndsize.left, 
            GetSystemMetrics(SM_CYMIN), 
            TRUE);

        // get real window size
        RECT real;
        GetWindowRect(hwnd, &real);

        window_tag wt;
        wt.original_height = height;
        wt.shaded_height   = real.bottom - real.top;
        gWindows[hwnd] = wt;
    }
}  

void hotkey_press(const evtarg& args)
{
    HWND hwnd = GetForegroundWindow();
    
    if (hwnd == GetDesktopWindow() ||
        hwnd == GetShellWindow())
        return;

    DWORD dStyle = GetWindowLongPtr(hwnd, GWL_STYLE);
    if (!(dStyle & WS_OVERLAPPED)       &&
        !(dStyle & WS_OVERLAPPEDWINDOW) &&
        !(dStyle & WS_POPUP))
        return;

    switch (args.w)
    {
        case toggle_ontop:
            set_ontop(hwnd);
            break;
        case toggle_transparency:
            set_transparency(hwnd);
            break;
        case toggle_shade:
            shade_window(hwnd);
            break;
    }
}

void load_settings(settings& set)
{
    registry r;
    if (r.open("Software\\Window Shade"))
    {
        if (has_value(r.key, "hk_ontop"))
            set.hk_ontop = read_int(r.key, "hk_ontop");
        if (has_value(r.key, "hk_shade"))
            set.hk_shade = read_int(r.key, "hk_shade");
        if (has_value(r.key, "hk_trans"))
            set.hk_trans = read_int(r.key, "hk_trans");
        if (has_value(r.key, "alpha"))
            set.alpha = read_int(r.key, "alpha");
        if (has_value(r.key, "hideicon"))
            set.hideicon = read_int(r.key, "hideicon");
    }
    r.close();
    set.winstart = false;
    // see if we are configured to run with windows automatically.
    if (r.open("Software\\Microsoft\\Windows\\CurrentVersion\\Run"))
    {
        if (has_value(r.key, "WindowShade"))
            set.winstart = true;
    }
}

void save_settings(const settings& set)
{
    registry r;
    r.create("Software\\Window Shade");

    write_int(r.key, "hk_ontop", set.hk_ontop);
    write_int(r.key, "hk_shade", set.hk_shade);
    write_int(r.key, "hk_trans", set.hk_trans);
    write_int(r.key, "alpha",    set.alpha);
    write_int(r.key, "hideicon", set.hideicon);
    r.close();

    r.create("Software\\Microsoft\\Windows\\CurrentVersion\\Run");
    if (set.winstart)
    {
        char bork[255] = {};
        GetModuleFileName(NULL, bork, sizeof(bork));

        std::string s(bork);
        write_string(r.key, "WindowShade", s);
    }
    else
    {
        delete_value(r.key, "WindowShade");
    }
}

void install_keys(settings& set, HWND where)
{
    if (set.hk_shade)
    {
        if (!RegisterHotKey(where, 
                toggle_shade, 
                set.hk_shade >> 8,
                set.hk_shade & 0xFF))
            set.hk_shade = 0;
    }
    if (set.hk_ontop)
    {
        if (!RegisterHotKey(where,
                toggle_ontop,
                set.hk_ontop >> 8,
                set.hk_ontop & 0xFF))
            set.hk_ontop = 0;
    }
    if (set.hk_trans)
    {
        if (!RegisterHotKey(where,
                toggle_transparency,
                set.hk_trans >> 8,
                set.hk_trans & 0xFF))
            set.hk_trans = 0;
    }
}   

void undo_all()
{
    for (container::iterator it = gWindows.begin();
        it != gWindows.end();
        ++it)
    {
        HWND hwnd = it->first;
        RECT rc;
        if (GetWindowRect(hwnd, &rc))
        {
            RemoveProp(hwnd, "_shaded_");
            MoveWindow(hwnd,
                rc.left,
                rc.top,
                rc.right - rc.left,
                it->second.original_height,
                TRUE);
            //if (it->second.alpha)
        }
    }

    gWindows.clear();

    // invalidate all windows on the desktop
    InvalidateRect(NULL, NULL, TRUE);

}

void end_session(const winui::evtarg&)
{
    save_settings(gSettings);
}

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPTSTR, int)
{
    if (LOBYTE(GetVersion()) != 5)
    {
        MessageBox(
            NULL, 
            "Window Shade requires either Windows 2000 or Windows XP",
            "Window Shade",
            MB_OK | MB_ICONEXCLAMATION);
        return 0;
    }
    HANDLE mutex = CreateMutex(NULL, TRUE, "_shade_");
    if (mutex && GetLastError() == ERROR_ALREADY_EXISTS)
    {
        CloseHandle(mutex);
        HWND prev = FindWindow("_shade_", NULL);
        if (prev)
            SendMessage(prev, WM_RESTORE_ICON, 0, 0);
        return 0;
    }
    try
    {
        win::library hook;
        hook.load("libshade.dll");
        hook.procedure<install_func>("install")();

        gMsg.bind(WM_RESTORE_ICON, restore_icon);
        gMsg.bind(WM_ICON_CLICKED, click_icon);
        gMsg.bind(WM_HOTKEY,       hotkey_press);
        gMsg.bind(WM_ENDSESSION,   end_session);
        gMsg.classname = "_shade_";
        gMsg.open();

        load_settings(gSettings);

        if (!gSettings.hideicon)
        {
            gIcon.add("Window Shade", gMsg.hwnd(), WM_ICON_CLICKED, IDI_ICON1);
        }
        install_keys(gSettings, gMsg.hwnd());
        
        MSG m;
        while (GetMessage(&m, 0, 0, 0))
        {
            TranslateMessage(&m);
            DispatchMessage(&m);
        }

        hook.procedure<install_func>("uninstall")();

        save_settings(gSettings);
        undo_all();
    }
    catch (const exception& e)
    {
        MessageBox(NULL, e.what(),"Window Shade", MB_ICONERROR);
    }
    CloseHandle(mutex);
    return 0;    
}

