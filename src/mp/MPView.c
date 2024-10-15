#include "mp/MPView.h"
#include "mp/main.h"
#include "o/OObj_builder.h"
#include "o/str.h"
#include "r/RTex_manip.h"
#include "r/RCam.h"
#include "r/RObjQuad.h"
#include "r/RObjText.h"
#include "r/tex.h"
#include "a/app.h"
#include "u/pose.h"
#include "mp/log.h"


static struct {
    // RObjQuad to create the batches
    oobj ro_batch_chars;
} mpview_L;

static const vec4 SHADOW = {{0.33, 0.33, 0.33, 0.66}};

O_STATIC
osize child_idx(OObj *parent, OObj *child)
{
    // search back, faster in most cases
    for(osize i = parent->children_num-1; i>=0; i--) {
        if(parent->children[i] == child) {
            return i;
        }
    }
    return -1;
}

O_STATIC
RTex *create_icon(oobj parent, const char *name)
{
    assert(o_strlen(name) == 3);

    // lazy init
    if (!mpview_L.ro_batch_chars) {
        mpview_L.ro_batch_chars = RObjQuad_new(mp_root(), 4, RTex_new_file(mp_root(), "res/r/font35.png"), true);

        struct r_quad *q;
        for (int i = 0; i < 3; i++) {
            q = o_at(mpview_L.ro_batch_chars, i);
            float left = 2 + i * 4;
            float top = 4;
            q->pose = u_pose_new_lt(-8 + left, 8 - top, 3, 5);
        }

        // white line -> we use the center of '+' which is a white pixel...
        q = o_at(mpview_L.ro_batch_chars, 3);
        vec2 uv_pos = RObjText__v_font35_char_uv(NULL, '+');
        q->uv = u_pose_new(m_2(uv_pos), 1, 1);
        q->pose = u_pose_new_lt(-8 + 2, 8 - 10, 11, 1);
    }

    oobj container = OObj_new(parent);

    oobj res = RTex_new(container, NULL, 16, 16);
    RTex_clear_full(res, R_TRANSPARENT);


    for (int i = 0; i < 3; i++) {
        struct r_quad *q = o_at(mpview_L.ro_batch_chars, i);
        vec2 uv_pos = RObjText__v_font35_char_uv(NULL, name[i]);
        q->uv = u_pose_new(m_2(uv_pos), 3, 5);
    }

    RTex_ro(res, mpview_L.ro_batch_chars);

    res = RTex_contour(res, r_tex_kernel_block2(false),
                       ivec2_(0, -1), vec4_(0, 0, 0, 1),
                       SHADOW, RTex_format(res));

    RTex_write_file(res, "debug.batch.png");

    RTex_use_done(res);

    o_move(res, parent);
    o_del(container);

    return res;
}


O_STATIC
void proxy_update(oobj view, oobj tex, float dt)
{
    OObj_assert(view, MPView);
    MPView *self = view;


    struct r_quad *icon_q = o_at(self->icon, 0);
    // todo handle icon clicked?

    struct a_pointer pointer = a_pointer_p(0, 0);
    if (u_btn_clicked(&self->icon_btn, pointer, self->prev)) {
        o_log("pressed: %s", self->name);
    }
    self->prev = pointer;

    if (u_pose_aa_contains(icon_q->pose, pointer.pos.xy)) {
        a_pointer_handled(-1, -1);
    }


    // call proxied update
    self->update(view, tex, dt);

}

O_STATIC
void proxy_render(oobj view, oobj tex, float dt)
{
    OObj_assert(view, MPView);
    MPView *self = view;

    struct r_quad *icon_q = o_at(self->icon, 0);
    
    // call proxied render
    self->render(view, tex, dt);

    // render icon
    RCam_apply_proj_p(a_app_cam(), tex);
    u_pose_aa_set_lt(&icon_q->pose, r_proj_left(RTex_proj(tex)), r_proj_top(RTex_proj(tex)));
    RTex_ro(tex, self->icon);

}


MPView *MPView_init(oobj obj, oobj parent,
                    OObj__event_fn opt_setup,
                    AView__fn update,
                    AView__fn render,
                    const char *name)
{
    AView *super = obj;
    MPView *self = obj;
    o_clear(self, sizeof *self, 1);

    // call super init
    AView_init(obj, parent, opt_setup, proxy_update, proxy_render);
    OObj_id_set(self, MPView_ID);

    assert(o_strlen(name) == 3);
    strcpy(self->name, name);

    self->icon = RObjQuad_new_color(self, 1, create_icon(self, name), true);
    self->update = update;
    self->render = render;

    self->icon_btn = u_btn_new_color(self, o_at(self->icon, 0),
                                     vec4_(1, 1, 1, 1), vec4_(0.8, 0.8, 0.8, 1.0));

    return self;
}

