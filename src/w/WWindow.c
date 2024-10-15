#include "w/WWindow.h"
#include "o/OObj_builder.h"
#include "m/vec/vec2.h"
#include "u/rect.h"
#include "u/btn.h"
#include "w/WTheme.h"
#include "w/WStyle.h"
#include "w/WPane.h"
#include "w/WAlign.h"
#include "w/WText.h"
#include "w/WTextShadow.h"
#include "o/timer.h"

#define O_LOG_LIB "w"

#include "o/log.h"


static const vec4 HIDE_BTN_COLOR = {{0.66, 0.66, 0.66, 0.66}};

static const float DBL_TAP_HIDE_TIME = 0.33;
static const float HIDE_BLOCK_TIME = 0.33;

//
// public
//

WWindow *WWindow_init(oobj obj, oobj parent, vec2 header_min_size)
{
    WObj *super = obj;
    WWindow *self = obj;
    o_clear(self, sizeof *self, 1);

    WObj_init(obj, parent);
    OObj_id_set(self, WWindow_ID);

    self->lt_on_unit = true;
    self->hideable = true;
    self->draggable = true;
    self->header_min_size = header_min_size;
    self->body_offset = vec2_(0, 1);

    self->container = OObj_new(self);
    self->header = WPane_new(self->container);
    self->body = WPane_new(self->container);

    self->header_align = WAlign_new(self->header);
    WAlign_align_v_set(self->header_align, WAlign_CENTER);
    WObj_fixed_size_set(self->header_align, vec2_(0));

    self->btn_hide = u_btn_new_rect(NULL, NULL, WTheme_ICON_WND_MIN, WTheme_ICON_WND_MAX);
    self->btn_hide.state_unpressed.s = self->btn_hide.state_pressed.s = HIDE_BTN_COLOR;

    // vfuncs
    super->v_update = WWindow__v_update;
    super->v_list = WWindow__v_list;
    super->v_style_apply = WWindow__v_style_apply;
    
    // apply style
    WObj_style_apply(self);

    return self;
}

WWindow *WWindow_new_title(oobj parent, vec2 header_min_size,
        const char *title, oobj *opt_out_title)
{
    oobj wnd = WWindow_new(parent, header_min_size);
    oobj txt = WTextShadow_new(WWindow_header_align(wnd), title);

    o_opt_set(opt_out_title, txt);
    return wnd;
}

//
// vfuncs
//


