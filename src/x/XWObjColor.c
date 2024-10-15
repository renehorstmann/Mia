#include "x/XWObjColor.h"
#include "o/OObj_builder.h"
#include "o/OArray.h"
#include "o/str.h"
#include "m/utils/color.h"
#include "m/vec/bvec4.h"
#include "r/RTex.h"
#include "a/AScene.h"
#include "u/rect.h"
#include "u/gradient.h"
#include "w/WBox.h"
#include "w/WTheme.h"
#include "w/WImg.h"
#include "w/WClick.h"
#include "w/WColor.h"
#include "w/WText.h"
#include "w/WFrame.h"
#include "x/XViewText.h"
#include "x/gradient.h"
#include "x/viewtext.h"

#define O_LOG_LIB "x"
#include "o/log.h"


#define HEX_INJECT "XWObjColor_hex_scene"

O_STATIC
void draw_gradients(XWObjColor *self, oobj theme)
{
    bvec4 gradient[32];
    vec3 hsv;
    vec4 rgba;
    hsv.v0 = WGradient_progress(self->hue);
    hsv.v1 = WGradient_progress(self->sat);
    hsv.v2 = WGradient_progress(self->val);
    rgba.v0 = WGradient_progress(self->red);
    rgba.v1 = WGradient_progress(self->green);
    rgba.v2 = WGradient_progress(self->blue);
    rgba.v3 = WGradient_progress(self->alpha);
    
    vec4 hue_uv = WImg_uv_rect(WGradient_imgpicker(self->hue));
    vec4 sat_uv = WImg_uv_rect(WGradient_imgpicker(self->sat));
    vec4 val_uv = WImg_uv_rect(WGradient_imgpicker(self->val));
    vec4 red_uv = WImg_uv_rect(WGradient_imgpicker(self->red));
    vec4 green_uv = WImg_uv_rect(WGradient_imgpicker(self->green));
    vec4 blue_uv = WImg_uv_rect(WGradient_imgpicker(self->blue));
    vec4 alpha_uv = WImg_uv_rect(WGradient_imgpicker(self->alpha));

    oobj theme_tex = WTheme_tex(theme);

    u_gradient_hue(gradient, 32);
    RTex_set(self->gradient_tex, gradient);
    RTex_blit(theme_tex, self->gradient_tex, m_2(hue_uv));

    u_gradient_sat(gradient, 32, hsv.v0);
    RTex_set(self->gradient_tex, gradient);
    RTex_blit(theme_tex, self->gradient_tex, m_2(sat_uv));

    u_gradient_val(gradient, 32, hsv.v0, hsv.v1);
    RTex_set(self->gradient_tex, gradient);
    RTex_blit(theme_tex, self->gradient_tex, m_2(val_uv));

    u_gradient_red(gradient, 32, rgba.v1, rgba.v2);
    RTex_set(self->gradient_tex, gradient);
    RTex_blit(theme_tex, self->gradient_tex, m_2(red_uv));

    u_gradient_green(gradient, 32, rgba.v0, rgba.v2);
    RTex_set(self->gradient_tex, gradient);
    RTex_blit(theme_tex, self->gradient_tex, m_2(green_uv));

    u_gradient_blue(gradient, 32, rgba.v0, rgba.v1);
    RTex_set(self->gradient_tex, gradient);
    RTex_blit(theme_tex, self->gradient_tex, m_2(blue_uv));

    u_gradient_alpha(gradient, 32, rgba.v0, rgba.v1, rgba.v2);
    RTex_set(self->gradient_tex, gradient);
    RTex_blit(theme_tex, self->gradient_tex, m_2(alpha_uv));


    WColor_color_set(self->color_new, rgba);
//    RTex_write_file(theme_tex, "debug.testcustom.png");

    // update hex text
    bvec4 hex = bvec4_cast_float_1(rgba.v);
    char buf[16];
    if(self->alpha_enabled) {
        o_strf_buf(buf, "#%02X%02X%02X%02X", hex.r, hex.g, hex.b, hex.a);
    } else {
        o_strf_buf(buf, "#%02X%02X%02X", hex.r, hex.g, hex.b);
    }
    WText_text_set(self->hex, buf);

    self->rgba = rgba;
}

