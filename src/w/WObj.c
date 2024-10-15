#include "w/WObj.h"
#include "o/OObj_builder.h"
#include "o/OMap.h"
#include "o/str.h"
#include "m/vec/vec2.h"
#include "u/pose.h"
#include "u/rect.h"
#include "w/WTheme.h"
#include "w/WStyle.h"

#define O_LOG_LIB "w"
#include "o/log.h"

//
// public
//

WObj *WObj_init(oobj obj, oobj parent)
{
    WObj *self = obj;
    o_clear(self, sizeof *self, 1);

    OObj_init(self, parent);
    OObj_id_set(self, WObj_ID);
    
    self->fixed_size = vec2_(-1);
    
    self->option_map = OMap_new_string_keys(self, sizeof (const char *), 64);
    
    // style
    if(OObj_check(parent, WObj)) {
        self->style = WObj_style(parent);
    } else if(OObj_check(parent, WStyle)) {
        self->style = parent;
    } else {
        self->style = WStyle_new(self);
    }

    // vfuncs
    self->v_update = WObj__v_update;
    self->v_list = WObj__v_list;
    self->v_style_apply = WObj__v_style_apply;

    return self;
}

//
// vfuncs
//

vec2 WObj__v_update(oobj obj, vec2 lt, vec2 min_size, oobj theme, a_pointer__fn pointer_fn)
{
    OObj_assert(obj, WObj);
    WObj *self = obj;
    vec2 child_size = WObj__update_children_stacked(obj, lt, min_size, theme, pointer_fn);
    return child_size;
}

oobj *WObj__v_list(oobj obj, osize *opt_out_num)
{
    return (oobj *) WObj_list_direct(obj, opt_out_num);
}

void WObj__v_style_apply(oobj obj)
{
   // noop 
}

//
// object functions
//

const char *WObj_option(oobj obj, const char *key)
{
    oobj map = WObj_option_map(obj);
    const char **res = OMap_get(map, &key, const char *);
    return res? *res : NULL;
}

void WObj_option_set(oobj obj, const char *key, const char *value) {
    oobj map = WObj_option_map(obj);
    char **old_ptr = OMap_get(map, &key, char *);
    if(old_ptr) {
        o_free(map, *old_ptr);
    }
    char *clone = o_str_clone(map, value);
    OMap_set(map, &key, &clone);
}

struct r_rect *WObj_gen_rects(oobj obj)
{
    OObj_assert(obj, WObj);
    WObj *self = obj;
    if(self->gen.rects_num <= 0) {
        return NULL;
    }
    return WTheme_at(self->gen.theme, self->gen.rects_theme_back_idx);
}


void WObj_gen_rects_color_set(oobj obj, vec4 color)
{
    struct r_rect *rects = WObj_gen_rects(obj);
    int rects_num = WObj_gen_rects_num(obj);
    for (int i = 0; i < rects_num; i++) {
        rects[i].s = color;
    }
}

vec4 WObj_gen_rect(oobj obj)
{
    OObj_assert(obj, WObj);
    WObj *self = obj;
    if(self->hide) {
        return u_rect_new_hidden();
    }
    return vec4_(self->gen.lt.x + self->gen.size.x/2,
                 self->gen.lt.y - self->gen.size.y/2,
                 m_2(self->gen.size));
}

mat4 WObj_gen_pose(oobj obj)
{
    OObj_assert(obj, WObj);
    WObj *self = obj;
    if(self->hide) {
        return u_pose_new_hidden();
    }
    return u_pose_new(self->gen.lt.x + self->gen.size.x / 2,
                      self->gen.lt.y - self->gen.size.y / 2,
                      m_2(self->gen.size));
}

WObj *WObj_focusable_find(oobj obj)
{
    OObj_assert(obj, WObj);
    WObj *self = obj;
    if (WObj_focusable(self)) {
        return self;
    }

    WObj *child = NULL;
    osize list_num;
    WObj **list = WObj_list(self, &list_num);
    for(osize i=list_num-1; i>=0; i--) {
        child = WObj_focusable_find(list[i]);
        if (child) {
            break;
        }
    }
    o_free(self, list);
    return child;
}

WObj *WObj_focusable_find_last(oobj obj)
{

    OObj_assert(obj, WObj);
    WObj *self = obj;
    WObj *find = NULL;
    if (WObj_focusable(self)) {
        find = self;
    }

    WObj *child = NULL;
    osize list_num;
    WObj **list = WObj_list(self, &list_num);
    for(osize i=list_num-1; i>=0; i--) {
        child = WObj_focusable_find_last(list[i]);
        if (child) {
            find = child;
        }
    }
    o_free(self, list);
    return find;
}

WObj *WObj_focus_find(oobj obj)
{
    OObj_assert(obj, WObj);
    WObj *self = obj;
    if (WObj_focusable(self) && self->focus) {
        return self;
    }

    WObj *child = NULL;
    osize list_num;
    WObj **list = WObj_list(self, &list_num);
    for(osize i=list_num-1; i>=0; i--) {
        child = WObj_focus_find(list[i]);
        if (child) {
            break;
        }
    }
    o_free(self, list);
    return child;
}

O_STATIC
void focus_clear_r(WObj *self)
{
    self->focus = false;
    osize list_num;
    WObj **list = WObj_list(self, &list_num);
    for(osize i=list_num-1; i>=0; i--) {
        focus_clear_r(list[i]);
    }
    o_free(self, list);
}

