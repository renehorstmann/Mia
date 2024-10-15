#include "w/WWindowDialog.h"
#include "o/OObj_builder.h"
#include "w/WBox.h"
#include "w/WColor.h"
#include "w/WIcon.h"
#include "w/WText.h"
#include "w/WTextShadow.h"
#include "w/WGrid.h"
#include "w/WBtn.h"
#include "w/WIcon.h"
#include "w/WStyle.h"
#include "u/rect.h"

#define O_LOG_LIB "w"
#include "o/log.h"


static const vec2 HEADER_MIN_SIZE = {{32, 10}};


O_STATIC
void on_btn_cancel_clicked(oobj btn)
{
    oobj dialog_obj = o_user(btn);
    OObj_assert(dialog_obj, WWindowDialog);
    WWindowDialog *self = dialog_obj;
    
    self->state = WWindowDialog_CANCEL;
}

O_STATIC
void on_btn_ok_clicked(oobj btn)
{
    oobj dialog_obj = o_user(btn);
    OObj_assert(dialog_obj, WWindowDialog);
    WWindowDialog *self = dialog_obj;
    
    self->state = WWindowDialog_OK;
}

//
// public
//

WWindowDialog *WWindowDialog_init(oobj obj, oobj parent, const char *title, bool show_cancel, bool show_ok)
{
    WWindow *super = obj;
    WWindowDialog *self = obj;
    o_clear(self, sizeof *self, 1);

    WWindow_init(obj, parent, HEADER_MIN_SIZE);
    
    OObj_id_set(self, WWindowDialog_ID);
    
    WWindow_hideable_set(self, false);
    
    self->bg = WColor_new(self, R_BLACK);
    WObj_fixed_size_set(self->bg, vec2_(0));
    
    self->state = WWindowDialog_RUNNING;
    
    oobj txt = WTextShadow_new(WWindow_header_align(self), title);

    oobj grid = WGrid_new(WWindow_body(self), 2, 2, vec2_(-1));
    
    oobj hbox = WBox_new(grid, WBox_LAYOUT_H);
    WGrid_child_cell_set(hbox, ivec4_(0, 0, 2, 1));
    
    self->icon_pane = WPane_new(hbox);
    WObj_padding_set(self->icon_pane, vec4_(2,4,2));
    WPane_style_set(self->icon_pane, WPane_ROUND);
    
    self->icon = WIcon_new(self->icon_pane, WTheme_ICON_EXCLAMATION);
    
    self->body = WObj_new(hbox);
    
    if(show_cancel) {
        self->btn_cancel = WBtn_new(grid);
        o_user_set(self->btn_cancel, self);
        WBtn_auto_event_set(self->btn_cancel, on_btn_cancel_clicked);
        WBtn_auto_mode_set(self->btn_cancel, WBtn_AUTO_CLICKED);
        WGrid_child_cell_set(self->btn_cancel, ivec4_(0, 1, 1, 1));
        WObj_padding_set(self->btn_cancel, vec4_(4, 0));
        oobj btn_cancel_icon = WIcon_new(self->btn_cancel, WTheme_ICON_CROSS);
        WIcon_color_set(btn_cancel_icon, vec4_(0.8, 0.1, 0.1, 1.0));
    }
    
    if(show_ok) {
        self->btn_ok = WBtn_new(grid);
        o_user_set(self->btn_ok, self);
        WBtn_auto_event_set(self->btn_ok, on_btn_ok_clicked);
        WBtn_auto_mode_set(self->btn_ok, WBtn_AUTO_CLICKED);
        WGrid_child_cell_set(self->btn_ok, ivec4_(1, 1, 1, 1));
        WObj_padding_set(self->btn_ok, vec4_(0, 0, 4, 0));
        WObj_option_set(self->btn_ok, WGrid_align_KEY_H, WGrid_align_VALUE_END);
        oobj btn_ok_icon = WIcon_new(self->btn_ok, WTheme_ICON_CHECK);
        WIcon_color_set(btn_ok_icon, vec4_(0.1, 0.8, 0.1, 1.0));
    }
    
    // vfuncs
    super->super.v_update = WWindowDialog__v_update;
    super->super.v_style_apply = WWindowDialog__v_style_apply;
    
    // apply style
    WObj_style_apply(self);

    
    return self;
}

vec2 WWindowDialog__v_update(oobj obj, vec2 lt, vec2 min_size, oobj theme, a_pointer__fn pointer_fn)
{
    OObj_assert(obj, WWindow);
    WWindowDialog *self = obj;

    vec2 size = WWindow__v_update(obj, lt, min_size, theme, pointer_fn);
    
    if(self->bg_maximize) {
        struct r_rect *rect = WObj_gen_rects(self->bg);
        // m_MAX is to large...
        rect->rect = u_rect_new(0, 0, 1E8, 1E8);
    }

    //
    // pointer stuff
    //

    if(!self->bg_handles_pointer) {
        return size;
    }
    if(self->bg_maximize) {
        a_pointer_handled(0, 0);
    }
    
    struct a_pointer p = pointer_fn(0, 0);
    struct r_rect *r = WObj_gen_rects(self->bg);
    if(r && u_rect_contains(r->rect, p.pos.xy)) {
        a_pointer_handled(0, 0);
    }

    return size;
}

void WWindowDialog__v_style_apply(oobj obj)
{
    WWindow__v_style_apply(obj);
    
    OObj_assert(obj, WWindowDialog);
    WWindowDialog *self = obj;
    oobj style = WObj_style(self);
    
    WColor_color_set(self->bg, WStyle_windowdialog_bg(style));
    
    WPane_color_set(self->icon_pane, WStyle_windowdialog_icon_pane(style));
}
