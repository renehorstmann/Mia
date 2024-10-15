#include "a/AView.h"
#include "o/OObj_builder.h"
#include "o/OArray.h"
#include "r/RCam.h"
#include "r/RTex.h"
#include "a/app.h"
#include "a/ADefer.h"
#include "m/mat/mat4.h"
#include "m/vec/ivec4.h"
#include "m/vec/vec4.h"
#include "u/pose.h"

#define O_LOG_LIB "a"

#include "o/log.h"


AView *AView_init(oobj obj, oobj parent,
                  OObj__event_fn opt_setup,
                  AView__fn update, AView__fn render)
{

    AView *self = obj;
    o_clear(self, sizeof *self, 1);

    OObj_init(self, parent);
    OObj_id_set(self, AView_ID);

    self->cam = RCam_new(self, true, vec2_(180));
    self->cam_units_auto = true;

    self->scale = 1.0f;

    self->scale_auto = false;
    self->scale_auto_max = m_MAX;

    self->render_auto = true;
    self->time = 0;

    self->tex = NULL;
    self->own_tex = NULL;
    self->use_own_tex = false;
    
    // v func lists
    struct AView_layer layer = {
        opt_setup, update, render
    };
    self->layers = OArray_new(self, &layer, sizeof (struct AView_layer), 1);
    self->current_layer = -1;
    

    // v funcs
    self->v_pointer_viewport = AView__v_pointer_viewport;

    return self;
}


struct a_pointer AView__v_pointer_viewport(oobj obj, int idx, int history)
{
    return a_pointer_back_viewport(idx, history);
}


void AView_update(oobj obj, oobj tex, ivec4 viewport)
{
    OObj_assert(obj, AView);
    AView *self = obj;

    self->viewport = viewport;

    ivec4 tex_viewport = RTex_viewport(tex);
    struct r_proj tex_proj = *RTex_proj(tex);
    float tex_scale = tex_proj.scale;

    if (self->scale_auto) {
        self->scale = o_min(tex_scale, self->scale_auto_max);
    }

    vec4 lbwh = r_proj_viewport_to_lbwh(&tex_proj, viewport, NULL);

    int cols, rows;

    if (self->use_own_tex) {
        cols = (int) m_floor(lbwh.v2 * self->scale);
        rows = (int) m_floor(lbwh.v3 * self->scale);

        if (!self->own_tex
            || RTex_size_int(self->own_tex).x != cols
            || RTex_size_int(self->own_tex).y != rows) {

            o_del(self->own_tex);
            self->own_tex = RTex_new(self, NULL, cols, rows);
        }

        self->tex = self->own_tex;
    } else {
        cols = viewport.v2;
        rows = viewport.v3;
        self->tex = tex;
    }

    vec2 min_units_size = lbwh.zw;
    if (self->cam_units_auto) {
        RCam_min_units_size_set(self->cam, min_units_size);
    }
    RCam_update_ex(self->cam, cols, rows);


    // update projection and viewport
    RCam_apply_proj(self->cam, self->tex);
    if (!self->own_tex) {
        RTex_viewport_set(tex, self->viewport);
    }

    // set current AView in app (for pointers, etc.)
    void a_app__view_set(oobj opt_view);
    oobj opt_prev_view = a_app_view_try();
    a_app__view_set(self);

    //
    // call setup + update with configs set
    //

    double dt = a_app_dt();
    self->time += dt;
    
    self->in_update = true;
    
    // back to front
    for(osize i=o_num(self->layers)-1; i>=0; i--) {
        struct AView_layer *layer = o_at(self->layers, i);
        self->current_layer = i;
        if(layer->opt_setup) {
            layer->opt_setup(self);
            layer->opt_setup = NULL;
        }
        layer->update(self, self->tex, (float) dt);
    }
    self->current_layer = -1;
    self->in_update = false;
    
    //
    // reset
    //
    a_app__view_set(opt_prev_view);
    if (!self->own_tex) {
        RTex_viewport_set(tex, tex_viewport);
        *RTex_proj(self->tex) = tex_proj;
    }
}

