#include "x/iui.h"
#include "o/OArray.h"
#include "o/OMap.h"
#include "a/app.h"
#include "a/AView.h"
#include "w/WStack.h"
#include "w/WTheme.h"
#include "w/WWindow.h"
#include "w/WNum.h"
#include "w/WDrag.h"
#include "w/WBox.h"
#include "x/num.h"
#include "x/XWObjColor.h"

#include "r/RTex.h"

#define O_LOG_LIB "x"
#include "o/log.h"


#define VIEWPASTE_NAME "x_iui__viewpaste"

struct viewpaste {
    oobj o;
    oobj map;
    oobj theme;
    oobj stack;
    vec4 color_uv_rect;
    vec2 lt;
};

struct item {
    bool used;
    oobj wobj;
};


O_STATIC
struct item *viewpaste_item(struct viewpaste *self, const char *title, x_iui_custom__fn setup_fn, void *user_data)
{
    struct item *item_ref = OMap_get(self->map, &title, struct item);
    if(!item_ref) {
        struct item item = {0};
        item.wobj = WObj_new(self->stack);
        setup_fn(item.wobj, user_data);
        o_log_debug_s("iui", "added: %s", title);
        osize idx = OMap_set(self->map, &title, &item);
        item_ref = o_at(self->map, idx);
    }
    item_ref->used = true;
    return item_ref;
}
O_STATIC
struct item *viewpaste_item_window(struct viewpaste *self, const char *title, x_iui_custom__fn setup_fn, void *user_data)
{
    struct item *item_ref = OMap_get(self->map, &title, struct item);
    if(!item_ref) {
        struct item item = {0};
        item.wobj = WWindow_new_title(self->stack, vec2_(32, 10), title, NULL);
        osize cnt = o_num(self->map);
        vec2 lt = vec2_(self->lt.x + cnt*6, self->lt.y + cnt*12);
        WWindow_lt_set(item.wobj, lt);
        WWindow_hidden_set(item.wobj, true);
        setup_fn(item.wobj, user_data);
        o_log_debug_s("iui", "added window: %s", title);
        osize idx = OMap_set(self->map, &title, &item);
        item_ref = o_at(self->map, idx);
    }
    item_ref->used = true;
    return item_ref;
}

O_STATIC
struct viewpaste *viewpaste_tryget(oobj view)
{
    struct oobj_opt glue = OObj_find(view, OObj, VIEWPASTE_NAME, 0);
    return glue.o? o_user(glue.o) : NULL;
}


O_STATIC
void on_update(oobj view, oobj tex, float dt)
{
    struct viewpaste *vp = viewpaste_tryget(view);
    if(!vp) {
        o_log_wtf_s("iui", "no viewpaste?");
        return;
    }

    // remove unused
    for(osize i=0; i<o_num(vp->map); i++) {
        assert(i>=0);
        struct item *item = o_at(vp->map, i);
        if(!item->used) {
            o_del(item->wobj);
            const char **title = OMap_key_at(vp->map, i, char*);
            o_log_debug_s("iui", "removed: %s", *title);
            OMap_remove(vp->map, title);
            i = -1;
            // (-1)++ -> 0
            continue;
        }
    }

    // set unused for next frame
    for(osize i=0; i<o_num(vp->map); i++) {
        struct item *item = o_at(vp->map, i);
        item->used = false;
    }
    
    WTheme_update_stack(vp->theme, vp->stack, tex, NULL, a_pointer);

}

O_STATIC
void on_render(oobj view, oobj tex, float dt)
{
    struct viewpaste *vp = viewpaste_tryget(view);
    if(!vp) {
        o_log_wtf_s("iui", "no viewpaste?");
        return;
    }
    
    WTheme_render(vp->theme, tex);
}



O_STATIC
struct viewpaste *viewpaste_glue(oobj view) 
{
    struct viewpaste *vp = viewpaste_tryget(view);
    if(vp) {
        return vp;
    }
    // new viepaste 'n glue:
    oobj glue = OObj_new(view);
    OObj_name_set(glue, VIEWPASTE_NAME);
    vp = o_new0(glue, *vp, 1);
    o_user_set(glue, vp);
    vp->o = OObj_new(glue);
    
    // ...
    vp->theme = WTheme_new_tiny(vp->o);
    vp->color_uv_rect = u_atlas_rect(WTheme_atlas(vp->theme), WTheme_CUSTOM_32);

    vp->stack = WStack_new(vp->o);
    WStack_window_auto_mode_set(vp->stack, true);
    
    vp->map = OMap_new_string_keys(vp->o, sizeof (struct item), 32);
    
    // add loop and event to view
    struct AView_layer layer = {0};
    layer.update = on_update;
    layer.render = on_render;
    AView_push_layer_deferred(view, layer, true);
    return vp;
}


//
// public
//

void x_iui_custom(const char *title, x_iui_custom__fn setup_fn, x_iui_custom__fn update_fn, void *user_data)
{
    struct viewpaste *vp = viewpaste_glue(a_app_view());
    struct item *item = viewpaste_item(vp, title, setup_fn, user_data);
    update_fn(item->wobj, user_data);
}

