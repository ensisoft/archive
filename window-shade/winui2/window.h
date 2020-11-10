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


// $Id: window.h,v 1.8 2008/02/24 14:06:50 enska Exp $

#ifndef WINUI_WINDOW_H
#define WINUI_WINDOW_H

#include <windows.h>
#include <string>

namespace winui
{
    // Window class is the root class of the windowing hierarchy.
    // This class cannot be used directly, but just defines an interface for windows.
    class window
    {
    public:
        virtual ~window();

        // Request the window to be updatet.
        virtual void update();
        
        // Invalidate the whole window client area.
        virtual void invalidate();

        // Hide the window. 
        virtual void hide();
        
        // Show the window.
        virtual void show();
        
        // Enable the window (an enabled can gain focus and accept input).
        virtual void enable();
        
        // Disable the window (a disabled window cannot gain focus or accept input).
        virtual void disable();

        // Set the window position relative to its parent. 
        // Or relative to the desktop if the window is a top level window.
        virtual void position(int x, int y);
        virtual void position(const POINT& pos);

        // Set the window size. (outer size, client area will be less than this).
        virtual void size(int cx, int cy);
        virtual void size(const SIZE& s);
        
        // Set input focus on the window.
        virtual void focus() const;
        

        // Returns true if the window has been created, i.e. "exists" with a handle,
        // otherwise return false.
        bool exists() const;

        // Returns true if window is visible, otherwise false.
        bool is_visible() const;
        

        // Returns true if window is enabled, otherwise false.
        bool is_enabled() const;


        // Returns true if window is focused, otherwise false.
        bool is_focused() const;
        
        // Get window position relative to its parent.
        POINT position() const;
        

        // Get window rectangle.
        RECT  rectangle() const;
        
        // Get window client area rectangle.
        RECT  client_rectangle() const;
        
        // Get window size. 
        SIZE  size() const;
        

        // Get window client area size.
        SIZE  client_size() const;
        

        // Get window text (caption).
        std::string text() const;
        
        // Set window text.
        void text(const std::string& str);

        // Get internal window handle.
        HWND hwnd();
    protected:
        window();
        // it might be possible to assign concrete windows to each other
        // but its not possible through the window inteface.
        window(const window& w);
        window& operator=(const window& w);
        
        HWND hwnd_;
    };

} // winui

#endif // WINUI_WINDOW_H