O_STATIC
void on_color_hsv_update(oobj gradient)
{
    XWObjColor *self = o_user(gradient);
    vec3 hsv;
    hsv.v0 = WGradient_progress(self->hue);
    hsv.v1 = WGradient_progress(self->sat);
    hsv.v2 = WGradient_progress(self->val);
    vec3 rgb = vec3_hsv2rgb(hsv);
    WGradient_progress_set(self->red, rgb.v0);
    WGradient_progress_set(self->green, rgb.v1);
    WGradient_progress_set(self->blue, rgb.v2);
    self->gradients_update = true;
}

O_STATIC
void on_color_rgba_update(oobj gradient)
{
    XWObjColor *self = o_user(gradient);
    vec3 rgb;
    rgb.v0 = WGradient_progress(self->red);
    rgb.v1 = WGradient_progress(self->green);
    rgb.v2 = WGradient_progress(self->blue);
    vec3 hsv = vec3_rgb2hsv(rgb);
    WGradient_progress_set(self->hue, hsv.v0);
    WGradient_progress_set(self->sat, hsv.v1);
    WGradient_progress_set(self->val, hsv.v2);
    self->gradients_update = true;
}



O_STATIC
void hex_scene_ok(oobj scene)
{
    oobj view = AScene_view(scene);
    oobj text = XViewText_text_array(view);

    struct oobj_opt inject = OObj_find(scene, OObj, HEX_INJECT, 0);
    assert(inject.o);
    XWObjColor *self = o_user(inject.o);
    OObj_assert(self, XWObjColor);
    
    const char *text_str = OArray_data_void(text);
    if(*text_str == '#') {
        text_str++;
    }

    unsigned int hex[4];
    if(sscanf(text_str, "%2x%2x%2x%2x", &hex[0], &hex[1], &hex[2], &hex[3]) == 4) {
        bvec4 hex_rgba = {{hex[0], hex[1], hex[2], hex[3]}};
        self->rgba = vec4_cast_byte_1(hex_rgba.v);
        self->rgba_update = true;
    } else if(sscanf(text_str,  "%2x%2x%2x", &hex[0], &hex[1], &hex[2]) == 3) {
        bvec4 hex_rgba = {{hex[0], hex[1], hex[2], 255}};
        self->rgba = vec4_cast_byte_1(hex_rgba.v);
        self->rgba_update = true;
    }
}

O_STATIC
void hex_scene_cancel(oobj scene)
{
    // noop
}

O_STATIC
void on_hex_pressed(oobj click)
{
    XWObjColor *self = o_user(click);

    const char *title = "RGBA HEX CODE";

    oobj scene = x_viewtext_scene(self, title, 9, hex_scene_ok, hex_scene_cancel);
    oobj view = AScene_view(scene);
    oobj text = XViewText_text_array(view);

    OArray_clear(text);
    OArray_append_string(text, WText_text(self->hex));

    oobj inject = OObj_new(scene);
    OObj_name_set(inject, HEX_INJECT);
    o_user_set(inject, self);
}

void on_prev_pressed(oobj click)
{
    XWObjColor *self = o_user(click);
    self->rgba = self->rgba_prev;
    self->rgba_update = true;
    o_log_s("XObjColor", "PREV rgba: vec4_(%g,%g,%g,%g)", m_4(self->rgba));
}

void on_new_pressed(oobj click)
{
    XWObjColor *self = o_user(click);
    o_log_s("XObjColor", "NEW rgba: vec4_(%g,%g,%g,%g)", m_4(self->rgba));
}

//
// public
//