void AView_render(oobj obj, oobj tex)
{
    OObj_assert(obj, AView);
    AView *self = obj;

    ivec4 tex_viewport = RTex_viewport(tex);
    struct r_proj tex_proj = *RTex_proj(tex);

    // update projection and viewport
    RCam_apply_proj(self->cam, self->tex);
    if (!self->own_tex) {
        RTex_viewport_set(tex, self->viewport);
    }

    // set current AView in app (for pointers, etc.)
    void a_app__view_set(oobj opt_view);
    oobj opt_prev_view = a_app_view_try();
    a_app__view_set(self);

    //
    // call render with configs set
    //

    double dt = a_app_dt();
    
    self->in_render = true;
    
    // front to back
    for(osize i=0; i<o_num(self->layers); i++) {
        struct AView_layer *layer = o_at(self->layers, i);
        self->current_layer = i;
        layer->render(self, self->tex, (float) dt);
    }
    self->current_layer = -1;
    self->in_render = false;
    
    //
    // reset
    //
    a_app__view_set(opt_prev_view);
    if (!self->own_tex) {
        RTex_viewport_set(tex, tex_viewport);
        *RTex_proj(self->tex) = tex_proj;
    }

    //
    // render own_tex to tex
    //
    if (self->render_auto) {
        // render the view onto the scene
        AView_render_tex(self, tex);
    }
}

void AView_render_tex(oobj obj, oobj tex)
{
    OObj_assert(obj, AView);
    AView *self = obj;
    if (self->use_own_tex) {
        ivec4 tex_viewport = RTex_viewport(tex);
        struct r_proj tex_proj = *RTex_proj(tex);

        RTex_viewport_set(tex, self->viewport);
        // size of proj may differ, to enable different scalings
        *RTex_proj(tex) = r_proj_new(RTex_size_int(self->own_tex), vec2_(-1), true);
        RTex_blit(tex, self->own_tex, 0, 0);

        RTex_viewport_set(tex, tex_viewport);
        *RTex_proj(tex) = tex_proj;
    }
}

struct deffered_data {
    oobj view;
    struct AView_layer layer;
    bool back;
};

O_STATIC
void deferred_fn(oobj defer)
{
    struct deffered_data *data = o_user(defer);
    assert(data);
    if(data->back) {
        OArray_push(AView_layers(data->view), &data->layer);
    } else {
        OArray_push_front(AView_layers(data->view), &data->layer);
    }
}

void AView_push_layer_deferred(oobj obj, struct AView_layer layer, bool back)
{
    OObj_assert(obj, AView);
    AView *self = obj;

    oobj defer = ADefer_new(self, deferred_fn);
    struct deffered_data *data = o_new0(defer, *data, 1);
    data->view = self;
    data->layer = layer;
    data->back = back;
    o_user_set(defer, data);
}


struct a_pointer AView_pointer_raw(oobj obj, int idx, int history)
{

    OObj_assert(obj, AView);
    AView *self = obj;

    // viewport pointer to raw [-1.0 : +1.0]
    struct a_pointer p = AView_pointer_viewport(obj, idx, history);
    p.pos.x = ((p.pos.x - (float) self->viewport.x) * 2.0f / (float) self->viewport.v2) - 1.0f;
    p.pos.y = ((p.pos.y - (float) self->viewport.y) * 2.0f / (float) self->viewport.v3) - 1.0f;

    // bounds check
    bool in_bounds = -1.0f <= p.pos.x && p.pos.x <= +1.0f && -1.0f <= p.pos.y && p.pos.y <= +1.0f;
    p.active &= in_bounds;

    return p;
}

struct a_pointer AView_pointer_p(oobj obj, int idx, int history)
{
    struct a_pointer p = AView_pointer_raw(obj, idx, history);
    p.pos = mat4_mul_vec(*RCam_cam_p_inv(AView_cam(obj)), p.pos);
    return p;
}

struct a_pointer AView_pointer(oobj obj, int idx, int history)
{
    struct a_pointer p = AView_pointer_raw(obj, idx, history);
    p.pos = mat4_mul_vec(*RCam_cam_inv(AView_cam(obj)), p.pos);
    return p;
}
