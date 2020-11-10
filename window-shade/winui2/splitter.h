

// $Id: splitter.h,v 1.4 2008/02/24 14:06:50 enska Exp $

#ifndef WINUI_SPLITTER_H
#define WINUI_SPLITTER_H

#include <boost/function.hpp>
#include <boost/noncopyable.hpp>
#include "events.h"
#include "sdiwindow.h"
#include "basic_window.h"

namespace winui
{
    // a resize argument
    // that contains new sizes for the window halves
    struct splitter_evt_arg
    {
        SIZE size_first;        // size of left/top
        SIZE size_second;       // sizeof right/bottom
        POINT pos_first;        // position of left/top
        POINT pos_second;       // position of the right/bottom
    };
    
    class splitter : boost::noncopyable
    {
    public:
        boost::function1<void, splitter_evt_arg> evtresize;

        splitter();
       ~splitter();

        void split(window& wnd, bool vertical);
        void moveto(int pos);
        void adjust();
    private:
        class dragblt
        {
        protected:
           ~dragblt() {}
            LRESULT paint(HWND hwnd, const PAINTSTRUCT& ps)
            {
                RECT rc;
                GetClientRect(hwnd, &rc);
                FillRect(ps.hdc, &rc,(HBRUSH)GetStockObject(BLACK_BRUSH));
                return TRUE;
            }
            LRESULT erase(HWND) 
            {
                return FALSE;
            }
        private:
        };

        void mouse_capture(const evtarg&);
        void mouse_release(const evtarg&);
        void mouse_move(const evtarg&);
        void rehash() const;
        
        POINT splitter_pos_;
        POINT mouse_pos_;
        bool captured_;
        bool vertical_;
        window* window_;
        basic_window<sdiwindow, mouseevents>        splitter_;
        basic_window<sdiwindow, noevents, dragblt>  dragbar_;
    };

} // winui
 
#endif // WINUI_SPLITTER_H