vec2 WWindow__v_update(oobj obj, vec2 lt, vec2 min_size, oobj theme, a_pointer__fn pointer_fn)
{
    OObj_assert(obj, WWindow);
    WWindow *self = obj;

    vec2 field_size = vec2_max_v(self->children_size_prev, min_size);

    //
    // clamp lt
    //

    self->lt = vec2_max(self->lt, 0);

    if (self->hidden) {
        self->lt.x = o_min(self->lt.x, field_size.x
                                       - WObj_gen_size(self->header).x);
        self->lt.y = o_min(self->lt.y, field_size.y
                                       - WObj_gen_size(self->header).y);
    } else {
        self->lt.x = o_min(self->lt.x, field_size.x
                                       - WObj_gen_size(self->body).x
                                       + self->body_offset.x);
        self->lt.y = o_min(self->lt.y, field_size.y
                                       - WObj_gen_size(self->body).y
                                       - self->header_min_size.y
                                       + self->body_offset.y);
    }

    vec2 drag_lt = self->lt;
    if (self->lt_on_unit) {
        drag_lt = vec2_floor(drag_lt);
    }

    //
    // update
    //

    // allocated before udpates, so that it appears on top
    WObj__alloc_rects(self, theme, 1);

    vec2 header_min_size = self->header_min_size;
    vec2 header_lt = vec2_(lt.x + drag_lt.x, lt.y - drag_lt.y);
    vec2 body_lt = vec2_(header_lt.x + self->body_offset.x,
                         header_lt.y - header_min_size.y + self->body_offset.y);

    WObj_min_size_set(self->body, vec2_(header_min_size.x, 0));
    vec2 body_size = vec2_(0);
    if (!self->hidden) {
        body_size = WObj_update(self->body, body_lt, vec2_(0), theme, pointer_fn);
    }

    WObj_min_size_set(self->header_align, WPane_gen_inner_size(self->header));
    if (!self->hidden) {
        header_min_size.x = o_max(header_min_size.x, body_size.x);
    }
    WObj_min_size_set(self->header, header_min_size);
    vec2 header_size = WObj_update(self->header, header_lt, vec2_(0), theme, pointer_fn);

    vec2 rb = vec2_(o_max(header_lt.x + header_size.x, body_lt.x + body_size.x),
                    o_min(header_lt.y - header_size.y, body_lt.y - body_size.y));
    vec2 size = vec2_(rb.x - lt.x, lt.y - rb.y);

    //
    // hide btn
    //



    // may become invalid in pointer event!
    self->btn_hide.opt_rect_ref = WObj_gen_rects(self);
    self->btn_hide.opt_atlas_ref = WTheme_atlas_ref(theme);

    vec2 icon_size = u_atlas_size(WTheme_atlas(theme), self->btn_hide.state_unpressed.atlas);
    vec2 pos = {{rb.x - 2.0f,
                 header_lt.y - header_size.y / 2.0f + icon_size.y / 2.0f + 1.0f}};
    pos = vec2_floor(pos);
    self->btn_hide.opt_rect_ref->rect = u_rect_new_rt(m_2(pos), m_2(icon_size));
    u_btn_set(&self->btn_hide, self->hidden);

    if(!self->hideable) {
        self->btn_hide.opt_rect_ref->rect.a = 0;
    }


    
    //
    // render children behind the actual window and grab size
    //

    self->children_size_prev = WObj__update_children_stacked(self, lt, min_size, theme, pointer_fn);

    vec2 full_size = vec2_max_v(size, field_size);

    //
    // pointer stuff
    //
    
    // may become invalid in contrast from update
    self->btn_hide.opt_rect_ref = WObj_gen_rects(self);

    // while dragging, pointer should be handled, so grabbing before
    struct a_pointer pointer = pointer_fn(0, 0);
    if (self->dragging) {
        a_pointer_handled(0, 0);
    }

    //
    // pointer
    //

    vec2 pp = pointer.pos.xy;

    bool hide_block = o_timer_elapsed_s(self->hide_block_timer) <= HIDE_BLOCK_TIME;
    bool toggled = false;
    
    hide_block |= !self->hideable;
   
    if (!hide_block) {
        if (u_btn_toggled(&self->btn_hide, pointer, self->pointer_prev)) {
            self->hidden = u_btn_is_pressed(&self->btn_hide);
            self->hide_block_timer = o_timer();
            toggled = true;
        }
    }

    if (!toggled && a_pointer_down(pointer, self->pointer_prev)
        && header_lt.x <= pp.x && pp.x <= header_lt.x + header_size.x
        && header_lt.y >= pp.y && pp.y >= header_lt.y - header_size.y) {

        if (!self->dragging) {
            self->dragging = true;
            self->dragging_pos = pp;
            self->dragging_lt = self->lt;
        }

        if (!hide_block && pp.x < u_rect_get_left(self->btn_hide.opt_rect_ref->rect)) {
            if (o_timer_elapsed_s(self->dbl_tap_hide_timer) <= DBL_TAP_HIDE_TIME) {
                self->hidden = !self->hidden;
                self->dbl_tap_hide_timer = 0;
                self->hide_block_timer = o_timer();
            } else {
                self->dbl_tap_hide_timer = o_timer();
            }
        }
    }


    if (!pointer.active) {
        self->dragging = false;
    }

    self->dragging &= self->draggable;
    if (self->dragging) {
        vec2 diff = vec2_sub_v(pp, self->dragging_pos);
        self->lt = vec2_(self->dragging_lt.x + diff.x, self->dragging_lt.y - diff.y);
    }

    self->pointer_prev = pointer;

    //
    // pointer handled if in window frame
    //

    if (header_lt.x <= pp.x && pp.x <= rb.x
        && header_lt.y >= pp.y && pp.y >= rb.y) {
        a_pointer_handled(0, 0);
    }

    return full_size;
}

oobj *WWindow__v_list(oobj obj, osize *opt_out_num)
{
    OObj_assert(obj, WWindow);
    WWindow *self = obj;

    osize direct_size;
    WObj **direct = WObj_list_direct(self, &direct_size);

    // directs + label + NULL
    direct = o_realloc(self, direct, sizeof(WObj *), direct_size + 3);
    direct[direct_size] = self->header;
    direct[direct_size + 1] = self->body;
    direct[direct_size + 2] = NULL;
    o_opt_set(opt_out_num, direct_size + 2);
    return (oobj *) direct;
}

void WWindow__v_style_apply(oobj obj)
{
    WObj__v_style_apply(obj);
    
    OObj_assert(obj, WWindow);
    WWindow *self = obj;
    oobj style = WObj_style(self);
    
    WPane_color_set(self->header, WStyle_window_header_color(style));
    WPane_style_set(self->header, WStyle_window_header_style(style));
    WPane_color_set(self->body, WStyle_window_body_color(style));
    WPane_style_set(self->body, WStyle_window_body_style(style));
}
