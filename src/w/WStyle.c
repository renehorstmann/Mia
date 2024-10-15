#include "w/WStyle.h"
#include "o/OObj_builder.h"
#include "r/common.h"

#define O_LOG_LIB "w"
#include "o/log.h"



//
// public
//

WStyle *WStyle_init(oobj obj, oobj parent)
{
    WStyle *self = obj;
    o_clear(self, sizeof *self, 1);

    OObj_init(self, parent);
    OObj_id_set(self, WStyle_ID);
    
    
    self->btn_color = R_GRAY_X(0.5);
    self->btn_style = WBtn_DEF;
    
    self->drag_bg_color = R_GRAY_X(0.35);
    self->drag_progress_color_x = R_GRAY_X(0.5);
    self->drag_progress_color_y = R_GRAY_X(0.5);
    self->drag_arrow_color = R_GRAY_X(0.8);
    self->drag_arrow_pressed_color = R_GRAY_X(0.8);
    
    self->frame_border_color = R_GRAY_X(0.1);
    self->frame_border_size = 1;
    
    self->pane_color = R_GRAY_X(0.3);
    self->pane_style = WPane_DEFAULT;
    
    self->progress_color = R_GRAY_X(0.5);
    
    self->slider_bar_color = R_GRAY_X(0.4);
    self->slider_badge_color = R_GRAY_X(0.8);
    self->slider_badge_pressed_color = R_GRAY_X(0.4);

    self->imgpicker_badge_color = vec4_(1, 1, 1, 0.75);
    self->imgpicker_badge_pressed_color = vec4_(1, 1, 1, 1);
    
    self->text_color = R_GRAY_X(0.9);
    self->textshadow_shadow_color = vec4_(0, 0, 0, 0.5);

    self->window_header_color = R_GRAY_X(0.2);
    self->window_header_style = WPane_ROUND_TOP;
    self->window_body_color = R_GRAY_X(0.3);
    self->window_body_style = WPane_ROUND_BOTTOM;
    
    self->windowdialog_bg = vec4_(0, 0, 0, 0.5);
    self->windowdialog_icon_pane = R_GRAY_X(0.3);

    return self;
}
