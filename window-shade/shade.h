
// $Id: shade.h,v 1.1 2006/12/27 01:19:29 enska Exp $

#ifndef SHADE_H
#define SHADE_H

#include <windows.h>
#include <map>

namespace shade
{

enum {
    WM_RESTORE_ICON = WM_USER + 1,
    WM_ICON_CLICKED = WM_USER + 2
};

enum {
    toggle_ontop,
    toggle_shade,
    toggle_transparency
};

struct settings
{
    int hk_ontop;
    int hk_shade;
    int hk_trans;
    int alpha;
    bool winstart;
    bool hideicon;
};

struct window_tag
{
    int shaded_height;
    int original_height;
};

typedef
std::map<HWND, window_tag> container;

} // 

#endif