XWObjColor *XWObjColor_init(oobj obj, oobj parent, vec4 uv_rect, vec4 rgba_prev, bool enable_alpha)
{
    WObj *super = obj;
    XWObjColor *self = obj;
    o_clear(self, sizeof *self, 1);

    WObj_init(obj, parent);
    OObj_id_set(self, XWObjColor_ID);

    assert(uv_rect.v2>=1 && uv_rect.v3 >= 7 && "needs at least 7 rows to draw into");
    
    self->alpha_enabled = enable_alpha; 
    if(!self->alpha_enabled) {
        rgba_prev.a = 1.0f;
    }
    
    self->rgba_prev = rgba_prev;
    self->rgba = rgba_prev;
    self->rgba_update = true;
    
    
    vec4 uvs[7];
    uvs[0] = uv_rect;
    uvs[0].y += uvs[0].v3/2-0.5;
    uvs[0].v2 = m_floor(uvs[0].v2);
    uvs[0].v3 = 1;
    for(int i=1; i<7; i++) {
        uvs[i] = uvs[0];
        uvs[i].y -= (float) i;
    }

    self->gradient_tex = RTex_new(self, NULL,
                                  (int) uvs[0].v2, (int) uvs[0].v3);

    self->box = WBox_new(self, WBox_LAYOUT_V);
    WBox_spacing_set(self->box, vec2_(2));

    self->hue = x_gradient(self->box, uvs[0], 0, 360, 0.5f, "HUE");
    o_user_set(self->hue, self);
    WGradient_change_event_set(self->hue, on_color_hsv_update);

    self->sat = x_gradient(self->box, uvs[1], 0, 100, 0.1f, "SAT");
    o_user_set(self->sat, self);
    WGradient_change_event_set(self->sat, on_color_hsv_update);

    self->val = x_gradient(self->box, uvs[2], 0, 100, 0.1f, "VAL");
    o_user_set(self->val, self);
    WGradient_change_event_set(self->val, on_color_hsv_update);


    self->red = x_gradient(self->box, uvs[3], 0, 255, 1, "RED");
    o_user_set(self->red, self);
    WGradient_change_event_set(self->red, on_color_rgba_update);

    self->green = x_gradient(self->box, uvs[4], 0, 255, 1, "GREEN");
    o_user_set(self->green, self);
    WGradient_change_event_set(self->green, on_color_rgba_update);

    self->blue = x_gradient(self->box, uvs[5], 0, 255, 1, "BLUE");
    o_user_set(self->blue, self);
    WGradient_change_event_set(self->blue, on_color_rgba_update);

    self->alpha = x_gradient(self->box, uvs[6], 0, 255, 1, "ALPHA");
    o_user_set(self->alpha, self);
    WGradient_change_event_set(self->alpha, on_color_rgba_update);


    oobj frame;
    oobj click;

    frame = WFrame_new(self->box);
    WText_new(WFrame_label(frame), "HEX");
    click = WClick_new(frame);
    o_user_set(click, self);
    WClick_pressed_event_set(click, on_hex_pressed);
    self->hex = WText_new(click, "#FFFFFFFF");
    WObj_padding_set(self->hex, vec4_(1));


    oobj col_box = WBox_new(self->box, WBox_LAYOUT_H);
    WBox_spacing_set(col_box, vec2_(2));

    frame = WFrame_new(col_box);
    WText_new(WFrame_label(frame), "PREV");
    click = WClick_new(frame);
    o_user_set(click, self);
    WClick_pressed_event_set(click, on_prev_pressed);
    self->color_prev = WColor_new(click, rgba_prev);
    WObj_min_size_set(self->color_prev, vec2_(32, 8));

    frame = WFrame_new(col_box);
    WText_new(WFrame_label(frame), "NEW");
    click = WClick_new(frame);
    o_user_set(click, self);
    WClick_pressed_event_set(click, on_new_pressed);
    self->color_new = WColor_new(click, rgba_prev);
    WObj_min_size_set(self->color_new, vec2_(32, 8));

    // disable with moving into an isolated container
    if(!enable_alpha) {
        oobj container = OObj_new(self);
        o_move(self->alpha, container);
    }
    
    // vfuncs
    super->v_update = XWObjColor__v_update;

    return self;
}

//
// virtual implementations
//

vec2 XWObjColor__v_update(oobj obj, vec2 lt, vec2 min_size, oobj theme, a_pointer__fn pointer_fn)
{
    OObj_assert(obj, XWObjColor);
    XWObjColor *self = obj;

    if(self->rgba_update) {
        self->rgba_update = false;
        self->gradients_update = true;
        
        if(!self->alpha_enabled) {
            self->rgba.a = 1.0f;
        }

        WGradient_progress_set(self->red, self->rgba.r);
        WGradient_progress_set(self->green, self->rgba.g);
        WGradient_progress_set(self->blue, self->rgba.b);
        WGradient_progress_set(self->alpha, self->rgba.a);

        // update hsv...
        on_color_rgba_update(self->red);
    }

    if(self->gradients_update) {
        self->gradients_update = false;
        draw_gradients(self, theme);
    }


    return WObj__v_update(obj, lt, min_size, theme, pointer_fn);
}

//
// object functions
//