void x_iui_custom_window(const char *title, x_iui_custom__fn setup_fn, x_iui_custom__fn update_fn, void *user_data)
{
    struct viewpaste *vp = viewpaste_glue(a_app_view());
    struct item *item = viewpaste_item_window(vp, title, setup_fn, user_data);
    update_fn(item->wobj, user_data);
}


struct oobj_opt x_iui_wobj(const char *title)
{
    struct viewpaste *vp = viewpaste_tryget(a_app_view());
    if(!vp) {
        return oobj_opt(NULL);
    }
    struct item *item_ref = OMap_get(vp->map, &title, struct item);
    if(!item_ref) {
        return oobj_opt(NULL);
    }
    return oobj_opt(item_ref->wobj);
}

vec2 x_iui_init_lt(void)
{
    struct viewpaste *vp = viewpaste_glue(a_app_view());
    return vp->lt;
}

vec2 x_iui_init_lt_set(vec2 set)
{
    struct viewpaste *vp = viewpaste_glue(a_app_view());
    vp->lt = set;
    return vp->lt;
}



struct vecX_data {
    const char *title;
    int n;
    vec4 *val, min, max;
};

O_STATIC
void vecX_setup(oobj wobj_parent, void *user_data)
{
    struct vecX_data *data = user_data;
    vec4 step = vec4_div(vec4_sub_v(data->max, data->min), 1000.0f);
    oobj box = WBox_new(WWindow_body(wobj_parent), WBox_LAYOUT_V);
    for(int i=0; i<data->n; i++) {
        char label[2] = {(char) ('X'+i), '\0'};
        oobj num = x_num(box, data->min.v[i], data->max.v[i], step.v[i], 
                data->n>1? label : NULL);
        OObj_name_set(num, label);
        WObj_min_size_ref(num)->x = 80;
        WNum_num_set(num, data->val->v[i]);
    }
}

O_STATIC
void vecX_update(oobj wobj_parent, void *user_data)
{
    struct vecX_data *data = user_data;
    for(int i=0; i<data->n; i++) {
        char label[2] = {(char) ('X'+i), '\0'};
        float step = (data->max.v[i] - data->min.v[i]) / 1000.0f;
        struct oobj_opt num = OObj_find(wobj_parent, WNum, data->n>1? label : NULL, oi32_MAX);
        assert(num.o);
        WNum_min_set(num.o, data->min.v[i]);
        WNum_max_set(num.o, data->max.v[i]);
        WNum_step_set(num.o, step);
        data->val->v[i] = WNum_num(num.o);
    }
}

void x_iui_float(const char *title, float *val, float min, float max)
{
    vec4 val4, min4, max4;
    val4.x = *val;
    min4.x = min;
    max4.x = max;
    struct vecX_data data = {title, 1, &val4, min4, max4};
    x_iui_custom_window(title, vecX_setup, vecX_update, &data);
    *val = val4.x;
}

void x_iui_vec2(const char *title, vec2 *val, vec2 min, vec2 max)
{
    vec4 val4, min4, max4;
    val4.xy = *val;
    min4.xy = min;
    max4.xy = max;
    struct vecX_data data = {title, 2, &val4, min4, max4};
    x_iui_custom_window(title, vecX_setup, vecX_update, &data);
    *val = val4.xy;
}

void x_iui_vec3(const char *title, vec3 *val, vec3 min, vec3 max)
{
    vec4 val4, min4, max4;
    val4.xyz = *val;
    min4.xyz = min;
    max4.xyz = max;
    struct vecX_data data = {title, 3, &val4, min4, max4};
    x_iui_custom_window(title, vecX_setup, vecX_update, &data);
    *val = val4.xyz;
}

void x_iui_vec4(const char *title, vec4 *val, vec4 min, vec4 max)
{
    struct vecX_data data = {title, 4, val, min, max};
    x_iui_custom_window(title, vecX_setup, vecX_update, &data);
}




struct rgba_data {
    const char *title;
    bool alpha;
    vec4 *rgba;
};

O_STATIC
void rgba_setup(oobj wobj_parent, void *user_data)
{
    struct rgba_data *data = user_data;
    struct viewpaste *vp = viewpaste_tryget(a_app_view());
    assert(vp);
    vec4 uv_rect = vp->color_uv_rect;
    XWObjColor_new(WWindow_body(wobj_parent), uv_rect, *data->rgba, data->alpha);
    uv_rect.y -= 7.0f/2.0f;
    uv_rect.v3 -= 7.0f;
    vp->color_uv_rect = uv_rect;
}

O_STATIC
void rgba_update(oobj wobj_parent, void *user_data)
{
    struct rgba_data *data = user_data;
    struct oobj_opt color = OObj_find(wobj_parent, XWObjColor, NULL, oi32_MAX);
    assert(color.o);
    vec4 rgba = XWObjColor_rgba(color.o);
    *data->rgba = rgba;
}


void x_iui_rgb(const char *title, vec3 *val)
{
    vec4 rgba;
    rgba.xyz = *val;
    rgba.a = 1;
    struct rgba_data data = {title, false, &rgba};
    x_iui_custom_window(title, rgba_setup, rgba_update, &data);
    *val = rgba.xyz;
}

void x_iui_rgba(const char *title, vec4 *val)
{
    struct rgba_data data = {title, true, val};
    x_iui_custom_window(title, rgba_setup, rgba_update, &data);
}
