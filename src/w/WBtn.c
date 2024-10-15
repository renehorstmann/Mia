#include "w/WBtn.h"
#include "o/OObj_builder.h"
#include "w/WTheme.h"
#include "w/WStyle.h"
#include "m/vec/vec2.h"
#include "u/rect.h"

#define O_LOG_LIB "w"
#include "o/log.h"


bool pointer_clicked(WBtn *self)
{
    bool in_r = self->pointer_current.active
            && u_rect_contains(WObj_gen_rect(self), self->pointer_current.pos.xy);
    bool press = a_pointer_down(self->pointer_current, self->pointer_prev) && in_r;

    bool clicked = a_pointer_up(self->pointer_current, self->pointer_prev)
                   && WBtn_is_pressed(self); // was pressed before

    if(press) {
        WBtn_set(self, true);
    }
    if(!in_r || clicked) {
        WBtn_set(self, false);
    }

    self->auto_action = clicked;
    return clicked;
}


bool pointer_pressed(WBtn *self)
{
    bool press = a_pointer_down(self->pointer_current, self->pointer_prev)
                 && u_rect_contains(WObj_gen_rect(self), self->pointer_current.pos.xy);
    if(press) {
        WBtn_set(self, true);
    }

    self->auto_action = press;
    return press;
}

bool pointer_toggled(WBtn *self)
{
    bool toggle = a_pointer_up(self->pointer_current, self->pointer_prev)
                  && u_rect_contains(WObj_gen_rect(self), self->pointer_current.pos.xy);
    if(toggle) {
        WBtn_set(self, !WBtn_is_pressed(self));

    }
    self->auto_action = toggle;
    return toggle;
}

//
// public
//

WBtn *WBtn_init(oobj obj, oobj parent)
{
    WObj *super = obj;
    WBtn *self = obj;
    o_clear(self, sizeof *self, 1);

    WObj_init(obj, parent);
    OObj_id_set(self, WBtn_ID);
    
    // vfuncs
    super->v_update = WBtn__v_update;
    super->v_style_apply = WBtn__v_style_apply;
    
    // apply style
    WObj_style_apply(self);

    return self;
}

//
// vfuncs
//

