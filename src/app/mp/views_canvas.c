#include "app/mp/views.h"
#include "r/RCam.h"
#include "r/RObjQuad.h"
#include "u/pose.h"
#include "a/app.h"
#include "a/input.h"
#include "app/mp/log.h"
#include "app/mp/canvas.h"
#include "m/flt.h"

#define MOVE_TIMEOUT 1.5f
#define WHEEL_ZOOM 1.1f

struct vcan_context {
    float move_timeout;
    struct a_pointer pointer0_prev;
    vec2 move_start_pos;
    float move_start_dist;
    bool move;
};


O_STATIC
void vcan_cam_home(oobj view)
{
    struct vcan_context *C = o_user(view);

    oobj cam = a_app_cam();

    vec2 canvas_size = mp_canvas_size();

    vec2 zoom_wh = vec2_div_v(canvas_size, RCam_size(cam));
    float zoom = o_max(zoom_wh.x, zoom_wh.y);
    zoom *= 1.2f;
    RCam_zoom_set(cam, zoom, false);
    RCam_pos_set(cam, vec2_div(canvas_size, 2.0f), true);
}

O_STATIC
void vcan_pointer(oobj view, float dt)
{
    struct vcan_context *C = o_user(view);

    oobj cam = a_app_cam();
    vec2 cam_pos = RCam_pos(cam);
    float cam_zoom = RCam_zoom(cam);

    struct a_pointer p0 = a_pointer_p(0, 0);
    struct a_pointer p1 = a_pointer_p(1, 0);

    if(a_pointer_is_touch()) {

        C->move_timeout-=dt;
        if(!p0.active && !p1.active) {
            C->move_timeout = MOVE_TIMEOUT;
        }

        if((C->move || C->move_timeout>0) && p0.active && p1.active) {
            vec2 pos = vec2_div(vec2_add_v(p0.pos.xy, p1.pos.xy), 2.0f);
            float dist = vec2_distance(p0.pos.xy, p1.pos.xy);
            if(!C->move) {
                C->move = true;
            } else {
                vec2 pos_diff = vec2_sub_v(pos, C->move_start_pos);
                pos_diff = vec2_scale(pos_diff, cam_zoom);
                cam_pos = vec2_sub_v(cam_pos, pos_diff);

                float zoom_factor = dist / C->move_start_dist;
                zoom_factor = o_clamp(zoom_factor, 1.0f/3.0f, 3.0f);
                cam_zoom /= zoom_factor;
            }
            C->move_start_pos = pos;
            C->move_start_dist = dist;
        } else {
            C->move = false;
        }
    } else {

        // mouse wheel button
        if(p1.active) {
            vec2 pos = p1.pos.xy;
            if(!C->move) {
                C->move = true;
            } else {
                vec2 pos_diff = vec2_sub_v(pos, C->move_start_pos);
                pos_diff = vec2_scale(pos_diff, cam_zoom);
                cam_pos = vec2_sub_v(cam_pos, pos_diff);
            }
            C->move_start_pos = pos;
        } else {
            C->move = false;
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


    if(!C->move) {
        for(int bt=a_pointer_history_num(0); bt>0; bt--) {
            mp_canvas_pointer(a_pointer(0, bt));
        }
    }


    RCam_zoom_set(cam, cam_zoom, false);
    RCam_pos_set(cam, cam_pos, true);

    C->pointer0_prev = p0;
}


O_STATIC
void vcan_setup(oobj view)
{
    struct vcan_context *C = o_user(view);

    // to allow smooth zooming
    RCam_pixelperfect_view_set(a_app_cam(), false);

    vcan_cam_home(view);
}

O_STATIC
void vcan_update(oobj view, oobj tex, float dt)
{
    struct vcan_context *C = o_user(view);

//    o_log("vp: %i %i %i %i", m_4(AView_viewport(view)));

    vcan_pointer(view, dt);
}

O_STATIC
void vcan_render(oobj view, oobj tex, float dt)
{
    struct vcan_context *C = o_user(view);

//    o_log("vp: %i %i %i %i", m_4(AView_viewport(view)));

    RTex_clear(tex, R_BLACK);
    mp_canvas_render(tex);
}


//
// public
//

MPView *mp_views_new_canvas(oobj parent)
{
    MPView *self = MPView_new(parent, vcan_setup, vcan_update, vcan_render, "CAN");
    AView_use_own_tex_set(self, false);
//    AView_scale_auto_set(self, true);
    o_user_set(self, o_new0(self, struct vcan_context, 1));
    return self;
}

