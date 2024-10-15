#include "w/WNum.h"
#include "o/OObj_builder.h"
#include "o/str.h"
#include "u/rect.h"
#include "w/WSlider.h"
#include "w/WDrag.h"
#include "w/WText.h"
#include "w/WFrame.h"
#include "w/WBox.h"

#define O_LOG_LIB "w"

#include "o/log.h"

O_STATIC
int num_determine_decimal_places(float step)
{
    int decimal_places = 0;
    while (step < 1) {
        step *= 10;
        decimal_places++;
    }
    return decimal_places;
}

O_STATIC
int num_determine_num_width(float min, float max)
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
float num_num_to_progress(WNum *self, float num)
{
    float progress = (num - self->min) / (self->max - self->min);
    progress = o_clamp(progress, 0.0f, 1.0f);
    return progress;
}

O_STATIC
float num_progress_to_num(WNum *self, float progress)
{
    progress = o_clamp(progress, 0.0f, 1.0f);
    float num = progress * (self->max - self->min) + self->min;
    return num;
}

O_STATIC
void num_sliderdrag_event(oobj obj)
{
    WNum *self = o_user(obj);
    if (!self || !self->change_event) {
        return;
    }
    self->change_event(self);
}

//
// public
//

WNum *WNum_init(oobj obj, oobj parent, float min, float max, float step, const char *opt_label)
{
    WObj *super = obj;
    WNum *self = obj;
    o_clear(self, sizeof *self, 1);

    WObj_init(obj, parent);
    OObj_id_set(self, WNum_ID);

    self->min = self->num = min;
    self->max = max;
    self->step = step;


    // init objects
    self->frame = WFrame_new(self);
    if (opt_label) {
        self->frame_label = WText_new(WFrame_label(self->frame), opt_label);
    }

    oobj box = WBox_new(self->frame, WBox_LAYOUT_H_WEIGHTS);
    self->drag = WDrag_new(box);
    o_user_set(self->drag, self);
    WBox_child_weight_set(self->drag, 1.0);
    WDrag_change_event_set(self->drag, num_sliderdrag_event);
    self->text_num = WText_new(box, NULL);
    WObj_padding_set(self->text_num, vec4_(1));
    WBox_child_weight_set(self->text_num, 0);

    WObj_min_size_set(self->drag, vec2_(32, 0));

    // vfuncs
    super->v_update = WNum__v_update;

    return self;
}

//
// vfuncs
//

vec2 WNum__v_update(oobj obj, vec2 lt, vec2 min_size, oobj theme, a_pointer__fn pointer_fn)
{
    OObj_assert(obj, WNum);
    WNum *self = obj;

    assert(self->step > 0);
    int decimal_places = num_determine_decimal_places(self->step);
    int num_width = num_determine_num_width(self->min, self->max);

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

    float drag_step = self->step / (self->max - self->min);
    WDrag_steps_set(self->drag, vec2_(drag_step, -1));

    vec2 child_size = WObj__v_update(self, lt, min_size, theme, pointer_fn);

    self->num = num_progress_to_num(self, WDrag_progress(self->drag).x);
    self->num = o_clamp(self->num, self->min, self->max);

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

void WNum_colors_set(oobj obj, vec4 frame_color,
                     vec4 drag_bg_color, vec4 drag_progress_color, vec4 drag_arrow_color,
                     vec4 num_text_color)
{
    OObj_assert(obj, WNum);
    WNum *self = obj;

    WFrame_border_color_set(self->frame, frame_color);
    if(self->frame_label) {
        WText_color_set(self->frame_label, num_text_color);
    }

    WDrag_bg_color_set(self->drag, drag_bg_color);
    WDrag_progress_color_x_set(self->drag, drag_progress_color);
    WDrag_arrow_color_set(self->drag, drag_arrow_color);
    WDrag_arrow_pressed_color_set(self->drag, drag_arrow_color);

    WText_color_set(self->text_num, num_text_color);
}

float WNum_num_set(oobj obj, float set)
{
    OObj_assert(obj, WNum);
    WNum *self = obj;
    WDrag_progress_raw_set(self->drag, vec2_(num_num_to_progress(self, set), 0));
    self->num = num_progress_to_num(self, WDrag_progress(self->drag).x);
    return self->num;
}

int WNum_decimal_places(oobj obj)
{
    return num_determine_decimal_places(WNum_step(obj));
}
