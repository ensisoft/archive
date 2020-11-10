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

// $Id: dialog.h,v 1.8 2008/02/24 15:24:57 enska Exp $

#ifndef WINUI_DIALOG_H
#define WINUI_DIALOG_H

#include <cassert>

namespace winui
{
    // A modal dialog is a dialog that wont allow the application
    // execution to resume untill the dialog has been closed.
    class modal_dialog_window
    {
    public:
        int  resid;     // This should be the resource ID identifying the dialog template.
        HWND parent;    // This should be the parent window's handle. (if any)
    protected:
        enum { WM_STARTUP    = WM_INITDIALOG, 
               WM_RETURN_VAL = TRUE };

        modal_dialog_window() : resid(0), parent(0) {}
       ~modal_dialog_window() {}
        inline
        HWND create(WNDPROC proc, void* arg)
        {
            // this wont return untill the dialog is 
            // closed by calling EndDialog
            DialogBoxParam(
                GetModuleHandle(NULL),
                MAKEINTRESOURCE(resid),
                parent,
                reinterpret_cast<DLGPROC>(proc),
                reinterpret_cast<LPARAM>(arg));
            return 0;
        }
        inline
        void destroy(HWND hwnd)
        {
            BOOL ret = EndDialog(hwnd, 0);
            assert(ret == TRUE);
            ret = 0;
        }
        static inline
        LRESULT default_handle_msg(HWND, UINT, WPARAM, LPARAM)
        {
            return FALSE;
        }
    private:
    };

    // a modeless dialog is a dialog that allows
    // the execution to continue while the dialog is shown/open.
    class modeless_dialog_window
    {
    public:
        int  resid;     // This should be the resource ID identifying the dialog template.
        HWND parent;    // This should be the parent window's handle. (if any)
    protected:
        enum { WM_STARTUP    = WM_INITDIALOG,
               WM_RETURN_VAL = TRUE };

        modeless_dialog_window() : resid(0), parent(0) {}
       ~modeless_dialog_window() {}
        inline
        HWND create(WNDPROC proc, void* arg)
        {
            // Create the window using a dialog resource.
            // This is basically just a "window" and will exists untill 
            // destroyed.
            HWND hwnd = CreateDialogParam(
                GetModuleHandle(NULL),
                MAKEINTRESOURCE(resid),
                parent,
                reinterpret_cast<DLGPROC>(proc),
                reinterpret_cast<LPARAM>(arg));
            assert(hwnd);
            ShowWindow(hwnd, SW_SHOW);
            return hwnd;
        }
        inline
        void destroy(HWND hwnd)
        {
            BOOL ret = DestroyWindow(hwnd);
            assert(ret == TRUE);
            ret = 0;
        }
        static inline
        LRESULT default_handle_msg(HWND, UINT, WPARAM, LPARAM)
        {
            return FALSE;
        }
    private:
    };    

} //  winui

#endif // WINUI_DIALOG_H