O_STATIC
void focus_next_r(WObj *self, int *state)
{
    if (*state == 2) {
        // already done
        return;
    }

    if (WObj_focusable(self)) {
        if (*state == 0 && self->focus) {
            self->focus = false;
            *state = 1; // next to focus
        } else if (*state == 1) {
            self->focus = true;
            *state = 2; // done
        }
    }

    osize list_num;
    WObj **list = WObj_list(self, &list_num);
    for(osize i=list_num-1; i>=0; i--) {
        focus_next_r(list[i], state);
    }
    o_free(self, list);
}

O_STATIC
void focus_prev_r(WObj *self, int *state, WObj **prev)
{
    if (*state != 0) {
        // already done
        return;
    }

    if (WObj_focusable(self)) {
        if (self->focus) {
            self->focus = false;
            if (*prev) {
                (*prev)->focus = true;
                *state = 2; // done
            } else {
                *state = 1; // prev not found, needs to be last possible
            }
        }
        *prev = self;
    }

    osize list_num;
    WObj **list = WObj_list(self, &list_num);
    for(osize i=list_num-1; i>=0; i--) {
        focus_prev_r(list[i], state, prev);
    }
    o_free(self, list);
}

void WObj_focus_navigate(oobj obj, enum WObj_focus_navigation navigate)
{
    OObj_assert(obj, WObj);
    WObj *self = obj;
    int state = 0;
    WObj *prev = NULL;
    switch (navigate) {
        default:
        case WObj_focus_CLEAR:
            focus_clear_r(self);
            break;
        case WObj_focus_NEXT:
            focus_next_r(self, &state);
            if (state != 2) {
                WObj *first = WObj_focusable_find(self);
                if (first) {
                    first->focus = true;
                }
            }
            break;
        case WObj_focus_PREV:
            focus_prev_r(self, &state, &prev);
            if (state != 2) {
                WObj *last = WObj_focusable_find_last(self);
                if (last) {
                    last->focus = true;
                }
            }
            break;
    }
}

bool WObj_focus_trigger_event(oobj obj)
{
    WObj *find = WObj_focus_find(obj);
    if (find) {
        OObj_assert(find, WObj);
        WObj *find_self = find;
        find_self->v_opt_focus_trigger_event(find_self);
    }
    return find != NULL;
}

struct r_rect *WObj__alloc_rects(oobj obj, oobj theme, int num)
{
    OObj_assert(obj, WObj);
    WObj *self = obj;
    if (self->ignore_rects_alloc) {
        assert(self->gen.rects_num == num);
    } else {
        int back_idx = WTheme_alloc(theme, num);
        self->gen.rects_theme_back_idx = back_idx;
        self->gen.rects_num = num;
    }
    return WObj_gen_rects(self);
}

void WObj__ignore_alloc_rects_set(oobj obj, bool set)
{
    OObj_assert(obj, WObj);
    WObj *self = obj;
    self->ignore_rects_alloc = set;
    osize list_num;
    WObj **list = WObj_list_direct(self, &list_num);
    // no need for backwards, but to keep it consistent...
    for(osize i=list_num-1; i>=0; i--) {
        WObj__ignore_alloc_rects_set(list[i], set);
    }
    o_free(self, list);
}


vec2 WObj__update_list_stacked(WObj **list, osize list_num, vec2 lt, vec2 min_size, oobj theme, a_pointer__fn pointer_fn)
{
    vec2 child_size = vec2_(0);
    for(osize i=list_num-1; i>=0; i--) {
        vec2 s = WObj_update(list[i], lt, min_size, theme, pointer_fn);
        child_size = vec2_max_v(child_size, s);
    }
    return child_size;
}

vec2 WObj__update_children_stacked(oobj obj, vec2 lt, vec2 min_size, oobj theme, a_pointer__fn pointer_fn)
{
    OObj_assert(obj, WObj);
    WObj *self = obj;

    osize list_num;
    WObj **list = WObj_list_direct(self, &list_num);
    vec2 child_size = WObj__update_list_stacked(list, list_num, lt, min_size, theme, pointer_fn);
    o_free(self, list);

    return child_size;
}

vec2 WObj_update(oobj obj, vec2 lt, vec2 min_size, oobj theme, a_pointer__fn pointer_fn)
{
    OObj_assert(obj, WObj);
    WObj *self = obj;
    
    if(self->hide) {
        self->gen.lt = self->gen.padding_lt = lt;
        return self->gen.size = self->gen.padding_size = vec2_(0);
    }

    // if available, override
    self->gen.theme = o_or(self->theme, theme);

    assert(pointer_fn && "most of the cases >a_pointer< can be passed");
    pointer_fn = o_or(self->pointer_fn, pointer_fn);

    vec2 padded_lt = vec2_(lt.v0 + self->padding.v0, lt.v1 - self->padding.v1);
    vec2 size_diff = vec2_(self->padding.v0 + self->padding.v2, self->padding.v1 + self->padding.v3);
    min_size = vec2_sub_v(min_size, size_diff);
    min_size = vec2_max_v(min_size, self->min_size);
    vec2 used_size = self->v_update(self, padded_lt, min_size, self->gen.theme, pointer_fn);

    vec2 size = vec2_max_v(used_size, self->min_size);
    if(self->fixed_size.x>=0) {
        size.x = self->fixed_size.x;
    }
    if(self->fixed_size.y>=0) {
        size.y = self->fixed_size.y;
    }
    
    vec2 padding_size = vec2_add_v(size, size_diff);

    // set its generated values
    self->gen.lt = padded_lt;
    self->gen.used_size = used_size;
    self->gen.size = size;
    self->gen.padding_lt = lt;
    self->gen.padding_size = padding_size;

    // optional post update event
    if(self->v_opt_update_event) {
        self->v_opt_update_event(self);
    }

    // has focus active event
    if(self->focus && WObj_focusable(self) && self->v_opt_focus_active_event) {
        self->v_opt_focus_active_event(self);
    }

    return padding_size;
}