vec2 WBtn__v_update(oobj obj, vec2 lt, vec2 min_size, oobj theme, a_pointer__fn pointer_fn)
{
    OObj_assert(obj, WBtn);
    WBtn *self = obj;

    struct u_atlas atlas = WTheme_atlas(theme);

    int btn_idx;
    switch(self->style) {
        default:
        case WBtn_FLAT:
            btn_idx = WTheme_BTN_FLAT;
            break;
        case WBtn_FLAT_ROUND:
            btn_idx = WTheme_BTN_FLAT_ROUND;
            break;
        case WBtn_DEF:
            btn_idx = WTheme_BTN_DEF;
            break;
        case WBtn_DEF_ROUND:
            btn_idx = WTheme_BTN_DEF_ROUND;
            break;
        case WBtn_BIG:
            btn_idx = WTheme_BTN_BIG;
            break;
        case WBtn_BIG_ROUND:
            btn_idx = WTheme_BTN_BIG_ROUND;
            break;
        case WBtn_RADIO:
            btn_idx = WTheme_BTN_RADIO;
            break;
        case WBtn_CHECK:
            btn_idx = WTheme_BTN_CHECK;
            break;
        case WBtn_SWITCH:
            btn_idx = WTheme_BTN_SWITCH;
            break;
    }

    int sprite = btn_idx;
    if(self->mode == 1) {
        sprite += WTheme_BTN__SPRITES;
    }

    vec2 size_corner_lt = u_atlas_size(atlas, sprite);
    vec2 size_corner_rb = u_atlas_size(atlas, sprite + 8);
    vec2 frame_size = vec2_add_v(size_corner_lt, size_corner_rb);


    vec2 children_lt = vec2_(lt.x + size_corner_lt.x, lt.y - size_corner_lt.y);
    vec2 min_inner_size = vec2_sub_v(min_size, frame_size);

    vec2 inner_size = WObj__update_children_stacked(self, children_lt, min_inner_size, theme, pointer_fn);

    inner_size = vec2_max_v(inner_size, min_inner_size);
    inner_size = vec2_max(inner_size, 0);

    // theme the rects
    struct r_rect *rects = WObj__alloc_rects(self, theme, 9);

    vec2 offsets[9] = {
            {{0, 0}},
            {{size_corner_lt.x, 0}},
            {{size_corner_lt.x + inner_size.x, 0}},
            {{0, size_corner_lt.y}},
            {{size_corner_lt.x, size_corner_lt.y}},
            {{size_corner_lt.x + inner_size.x, size_corner_lt.y}},
            {{0, size_corner_lt.y + inner_size.y}},
            {{size_corner_lt.x, size_corner_lt.y + inner_size.y}},
            {{size_corner_lt.x + inner_size.x, size_corner_lt.y + inner_size.y}},
    };
    vec2 sizes[9] = {
            {{size_corner_lt.x, size_corner_lt.y}},
            {{inner_size.x, size_corner_lt.y}},
            {{size_corner_rb.x, size_corner_lt.y}},
            {{size_corner_lt.x, inner_size.y}},
            {{inner_size.x, inner_size.y}},
            {{size_corner_rb.x, inner_size.y}},
            {{size_corner_lt.x, size_corner_rb.y}},
            {{inner_size.x, size_corner_rb.y}},
            {{size_corner_rb.x, size_corner_rb.y}}
    };

    for (int i = 0; i < 9; i++) {
        struct r_rect *q = &rects[i];
        q->uv_rect = u_atlas_rect(atlas, sprite + i);
        q->rect = u_rect_new_lt(lt.x + offsets[i].x,
                                lt.y - offsets[i].y,
                                m_2(sizes[i]));
        q->s = self->color;
    }

    vec2 full_size = vec2_add_v(inner_size, frame_size);

    //
    // pointer stuff
    //

    // grab pointers
    self->pointer_prev = self->pointer_current;
    self->pointer_current = pointer_fn(0, 0);

    vec2 pp = self->pointer_current.pos.xy;
    if(lt.x <= pp.x && pp.x <= lt.x+full_size.x
       && lt.y >= pp.y && pp.y >= lt.y-full_size.y) {
        a_pointer_handled(0, 0);
    }

    self->auto_action = false;

    switch(self->auto_mode) {
        default:
        case WBtn_AUTO_OFF:
            break;
        case WBtn_AUTO_CLICKED:
            if(pointer_clicked(self) && self->auto_event) {
                self->auto_event(self);
            }
            break;
        case WBtn_AUTO_PRESSED:
            if(pointer_pressed(self) && self->auto_event) {
                self->auto_event(self);
            }
            break;
        case WBtn_AUTO_TOGGLED:
            if(pointer_toggled(self) && self->auto_event) {
                self->auto_event(self);
            }
            break;
    }

    return full_size;
}

void WBtn__v_style_apply(oobj obj) 
{
    WObj__v_style_apply(obj);
    
    OObj_assert(obj, WBtn);
    WBtn *self = obj;
    oobj style = WObj_style(self);
    
    self->color = WStyle_btn_color(style);
    self->style = WStyle_btn_style(style);    
}



bool WBtn_clicked(oobj obj)
{
    OObj_assert(obj, WBtn);
    WBtn *self = obj;
    if(self->auto_mode == WBtn_AUTO_OFF) {
        pointer_clicked(self);
    }
    return self->auto_action;
}

bool WBtn_pressed(oobj obj)
{
    OObj_assert(obj, WBtn);
    WBtn *self = obj;
    if(self->auto_mode == WBtn_AUTO_OFF) {
        pointer_pressed(self);
    }
    return self->auto_action;
}

bool WBtn_toggled(oobj obj)
{
    OObj_assert(obj, WBtn);
    WBtn *self = obj;
    if(self->auto_mode == WBtn_AUTO_OFF) {
        pointer_toggled(self);
    }
    return self->auto_action;
}
