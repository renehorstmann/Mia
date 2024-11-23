#include "u/USplit.h"
#include "o/OObj_builder.h"
#include "a/app.h"
#include "a/AScene.h"
#include "a/AView.h"
#include "r/RCam.h"
#include "r/RTex.h"
#include "o/OArray.h"

#define O_LOG_LIB "u"
#include "o/log.h"


O_INLINE
float even(USplit *self, float v)
{
    if(self->even_sizes) {
        return m_floor(v/2.0f) * 2.0f;
    }
    return v;
}


O_STATIC
void update(oobj child, oobj tex, ivec4 viewport)
{
    if (OObj_check(child, USplit)) {
        USplit_update_ex(child, tex, viewport);
    } else if (OObj_check(child, AView)) {
        AView_update(child, tex, viewport);
    } else {
        o_log_warn_s("USplit_update", "child invalid, should have been USplit or AView");
    }
}

O_STATIC
void split_render(oobj child, oobj tex)
{
    if (OObj_check(child, USplit)) {
        USplit_render(child, tex);
    } else if (OObj_check(child, AView)) {
        AView_render(child, tex);
    } else {
        o_log_warn_s("USplit_render", "child invalid, should have been USplit or AView");
    }
}


//
// public
//


USplit *USplit_init(oobj obj, oobj parent, oobj child_a, oobj child_b,
                    enum USplit_mode mode, float seperator, float spacing)
{
    USplit *self = obj;
    o_clear(self, sizeof *self, 1);

    OObj_init(self, parent);
    OObj_id_set(self, USplit_ID);

    self->child_a = child_a;
    self->child_b = child_b;
    self->mode = mode;
    self->seperator = seperator;
    self->spacing = spacing;
    self->even_sizes = true;

    return self;
}


void USplit_update_ex(oobj obj, oobj tex, ivec4 viewport)
{
    OObj_assert(obj, USplit);
    USplit *self = obj;

    // in units
    vec4 lbwh = r_proj_viewport_to_lbwh(RTex_proj(tex), viewport, NULL);
    float l = lbwh.v0;
    float b = lbwh.v1;
    float w = lbwh.v2;
    float h = lbwh.v3;

    if (w <= 0 || h <= 0) {
        return;
    }

    // lbwh
    float a_left, a_bottom, a_width, a_height;
    float b_left, b_bottom, b_width, b_height;

    if(self->mode == USplit_MODE_H_LEFT) {
        // full height
        a_bottom = b_bottom = b;
        a_height = b_height = even(self, h);

        // a left until seperator
        a_left = l;
        a_width = self->seperator;
        a_width = o_clamp(a_width, 0, w-self->spacing);
        a_width = even(self, a_width);

        // b behind a
        b_left = a_left + a_width + self->spacing;
        b_width = l + w - b_left;
        b_width = o_clamp(b_width, 0, w-self->spacing);
        float b_width_even = even(self, b_width);
        float even_diff = b_width - b_width_even;
        b_width = b_width_even;
        b_left += even_diff;
    } 
    
    else if(self->mode == USplit_MODE_H_RIGHT) {
        // full height
        a_bottom = b_bottom = b;
        a_height = b_height = even(self, h);

        // a left = width - seperator - spacing
        a_left = l;
        a_width = w - self->seperator - self->spacing;
        a_width = o_clamp(a_width, 0, w-self->spacing);
        a_width = even(self, a_width);

        // b behind a
        b_left = a_left + a_width + self->spacing;
        b_width = l + w - b_left;
        b_width = o_clamp(b_width, 0, w-self->spacing);
        float b_width_even = even(self, b_width);
        float even_diff = b_width - b_width_even;
        b_width = b_width_even;
        b_left += even_diff;
    }
    
    else if(self->mode == USplit_MODE_V_BOTTOM) {
        // full width
        a_left = b_left = l;
        a_width = b_width = even(self, w);

        // a bottom until seperator
        a_bottom = b;
        a_height = self->seperator;
        a_height = o_clamp(a_height, 0, h-self->spacing);
        a_height = even(self, a_height);

        // b behind a
        b_bottom = a_bottom + a_height + self->spacing;
        b_height = b + h - b_bottom;
        b_height = o_clamp(b_height, 0, h-self->spacing);
        float b_height_even = even(self, b_height);
        float even_diff = b_height - b_height_even;
        b_height = b_height_even;
        b_bottom += even_diff;
    }
    
    else {
        // full width
        a_left = b_left = l;
        a_width = b_width = even(self, w);

        // a bottom = height - seperator - spacing
        a_bottom = b;
        a_height = h - self->seperator - self->spacing;
        a_height = o_clamp(a_height, 0, h-self->spacing);
        a_height = even(self, a_height);

        // b behind a
        b_bottom = a_bottom + a_height + self->spacing;
        b_height = b + h - b_bottom;
        b_height = o_clamp(b_height, 0, h-self->spacing);
        float b_height_even = even(self, b_height);
        float even_diff = b_height - b_height_even;
        b_height = b_height_even;
        b_bottom += even_diff;
    }


    ivec4 vp_a = r_proj_lbwh_to_viewport(RTex_proj(tex), vec4_(a_left, a_bottom, a_width, a_height), NULL);
    ivec4 vp_b = r_proj_lbwh_to_viewport(RTex_proj(tex), vec4_(b_left, b_bottom, b_width, b_height), NULL);

    update(self->child_a, tex, vp_a);
    update(self->child_b, tex, vp_b);
}

void USplit_update(oobj obj, oobj tex)
{
    USplit_update_ex(obj, tex, RTex_viewport(tex));
}

void USplit_render(oobj obj, oobj tex)
{
    OObj_assert(obj, USplit);
    USplit *self = obj;

    split_render(self->child_a, tex);
    split_render(self->child_b, tex);
}

O_STATIC
void collect_views_r(oobj obj, oobj list)
{
    if(OObj_check(obj, USplit)) {
        USplit *split = obj;
        collect_views_r(split->child_a, list);
        collect_views_r(split->child_b, list);
    } else if (OObj_check(obj, AView)) {
        OArray_push(list, &obj);
    } else {
        o_log_warn_s("USplit_views", "child invalid, should have been USplit or AView");
    }
}

AView **USplit_views(oobj obj)
{
    oobj list = OArray_new_dyn(obj, NULL, sizeof(AView *), 0, 8);
    collect_views_r(obj, list);
    AView **res = OArray_data(list, AView *);
    OArray_move(list, obj);
    o_del(list);
    return res;
}
