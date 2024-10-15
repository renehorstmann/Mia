#include "x/gradient.h"
#include "x/viewtext.h"
#include "o/OArray.h"
#include "o/str.h"
#include "a/AScene.h"
#include "w/WText.h"
#include "x/XViewText.h"
#include "x/XViewKeys.h"
#include "m/utils/color.h"


#define GRADIENT_INJECT "x_gradient_scene"


O_STATIC
void gradient_scene_ok(oobj scene)
{
    oobj view = AScene_view(scene);
    oobj text_array = XViewText_text_array(view);
    
    struct oobj_opt inject = OObj_find(scene, OObj, GRADIENT_INJECT, 0);
    assert(inject.o);
    oobj gradient = o_user(inject.o);
    OObj_assert(gradient, WGradient);
    
    float val;
    if(sscanf(OArray_data_void(text_array), "%f", &val) == 1) {
        WGradient_num_set(gradient, val);
    } else {
        WGradient_num_set(gradient, 0);
    }
}

O_STATIC
void gradient_scene_cancel(oobj scene)
{
    // noop
}

O_STATIC
void gradient_num_pressed(oobj gradient)
{
    float val = WGradient_num(gradient);
    
    const char *title = "number";
    oobj label = WGradient_frame_label(gradient);
    if(label) {
        title = WText_text(label);
    }
    
    oobj scene = x_viewtext_scene(gradient, title, 40, gradient_scene_ok, gradient_scene_cancel);
    oobj view = AScene_view(scene);

    float min = WGradient_min(gradient);
    float max = WGradient_max(gradient);
    const int max_btns = 6;
    for(int i=0; i<max_btns; i++) {
        char repl[16];
        float t = (float) i / (float) (max_btns-1);
        o_strf_buf(repl, "%g", m_mix(min, max, t));
        vec4 rgba = vec4_hsv2rgb(vec4_(t * 0.333, 0.5, 0.5, 1.0));
        XViewText_custom_replace(view, repl, &rgba);
    }

    oobj keys = XViewText_viewkeys(view);
    XViewKeys_mode_set(keys, XViewKeys_NUM);
    oobj text_array = XViewText_text_array(view);
    
    OArray_clear(text_array);
    OArray_append_stringf(text_array, "%.*f", WGradient_decimal_places(gradient), val);
    
    oobj inject = OObj_new(scene);
    OObj_name_set(inject, GRADIENT_INJECT);
    o_user_set(inject, gradient);
}

//
// public
//

WGradient *x_gradient(oobj parent, vec4 uv_rect, float min, float max, float step, const char *opt_label)
{
    WGradient *self = WGradient_new(parent, uv_rect, min, max, step, opt_label);
    WGradient_num_pressed_event_set(self, gradient_num_pressed);
    return self;
}
