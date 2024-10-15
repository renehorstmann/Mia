#include "w/WGradient.h"
#include "o/OObj_builder.h"
#include "o/str.h"
#include "u/rect.h"
#include "w/WImgPicker.h"
#include "w/WText.h"
#include "w/WFrame.h"
#include "w/WBox.h"

#define O_LOG_LIB "w"

#include "o/log.h"

O_STATIC
int determine_decimal_places(float step)
{
    int decimal_places = 0;
    while (step < 1) {
        step *= 10;
        decimal_places++;
    }
    return decimal_places;
}

O_STATIC
int determine_num_width(float min, float max)
{
    min = o_abs(min);
    max = o_abs(max);
    max = o_max(min, max);
    int w = 0;
    while (max >= 1.0f) {
        max /= 10.0f;
        w++;
    }
    return w;
}


O_STATIC
float num_to_progress(WGradient *self, float num)
{
    float progress = (num - self->min) / (self->max - self->min);
    progress = o_clamp(progress, 0.0f, 1.0f);
    return progress;
}

O_STATIC
float progress_to_num(WGradient *self, float progress)
{
    progress = o_clamp(progress, 0.0f, 1.0f);
    float num = progress * (self->max - self->min) + self->min;
    return num;
}

O_STATIC
void imgpicker_event(oobj obj)
{
    WGradient *self = o_user(obj);
    if (!self || !self->change_event) {
        return;
    }
    self->change_event(self);
}

//
// public
//

WGradient *WGradient_init(oobj obj, oobj parent, vec4 uv_rect, float min, float max, float step, const char *opt_label)
{
    WObj *super = obj;
    WGradient *self = obj;
    o_clear(self, sizeof *self, 1);

    WObj_init(obj, parent);
    OObj_id_set(self, WGradient_ID);

    self->min = self->num = min;
    self->max = max;
    self->step = step;


    // init objects
    self->frame = WFrame_new(self);
    if (opt_label) {
        self->frame_label = WText_new(WFrame_label(self->frame), opt_label);
    }

    oobj box = WBox_new(self->frame, WBox_LAYOUT_H);
    self->imgpicker = WImgPicker_new(box, uv_rect);
    o_user_set(self->imgpicker, self);
    WImgPicker_mode_set(self->imgpicker, WImgPicker_X);
    WImgPicker_change_event_set(self->imgpicker, imgpicker_event);
    self->text_num = WText_new(box, NULL);
    WObj_padding_set(self->text_num, vec4_(1));

    WObj_fixed_size_set(self->imgpicker, vec2_(64, 7));

    // vfuncs
    super->v_update = WGradient__v_update;

    return self;
}

//
// vfuncs
//

vec2 WGradient__v_update(oobj obj, vec2 lt, vec2 min_size, oobj theme, a_pointer__fn pointer_fn)
{
    OObj_assert(obj, WGradient);
    WGradient *self = obj;

    assert(self->step > 0);
    int decimal_places = determine_decimal_places(self->step);
    int num_width = determine_num_width(self->min, self->max);

    const char *format;
    if(self->min >= 0 && self->max >= 0) {
        format = "%*.*f";
    } else {
        format = "%+*.*f";
    }
    char buf[32];
    // +2 because of the dot and the - sign
    o_strf_buf(buf, format, num_width + decimal_places + 2, decimal_places, self->num);

    WText_text_set(self->text_num, buf);

    vec2 child_size = WObj__v_update(self, lt, min_size, theme, pointer_fn);

    WGradient_num_set(self, progress_to_num(self, WImgPicker_progress(self->imgpicker).x));

    //
    // pointer stuff
    //

    if (self->num_pressed_event) {
        struct a_pointer pointer = pointer_fn(0, 0);
        if (a_pointer_down(pointer, self->pointer_prev)
            && u_rect_contains(WObj_gen_rect(self->text_num), pointer.pos.xy)) {

            // call event
            self->num_pressed_event(self);
        }

        self->pointer_prev = pointer;
    }


    return child_size;
}

void WGradient_colors_set(oobj obj, vec4 frame_color, 
        vec4 badge_color, vec4 badge_pressed_color,
        vec4 num_text_color)
{
    OObj_assert(obj, WGradient);
    WGradient *self = obj;

    WFrame_border_color_set(self->frame, frame_color);
    if(self->frame_label) {
        WText_color_set(self->frame_label, num_text_color);
    }
    
    WImgPicker_badge_color_set(self->imgpicker, badge_color);
    WImgPicker_badge_pressed_color_set(self->imgpicker, badge_pressed_color);

    WText_color_set(self->text_num, num_text_color);
}

float WGradient_progress(oobj obj)
{
    OObj_assert(obj, WGradient);
    WGradient *self = obj;
    return WImgPicker_progress(self->imgpicker).x;
}

float WGradient_progress_set(oobj obj, float set)
{
    OObj_assert(obj, WGradient);
    WGradient *self = obj;
    WGradient_num_set(self, progress_to_num(self, set));
    return WGradient_progress(self);
}

float WGradient_num_set(oobj obj, float set)
{
    OObj_assert(obj, WGradient);
    WGradient *self = obj;
    // apply step
    set -= self->min;
    set = m_floor(set / self->step) * self->step;
    set += self->min;
    set = m_clamp(set, self->min, self->max);
    WImgPicker_progress_set(self->imgpicker, vec2_(num_to_progress(self, set), 0.5));
    self->num = set;
    return self->num;
}

int WGradient_decimal_places(oobj obj)
{
    return determine_decimal_places(WGradient_step(obj));
}
