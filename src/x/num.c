#include "x/num.h"
#include "x/viewtext.h"
#include "o/OArray.h"
#include "o/str.h"
#include "a/AScene.h"
#include "w/WText.h"
#include "x/XViewText.h"
#include "x/XViewKeys.h"
#include "m/utils/color.h"


#define NUM_INJECT "x_num_scene"


O_STATIC
void num_scene_ok(oobj scene)
{
    oobj view = AScene_view(scene);
    oobj text_array = XViewText_text_array(view);
    
    struct oobj_opt inject = OObj_find(scene, OObj, NUM_INJECT, 0);
    assert(inject.o);
    oobj num = o_user(inject.o);
    OObj_assert(num, WNum);
    
    float val;
    if(sscanf(OArray_data_void(text_array), "%f", &val) == 1) {
        WNum_num_set(num, val);
    } else {
        WNum_num_set(num, 0);
    }
}

O_STATIC
void num_scene_cancel(oobj scene)
{
    // noop
}


O_STATIC
void num_num_pressed(oobj num)
{
    float val = WNum_num(num);
    
    const char *title = "number";
    oobj label = WNum_frame_label(num);
    if(label) {
        title = WText_text(label);
    }
    
    oobj scene = x_viewtext_scene(num, title, 40, num_scene_ok, num_scene_cancel);
    oobj view = AScene_view(scene);

    float min = WNum_min(num);
    float max = WNum_max(num);
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
    OArray_append_stringf(text_array, "%.*f", WNum_decimal_places(num), val);
    
    oobj inject = OObj_new(scene);
    OObj_name_set(inject, NUM_INJECT);
    o_user_set(inject, num);
}

//
// public
//

WNum *x_num(oobj parent, float min, float max, float step, const char *opt_label)
{
    WNum *self = WNum_new(parent, min, max, step, opt_label);
    WNum_num_pressed_event_set(self, num_num_pressed);
    return self;
}
