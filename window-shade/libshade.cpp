
// $Id: libshade.cpp,v 1.3 2007/01/01 19:08:09 enska Exp $

#include "libshade.h"
#include <cassert>

HINSTANCE hinstance;

#pragma data_seg(".SHADE")
HHOOK hook = NULL;
#pragma data_seg()
#pragma comment(linker, "/section:.SHADE,rws")

BOOL __stdcall DllMain(HINSTANCE hInst, DWORD, LPVOID)
{
    hinstance = hInst;
    return TRUE;
}

LRESULT
CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    HANDLE hdl = GetProp(hwnd, "_old_proc_");
    assert(hdl);
    RemoveProp(hwnd, "_old_proc_");
    SetWindowLongPtr(hwnd, GWL_WNDPROC, reinterpret_cast<LONG_PTR>(hdl));

    switch (uMsg)
    {
        case WM_GETMINMAXINFO:
        {
            MINMAXINFO* minmax = reinterpret_cast<MINMAXINFO*>(lParam);
            minmax->ptMinTrackSize.y = GetSystemMetrics(SM_CYMIN);
        }
        break;
        case WM_WINDOWPOSCHANGING:
        {
            WINDOWPOS* wpos    = reinterpret_cast<WINDOWPOS*>(lParam);
            wpos->cy = GetSystemMetrics(SM_CYMIN);
        }
        break;
        case WM_WINDOWPOSCHANGED:
        break;

        // eh, where do we get other message types?
        default:
            return CallWindowProc(
                reinterpret_cast<WNDPROC>(hdl),
                hwnd,
                uMsg,
                wParam,
                lParam);
            break;

        /*
        default:
        assert(false);*/
    }
    return 0;    
}

LRESULT 
CALLBACK CallWndProc(int nCode, WPARAM wParam, LPARAM lParam)
{
    if (nCode < 0)
    {
        CallNextHookEx(hook,
            nCode,
            wParam,
            lParam);
        return 0;
    }

    CWPSTRUCT* cw = reinterpret_cast<CWPSTRUCT*>(lParam);

    if (cw->message == WM_GETMINMAXINFO ||
        cw->message == WM_WINDOWPOSCHANGING ||
        cw->message == WM_WINDOWPOSCHANGED)
    {
        if (GetProp(cw->hwnd, "_shaded_"))
        {
            LONG_PTR proc =  SetWindowLongPtr(cw->hwnd, 
                GWL_WNDPROC, 
                reinterpret_cast<LONG_PTR>(WndProc));
                                              
            SetProp(cw->hwnd,
                "_old_proc_",
                reinterpret_cast<HANDLE>(proc));
        }
    }
    return CallNextHookEx(hook, nCode, wParam, lParam);
}

extern "C"
{

DLLFUNC void install()
{
    hook = SetWindowsHookEx(WH_CALLWNDPROC, CallWndProc, hinstance, 0);
}

DLLFUNC void uninstall()
{
    UnhookWindowsHookEx(hook);
}

}
//
