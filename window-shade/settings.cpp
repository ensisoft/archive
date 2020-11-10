
#define WINVER 0x0501
#define _WIN32_WINNT 0x0501

#include "res/resource.h"
#include "settings.h"
#include "shade.h"

namespace shade
{

settings_dlg::settings_dlg(settings* set, HWND owner) :
  set_(set),
  owner_(owner),
  close_(IDCANCEL),
  start_(IDC_CHECK1),
  icon_ (IDC_CHECK3),
  alpha_(IDC_SLIDER1),
  hk_shade_(IDC_HOTKEY2),
  hk_alpha_(IDC_HOTKEY3),
  hk_ontop_(IDC_HOTKEY1)
{
    dlg_.connect(close_);
    dlg_.connect(start_);
    dlg_.connect(icon_);
    dlg_.connect(alpha_);
    dlg_.connect(hk_shade_);
    dlg_.connect(hk_alpha_);
    dlg_.connect(hk_ontop_);

    dlg_.evtcreate   = boost::bind(&settings_dlg::create, this);
    close_.evtclick  = boost::bind(&settings_dlg::close,  this);
    alpha_.evtchange = boost::bind(&settings_dlg::change_alpha, this);

}

void settings_dlg::show(HWND parent)
{
    dlg_.resid  = IDD_DIALOG1;
    dlg_.parent = parent;
    dlg_.open();
          
}

void settings_dlg::create()
{
    start_.check(set_->winstart);
    icon_.check(set_->hideicon);
          
    hk_shade_.set(set_->hk_shade);
    hk_alpha_.set(set_->hk_trans);
    hk_ontop_.set(set_->hk_ontop);

    // dont let the window reach full 
    // transparency cause it gets lost easily then.
    alpha_.set_range(0, 220);
    alpha_.setpos(set_->alpha);
    SetWindowLongPtr(dlg_.hwnd(), GWL_EXSTYLE, WS_EX_LAYERED);
    SetLayeredWindowAttributes(dlg_.hwnd(), 0, 255 - set_->alpha, ULW_ALPHA);
}

void settings_dlg::close()
{
    if (!install_hotkey(hk_shade_, toggle_shade, set_->hk_shade))
        return;
    if (!install_hotkey(hk_ontop_, toggle_ontop, set_->hk_ontop))
        return;
    if (!install_hotkey(hk_alpha_, toggle_transparency, set_->hk_trans))
        return;

    set_->hideicon = icon_.is_checked();
    set_->winstart = start_.is_checked();
          
    if (set_->alpha == 0) // do blend only on fully opaque window.
    {
        LONG_PTR style = GetWindowLongPtr(dlg_.hwnd(), GWL_EXSTYLE);
        SetWindowLongPtr(dlg_.hwnd(), GWL_EXSTYLE, style & ~WS_EX_LAYERED);
        AnimateWindow(dlg_.hwnd(), 400, AW_BLEND | AW_HIDE);
    }
    dlg_.close();
}

void settings_dlg::change_alpha()
{
    set_->alpha = alpha_.getpos();
    SetLayeredWindowAttributes(dlg_.hwnd(), 0, 255 - set_->alpha, ULW_ALPHA);

}

bool settings_dlg::install_hotkey(const winui::hotkey& hk, int toggle, int& key) const
{
    int val = hk.as_int();
    if (val == key)
        return true;

    if (val == 0 && key != 0)
    {
        UnregisterHotKey(owner_, toggle);
        key = 0;
        return true;
    }
          
    UnregisterHotKey(owner_, toggle);
    if (RegisterHotKey(owner_, toggle, hk.modifier(), hk.virtkey()))
    {
        key = val;
        return true;
    }
    MessageBeep(0);
    hk.focus();
    return false;
}

} //

