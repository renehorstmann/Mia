#include "u/splash.h"
#include "o/OArray.h"
#include "o/str.h"
#include "r/RObjText.h"
#include "r/RTex.h"
#include "a/app.h"
#include "a/ADefer.h"
#include "a/AView.h"
#include "a/AScene.h"
#include "u/pose.h"

#define O_LOG_LIB "u"
#include "o/log.h"

#define SPLASH_INJECT "u_splash_context"

struct splash_context {
    oobj splash_scene;

    float time;
    oobj parent;
    oobj actual_view;
    bool move_actual_view_in_its_scene;
    oobj actual_scene;

};


O_STATIC
void splash_defer_switch(oobj defer)
{
    struct splash_context *C = o_user(defer);
    int scene_splash = a_app_scene_get_index(C->splash_scene);
    int scene_actual = a_app_scene_get_index(C->actual_scene);
    a_app_scene_switch_stack_position(scene_actual, scene_splash);
}


O_STATIC
void splash_update(oobj view, oobj tex, float dt)
{
    struct oobj_opt inject = OObj_find(view, OObj, SPLASH_INJECT, 0);
    if(!inject.o) {
        o_log_error_s("u_splash_layer_update", "failed to find inject object");
        return;
    }
    struct splash_context *C = o_user(inject.o);
    if(C->time>=0) {
        C->time-=dt;
        if(C->time<0) {
            C->actual_scene = AScene_new(C->parent, C->actual_view, C->move_actual_view_in_its_scene);
            // AScene registration runs deferred, so we also create a defer which will get executed directly after that
            oobj defer = ADefer_new(view, splash_defer_switch);
            o_user_set(defer, C);
        }
    }

}

O_STATIC
void splash_render(oobj view, oobj tex, float dt)
{
    // noop
}


struct oobj_opt u_splash_actual_scene(oobj splash_view)
{
    struct oobj_opt inject = OObj_find(splash_view, OObj, SPLASH_INJECT, 0);
    if(!inject.o) {
        o_log_error_s(__func__, "failed to find inject object");
        return oobj_opt(NULL);
    }
    struct splash_context *C = o_user(inject.o);
    return oobj_opt(C->actual_scene);
}

oobj u_splash_new(oobj parent,
                  oobj splash_view, bool move_splash_view,
                  oobj actual_view, bool move_actual_view_in_its_scene,
                  float time_until_actual_scene_creation)
{
    struct AView_layer layer = {0};
    layer.update = splash_update;
    layer.render = splash_render;
    OArray_push(AView_layers(splash_view), &layer);
    oobj inject = OObj_new(splash_view);
    OObj_name_set(inject, SPLASH_INJECT);
    struct splash_context *C = o_user_set(inject, o_new0(inject, *C, 1));
    C->time = time_until_actual_scene_creation;
    C->parent = parent;
    C->actual_view = actual_view;
    C->move_actual_view_in_its_scene = move_actual_view_in_its_scene;

    oobj scene = AScene_new(parent, splash_view, move_splash_view);
    C->splash_scene = scene;

    return scene;
}


//
// mia splash
//

struct mia_splash_context {
    char *author;
    vec4 bg_color;
    vec4 text_color;
    float time;
    float min_time;
    float fade_time;
    oobj ro;
};

O_STATIC
void mia_splash_setup(oobj view)
{
    struct mia_splash_context *C = o_user(view);
    C->ro = RObjText_new_font35(view, o_strlen(C->author), NULL);
    RObjText_text_mode_set(C->ro, RObjText_MODE_DEFAULT);
    // mode default is used on _text_set, not while rendering
    RObjText_text_set(C->ro, C->author);
    vec2 size = RObjText_text_size(C->ro);

    // w and h == 2 -> default
    RObjText_pose_set(C->ro, u_pose_new(-size.x, 0, 4, 4));
}

O_STATIC
void mia_splash_update(oobj view, oobj tex, float dt)
{
    // noop
}


O_STATIC
void mia_splash_render(oobj view, oobj tex, float dt)
{
    struct mia_splash_context *C = o_user(view);
    vec4 bg_color = C->bg_color;
    vec4 text_color = C->text_color;
    if(C->time<C->min_time) {
        C->time+=dt;
        if(C->time>=C->min_time) {
            a_app_scene_exit(a_app_scene_index());
        }
        float fading = C->time - (C->min_time - C->fade_time);
        if(fading>0) {
            fading /= C->fade_time;
            float alpha = 1.0f-fading;
            bg_color.a *= alpha;
            text_color.a *= alpha;
        }
    }

    RTex_clear_full(tex, bg_color);
    RObjText_color_set(C->ro, text_color);
    RTex_ro(tex, C->ro);
}




oobj u_splash_new_mia(oobj parent,
                      oobj actual_view, bool move_actual_view_in_its_scene,
                      vec4 bg_color, vec4 text_color, const char *author, float min_time, float fade_time)
{
    oobj splash = AView_new(parent, mia_splash_setup, mia_splash_update, mia_splash_render);
    struct mia_splash_context *C = o_user_set(splash, o_new0(splash, *C, 1));
    C->author = o_str_clone(splash, author);
    C->bg_color = bg_color;
    C->text_color = text_color;
    C->min_time = min_time;
    C->fade_time = fade_time;
    oobj scene = u_splash_new(parent, splash, true, actual_view, move_actual_view_in_its_scene, 0.0f);
    AScene_opaque_set(scene, false);
    return scene;
}
