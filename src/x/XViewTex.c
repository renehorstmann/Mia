#include "x/XViewTex.h"
#include "o/OObj_builder.h"
#include "r/RObjQuad.h"
#include "r/RObjText.h"
#include "r/RCam.h"
#include "a/app.h"
#include "a/input.h"
#include "u/pose.h"
#include "w/WTheme.h"
#include "w/WBox.h"
#include "w/WBtn.h"
#include "w/WIcon.h"
#include "w/WTextShadow.h"

#define MOVE_TIMEOUT 1.5f
#define WHEEL_ZOOM 1.1f

#define O_LOG_LIB "x"
#include "o/log.h"


O_STATIC
void cam_home(XViewTex *self)
{
    oobj cam = a_app_cam();

    vec2 canvas_size = RTex_size(XViewTex_tex(self));

    vec2 zoom_wh = vec2_div_v(canvas_size, RCam_size(cam));
    float zoom = o_max(zoom_wh.x, zoom_wh.y);
    zoom *= 1.4f;
    RCam_zoom_set(cam, zoom, false);
    RCam_pos_set(cam, vec2_div(canvas_size, 2.0f), true);
}

O_STATIC
void pointer(XViewTex *self, float dt)
{
    oobj cam = a_app_cam();
    vec2 cam_pos = RCam_pos(cam);
    float cam_zoom = RCam_zoom(cam);

    struct a_pointer p0 = a_pointer_p(0, 0);
    struct a_pointer p1 = a_pointer_p(1, 0);


    if(a_pointer_is_touch()) {

        self->move_timeout-=dt;
        if(!p0.active && !p1.active) {
            self->move_timeout = MOVE_TIMEOUT;
        }

        if((self->move || self->move_timeout>0) && p0.active && p1.active) {
            vec2 pos = vec2_div(vec2_add_v(p0.pos.xy, p1.pos.xy), 2.0f);
            float dist = vec2_distance(p0.pos.xy, p1.pos.xy);
            if(!self->move) {
                self->move = true;
            } else {
                vec2 pos_diff = vec2_sub_v(pos, self->move_start_pos);
                pos_diff = vec2_scale(pos_diff, cam_zoom);
                cam_pos = vec2_sub_v(cam_pos, pos_diff);

                float zoom_factor = dist / self->move_start_dist;
                zoom_factor = o_clamp(zoom_factor, 1.0f/3.0f, 3.0f);
                cam_zoom /= zoom_factor;
            }
            self->move_start_pos = pos;
            self->move_start_dist = dist;
        } else {
            self->move = false;
        }
    } else {

        // mouse wheel button
        if(p1.active) {
            vec2 pos = p1.pos.xy;
            if(!self->move) {
                self->move = true;
            } else {
                vec2 pos_diff = vec2_sub_v(pos, self->move_start_pos);
                pos_diff = vec2_scale(pos_diff, cam_zoom);
                cam_pos = vec2_sub_v(cam_pos, pos_diff);
            }
            self->move_start_pos = pos;
        } else {
            self->move = false;
        }

        struct a_pointer hover = a_pointer_p(0, 0);
        if(u_pose_aa_contains(RCam_pose(cam, 0, 0), hover.pos.xy)) {
            // mouse wheel zoom
            if(a_input_wheel().y>0) {
                cam_zoom /= WHEEL_ZOOM;
            } else if(a_input_wheel().y<0) {
                cam_zoom *= WHEEL_ZOOM;
            }
        }

    }


    if(!self->move) {
        if(self->v_opt_pointer) {
            self->v_opt_pointer(self, a_pointer(0, 0));
        }
    }


    RCam_zoom_set(cam, cam_zoom, false);
    RCam_pos_set(cam, cam_pos, true);

    self->pointer0_prev = p0;
}


//
// public
//

XViewTex *XViewTex_init(oobj obj, oobj parent, oobj tex, bool move_tex, OObj__event_fn done,
                        const char *opt_title, XViewTex_pointer_fn opt_pointer)
{

    AView *super = obj;
    XViewTex *self = obj;
    o_clear(self, sizeof *self, 1);

    AView_init(obj, parent, XViewTex__v_setup, XViewTex__v_update, XViewTex__v_render);
    OObj_id_set(self, XViewTex_ID);

    self->ro = RObjQuad_new(self, 1, tex, move_tex);

    self->theme = WTheme_new_tiny(self);
    self->gui = WBox_new(self, WBox_LAYOUT_V);

    oobj btn_title_box = WBox_new(self->gui, WBox_LAYOUT_H);

    self->exit_btn = WBtn_new(btn_title_box);
    oobj btn_icon = WIcon_new(self->exit_btn, WTheme_ICON_CROSS);
    WIcon_color_set(btn_icon, vec4_(0.8, 0.1, 0.1, 1.0));

    self->title_text = WTextShadow_new(btn_title_box, opt_title);
    WObj_padding_set(self->title_text, vec4_(2, 4, 0));
    
    // vfuncs
    self->v_done = done;
    self->v_opt_pointer = opt_pointer;

    return self;
    
}


//
// virtual implementations
//

void XViewTex__v_setup(oobj view)
{
    // to allow smooth zooming
    RCam_pixelperfect_view_set(a_app_cam(), false);

    cam_home(view);
}

void XViewTex__v_update(oobj view, oobj tex, float dt)
{
    OObj_assert(view, XViewTex);
    XViewTex *self = view;

    oobj t = XViewTex_tex(self);
    struct r_quad *q = o_at(self->ro, 0);
    q->pose = u_pose_new_lb(0, 0, m_2(RTex_size(t)));
    q->uv = u_pose_new(0, 0, m_2(RTex_size(t)));

    pointer(self, dt);

    WTheme_update(self->theme, self->gui, vec2_(0,-1), vec2_(0), a_pointer);

    if(WBtn_clicked(self->exit_btn)) {
        self->v_done(self);
    }
}

void XViewTex__v_render(oobj view, oobj tex, float dt)
{
    OObj_assert(view, XViewTex);
    XViewTex *self = view;

    RTex_clear(tex, R_BLACK);

    RObj_render(self->ro, tex);
    
    WTheme_render(self->theme, tex);
    
}

//
// object functions:
//

RTex *XViewTex_tex(oobj obj)
{
    return RObjQuad_tex(XViewTex_ro(obj));
}

RTex *XViewTex_tex_set(oobj obj, oobj tex, bool del_old)
{
    return RObjQuad_tex_set(XViewTex_ro(obj), tex, del_old);
}
