
// $Id: settings.h,v 1.1 2006/12/27 01:19:29 enska Exp $

#ifndef SHADE_SETTINGS_H
#define SHADE_SETTINGS_H

#include <ui.h>

namespace shade
{
    struct settings;

    class settings_dlg
    {
    public:
        settings_dlg(settings* set, HWND owner);
        void show(HWND parent);
    private:
        void create();
        void close();
        void change_alpha();
        
        bool install_hotkey(const winui::hotkey& hk, int toggle, int& key) const;

        settings* set_;
        HWND owner_;
        winui::modal_dialog dlg_;
        winui::pushbutton close_;
        winui::checkbox start_;
        winui::checkbox icon_;
        winui::slider alpha_;
        winui::hotkey hk_shade_;
        winui::hotkey hk_alpha_;
        winui::hotkey hk_ontop_;
    };

} //

#endif
