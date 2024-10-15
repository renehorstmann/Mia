#include "w/WSlider.h"
#include "o/OObj_builder.h"
#include "w/WTheme.h"
#include "w/WStyle.h"
#include "m/vec/vec2.h"
#include "u/rect.h"

#define O_LOG_LIB "w"
#include "o/log.h"

//
// public
//

WSlider *WSlider_init(oobj obj, oobj parent)
{
    WObj *super = obj;
    WSlider *self = obj;
    o_clear(self, sizeof *self, 1);

    WObj_init(obj, parent);
    OObj_id_set(self, WSlider_ID);

    self->vertical = false;
    self->on_unit = true;

    // vfuncs
    super->v_update = WSlider__v_update;
    super->v_style_apply = WSlider__v_style_apply;
    
    // apply style
    WObj_style_apply(self);

    return self;
}

//
// vfuncs
//

vec2 WSlider__v_update(oobj obj, vec2 lt, vec2 min_size, oobj theme, a_pointer__fn pointer_fn)
{
    OObj_assert(obj, WSlider);
    WSlider *self = obj;


    struct u_atlas atlas = WTheme_atlas(theme);

    int sprite = self->vertical? WTheme_SLIDER_V_BAR : WTheme_SLIDER_H_BAR;

    vec2 size_badge = u_atlas_size(atlas, sprite+3);
    

    vec2 slider_field = min_size;
    vec2 progress_length = {{0}};

    if(self->vertical) {
        slider_field = vec2_max_v(slider_field, vec2_(size_badge.x, 16));
        progress_length.y = slider_field.y - size_badge.y;
    } else {
        slider_field = vec2_max_v(slider_field, vec2_(16, size_badge.y));
        progress_length.x = slider_field.x - size_badge.x;
    }
    
    vec2 badge_lt = vec2_(lt.x + progress_length.x * self->progress,
                          lt.y - progress_length.y * (1.0f - self->progress));
    
    if(self->on_unit) {
        badge_lt = vec2_round(badge_lt);
    }

    // right next to the slider
    vec2 children_lt = vec2_(lt.x + slider_field.x, lt.y);
    vec2 children_size = WObj__update_children_stacked(self, children_lt, vec2_(0), theme, pointer_fn);

    // theme the badge rects
    struct r_rect *rects = WObj__alloc_rects(self, theme, 3+1);

    struct r_rect *q;
    vec2 sprite_size;
    vec2 hs = {{self->vertical? 0 : 1, self->vertical? 1 : 0}};
    vec2 sprite_lt = lt;

    q = &rects[0];
    q->uv_rect = u_atlas_rect(atlas, sprite + 0);
    sprite_size = u_atlas_size(atlas, sprite + 0);
    q->rect = u_rect_new_lt(m_2(sprite_lt), m_2(sprite_size));
    q->s = self->bar_color;
    sprite_lt = vec2_(sprite_lt.x + sprite_size.x * hs.x,
                      sprite_lt.y - sprite_size.y * hs.y);

    q = &rects[1];
    q->uv_rect = u_atlas_rect(atlas, sprite + 1);
    if(self->vertical) {
        sprite_size.y = slider_field.y - sprite_size.y - u_atlas_size(atlas, sprite + 2).y;
        sprite_size.y = o_max(sprite_size.y, 0);
    } else {
        sprite_size.x = slider_field.x - sprite_size.x - u_atlas_size(atlas, sprite + 2).x;
        sprite_size.x = o_max(sprite_size.x, 0);
    }
    q->rect = u_rect_new_lt(m_2(sprite_lt), m_2(sprite_size));
    q->s = self->bar_color;
    sprite_lt = vec2_(sprite_lt.x + sprite_size.x * hs.x,
                      sprite_lt.y - sprite_size.y * hs.y);

    q = &rects[2];
    q->uv_rect = u_atlas_rect(atlas, sprite + 2);
    sprite_size = u_atlas_size(atlas, sprite + 2);
    q->rect = u_rect_new_lt(m_2(sprite_lt), m_2(sprite_size));
    q->s = self->bar_color;

    // badge
    q = &rects[3];
    q->uv_rect = u_atlas_rect(atlas, sprite + 3);
    sprite_size = u_atlas_size(atlas, sprite + 3);
    q->rect = u_rect_new_lt(m_2(badge_lt), m_2(sprite_size));
    q->s = self->pressed? self->badge_pressed_color : self->badge_color;

    vec2 full_size = vec2_(slider_field.x + children_size.x,
                           o_max(slider_field.y, children_size.y));


    //
    // pointer stuff
    //

    // grab pointer
    struct a_pointer pointer = pointer_fn(0, 0);

    if(!pointer.active) {
        self->pressed = false;
    }

    vec2 pp = pointer.pos.xy;
    if(lt.x <= pp.x && pp.x <= lt.x+slider_field.x
       && lt.y >= pp.y && pp.y >= lt.y-slider_field.y) {
        if(a_pointer_down(pointer, self->prev_pointer)) {
            self->pressed = true;
        }
    }

    if(self->pressed) {
        vec2 progress = {{pp.x - lt.x - size_badge.x / 2.0f,
                          lt.y - pp.y - size_badge.y / 2.0f}};
        progress = vec2_div_v(progress, progress_length);
        progress = vec2_clamp(progress, 0.0f, 1.0f);
        self->progress = self->vertical? 1.0f-progress.y : progress.x;

        if(self->change_event) {
            self->change_event(self);
        }

        a_pointer_handled(0, 0);
    }
    
    self->prev_pointer = pointer;


    return full_size;
}

void WSlider__v_style_apply(oobj obj) 
{
    WObj__v_style_apply(obj);
    
    OObj_assert(obj, WSlider);
    WSlider *self = obj;
    oobj style = WObj_style(self);
    
    self->bar_color = WStyle_slider_bar_color(style);
    self->badge_color = WStyle_slider_badge_color(style);
    self->badge_pressed_color = WStyle_slider_badge_pressed_color(style);
}

