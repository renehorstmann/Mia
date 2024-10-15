#include "w/WImgPicker.h"
#include "o/OObj_builder.h"
#include "w/WTheme.h"
#include "w/WStyle.h"
#include "m/vec/vec2.h"
#include "m/vec/ivec2.h"
#include "u/rect.h"

#define O_LOG_LIB "w"

#include "o/log.h"



//
// public
//

WImgPicker *WImgPicker_init(oobj obj, oobj parent, vec4 uv_rect)
{
    WImg *super = obj;
    WImgPicker *self = obj;
    o_clear(self, sizeof *self, 1);

    WImg_init(obj, parent, uv_rect);

    OObj_id_set(self, WImgPicker_ID);


    self->mode = WImgPicker_XY;
    self->progress = vec2_(0.5);
    self->on_unit = true;

    // vfuncs
    super->super.v_update = WImgPicker__v_update;
    super->super.v_style_apply = WImgPicker__v_style_apply;

    // apply style
    WObj_style_apply(self);

    return self;
}

vec2 WImgPicker__v_update(oobj obj, vec2 lt, vec2 min_size, oobj theme, a_pointer__fn pointer_fn)
{
    OObj_assert(obj, WImgPicker);
    WImgPicker *self = obj;


    struct u_atlas atlas = WTheme_atlas(theme);

    int sprite;
    if(self->mode == WImgPicker_X) {
        sprite = WTheme_ICON_PICKER_X;
    } else if(self->mode == WImgPicker_Y) {
        sprite = WTheme_ICON_PICKER_Y;
    } else {
        sprite = WTheme_ICON_PICKER_XY;
    }

    vec2 size_badge = u_atlas_size(atlas, sprite);
    vec2 badge_offset = vec2_floor(vec2_div(size_badge, 2));

    vec2 picker_field = WImg_size(self);

    // valid is [0-picker_field), so sub 1
    picker_field = vec2_sub(picker_field, 1);

    vec2 badge_lt = vec2_(lt.x - badge_offset.x + picker_field.x * self->progress.x,
                          lt.y + badge_offset.y - picker_field.y * (1.0f - self->progress.y));

    if (self->on_unit) {
        badge_lt = vec2_round(badge_lt);
    }

    // theme the badge rect
    struct r_rect *rect = WObj__alloc_rects(self, theme, 1);

    // badge
    rect->uv_rect = u_atlas_rect(atlas, sprite);
    rect->rect = u_rect_new_lt(m_2(badge_lt), m_2(size_badge));
    rect->s = self->pressed ? self->badge_pressed_color : self->badge_color;


    // WImg
    vec2 full_size = WImg__v_update(self, lt, min_size, theme, pointer_fn);


    //
    // pointer stuff
    //

    // grab pointer
    struct a_pointer pointer = pointer_fn(0, 0);

    if (!pointer.active) {
        self->pressed = false;
    }

    vec2 pp = pointer.pos.xy;
    if (lt.x <= pp.x && pp.x <= lt.x + picker_field.x
        && lt.y >= pp.y && pp.y >= lt.y - picker_field.y) {
        if (a_pointer_down(pointer, self->prev_pointer)) {
            self->pressed = true;
        }
    }

    if (self->pressed) {
        // picker_field is sub'ed by 1 [0:img_size)
        //   so we subtract half of that from the pointer pos to get the centered position
        vec2 progress = {{(pp.x - 0.5f) - lt.x,
                          lt.y - (pp.y + 0.5f)}};
        progress = vec2_div_v(progress, picker_field);
        progress = vec2_clamp(progress, 0.0f, 1.0f);

        if(self->mode == WImgPicker_X || self->mode == WImgPicker_XY) {
            self->progress.x = progress.x;
        }
        if(self->mode == WImgPicker_Y || self->mode == WImgPicker_XY) {
            self->progress.y = 1.0f - progress.y;
        }

        if (self->change_event) {
            self->change_event(self);
        }

        a_pointer_handled(0, 0);
    }

    self->prev_pointer = pointer;


    return full_size;
}

void WImgPicker__v_style_apply(oobj obj)
{
    // WImg does not have a style to be applied
    WObj__v_style_apply(obj);

    OObj_assert(obj, WImgPicker);
    WImgPicker *self = obj;
    oobj style = WObj_style(self);

    self->badge_color = WStyle_imgpicker_badge_color(style);
    self->badge_pressed_color = WStyle_imgpicker_badge_pressed_color(style);
}

ivec2 WImgPicker_progress_img(oobj obj)
{
    vec2 progress = WImgPicker_progress(obj);
    vec2 size = WImg_size(obj);
    size = vec2_sub(size, 1);
    vec2 pos = vec2_scale_v(progress, size);
    pos = vec2_round(pos);
    return ivec2_cast_float(pos.v);
}
