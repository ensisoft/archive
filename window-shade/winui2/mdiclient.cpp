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


// $Id: mdiclient.cpp,v 1.6 2008/02/24 15:24:57 enska Exp $

#include "mdiclient.h"
#include "common.h"
#include "exception.h"
#include <cassert>

namespace winui
{

mdiclient::mdiclient() : wndstyle(WS_OVERLAPPEDWINDOW), client_(0)
{
    memset(&wndcls, 0, sizeof(WNDCLASSEX));
    wndcls.cbSize        = sizeof(WNDCLASSEX);
    wndcls.hInstance     = GetModuleHandle(NULL);
    wndcls.hCursor       = LoadCursor(NULL, IDC_ARROW);
    wndcls.style         = CS_VREDRAW | CS_HREDRAW | CS_DBLCLKS;
    ccs.hWindowMenu  = 0;
    ccs.idFirstChild = 0;
    classname_ = create_class_name(typeid(mdiclient).name());
}

mdiclient::~mdiclient()
{
    if (client_)
    {
       BOOL ret =  DestroyWindow(client_);
       assert(ret == TRUE);
       ret = 0;
    }
}

HWND mdiclient::client() 
{
    return client_;
}

HWND mdiclient::create(WNDPROC proc, void* arg)
{
    wndcls.lpfnWndProc   = proc;
    wndcls.lpszClassName = classname_.c_str();
    if (!RegisterClassEx(&wndcls))
    {
        int err = GetLastError();
        throw uiexception("failed to register class (RegisterClassEx)", err);
    }
    HWND hwnd = CreateWindow(wndcls.lpszClassName,
        wndcls.lpszClassName,
        wndstyle,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        NULL,
        NULL, 
        wndcls.hInstance,
        NULL);

    if (!hwnd)
        throw uiexception("failed to create window (CreateWindow)", GetLastError());

    // create the mdi client window
    client_ = CreateWindow("MDICLIENT",
        NULL, 
        WS_CHILD | WS_CLIPCHILDREN,
        0, 0, 0, 0, 
        hwnd, 
        NULL, 
        wndcls.hInstance,
        &ccs);
    if (!client_)
    {
        int err = GetLastError();
        DestroyWindow(hwnd);
        throw uiexception("failed to create MDICLIENT window (CreateWindow)", err);
    }
    BOOL ret = SetProp(hwnd, "client_", client_);
    assert(ret == TRUE);
    ret = 0;

    ShowWindow(client_, SW_SHOW);
    ShowWindow(hwnd, SW_SHOW);
    SendMessage(hwnd, WM_STARTUP, 0, reinterpret_cast<LPARAM>(arg));
    UpdateWindow(hwnd);
    return hwnd;
}

void mdiclient::destroy(HWND hwnd)
{
    BOOL ret = FALSE;
    ret = SetProp(hwnd, "client_", 0);
    assert(ret == TRUE);
    ret = DestroyWindow(client_);
    assert(ret == TRUE);
    ret = DestroyWindow(hwnd);
    assert(ret == TRUE);
    client_ = 0;
}

LRESULT mdiclient::default_handle_msg(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp)
{
    HWND client = static_cast<HWND>(GetProp(hwnd, "client_"));
    return DefFrameProc(hwnd, client, msg, wp, lp);
}

} //
