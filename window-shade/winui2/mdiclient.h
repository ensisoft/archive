//
// Copyright (c) 2004 Sami V�is�nen
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


// $Id: mdiclient.h,v 1.7 2008/02/24 15:24:57 enska Exp $

#ifndef WINUI_MDICLIENT_H
#define WINUI_MDICLIENT_H

#include "window.h"
#include <string>

namespace winui
{
    class mdiclient
    {
        public:
            CLIENTCREATESTRUCT ccs;
            WNDCLASSEX         wndcls;
            int                wndstyle;

            HWND client();
        protected:
           ~mdiclient();
            mdiclient();

            enum { WM_RETURN_VAL  = 0 };
            enum { WM_STARTUP = WM_USER + 1 };
            
            HWND create(WNDPROC proc, void* arg);
            void destroy(HWND hwnd);

            static 
            LRESULT default_handle_msg(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp);
        private:
            HWND client_;
            std::string classname_;
    };

} // winui

#endif // WINUI_MDICLIENT_H
