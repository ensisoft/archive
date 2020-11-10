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

// $Id: exception.h,v 1.2 2008/02/24 14:06:50 enska Exp $

#ifndef WINUI_EXCEPTION_H
#define WINUI_EXCEPTION_H

#include <exception>
#include <string>

namespace winui
{
    class uiexception : public std::exception
    {
    public:
        uiexception(const std::string& what, int err) : what_(what), code_(err)
        {
        }
       ~uiexception() throw ()
        {
        }
        const char* what() const throw()
        {
            return what_.c_str();
        }
        int errcode() const
        {
            return code_;
        }
    private:
        std::string what_;
        int code_;
    };
    
} // winui

#endif // WINUI_EXCEPTION_H

