#include "app/mp/views.h"
#include "m/vec/vec2.h"
#include "r/common.h"
#include "r/RCam.h"
#include "r/RObjQuad.h"
#include "u/grid.h"
#include "u/color.h"
#include "u/pose.h"
#include "u/atlas.h"
#include "o/OArray.h"
#include "o/img.h"
#include "app/mp/log.h"
#include "app/mp/brush.h"
#include "a/app.h"
#include "a/input.h"

#include "m/utils/random.h"

/*
 * Plan:
 * top left 16x16 corner is always the icon.+
 * below the top 16x16 bar is the palette color grid.
 *      the grid is scrollable, if too large, but not zoomable
 */


#define ITEM_SIZE 16
#define CLICK_DIST 4
#define SCROLL_STEPS 2
#define WHEEL_STEPS 8

struct vpal_context {
    oobj tex;
    vec4 bg_color;
    oobj ro;


    bvec4 *colors;
    int colors_num;

    struct a_pointer pointer_prev;
    vec2 pointer_start_pos;
    bool pointer_scrolling;
    vec2 pointer_scrolling_start;
    float pointer_scrolling_start_cam;
    int pointer_clicked_color;

    float cam_pos;
    float cam_max;

    int selected_color;
};

O_STATIC
struct r_quad *vpal_quad_bg(oobj ro, int idx)
{
    return RObjQuad_at(ro, idx*2);
}

O_STATIC
struct r_quad *vpal_quad_color(oobj ro, int idx)
{
    return RObjQuad_at(ro, idx*2+1);
}

O_STATIC
struct r_quad *vpal_quad_clicked(oobj ro)
{
    return RObjQuad_at(ro, RObjQuad_num(ro)-2);
}

O_STATIC
struct r_quad *vpal_quad_select(oobj ro)
{
    return RObjQuad_at(ro, RObjQuad_num(ro)-1);
}

O_STATIC
void vpal_colors_set(oobj view, const bvec4 *colors, int num)
{
    struct vpal_context *C = o_user(view);
    C->colors = o_renew(view, C->colors, bvec4, num);
    o_memcpy(C->colors, colors, sizeof(bvec4), num);
    C->colors_num = num;

    struct u_atlas atlas = u_atlas_new_fill(view, vec2_(ITEM_SIZE), 2, 2, u_atlas_START_LT, true);

    struct r_quad def_bg = r_quad_new(ITEM_SIZE, ITEM_SIZE);
    struct r_quad def_col = r_quad_new(ITEM_SIZE, ITEM_SIZE);
    def_bg.uv = u_atlas_uv(atlas, 0);
    def_col.uv = u_atlas_uv(atlas, 1);

    oobj quads = RObjQuad_quads(C->ro);

    // num * (bg + color) + select
    OArray_resize(quads, num*2+2);
    for(int i=0; i<num; i++) {
        struct r_quad *q_a = vpal_quad_bg(C->ro, i);
        struct r_quad *q_b = vpal_quad_color(C->ro, i);
        *q_a = def_bg;
        *q_b = def_col;
        q_b->s = u_color_to_vec4(C->colors[i]);
    }

    struct r_quad *q_clicked = vpal_quad_clicked(C->ro);
    *q_clicked = def_bg;
    q_clicked->uv = u_atlas_uv(atlas, 2);

    struct r_quad *q_select = vpal_quad_select(C->ro);
    *q_select = def_bg;
    q_select->uv = u_atlas_uv(atlas, 3);

    u_atlas_free(&atlas);
}

O_STATIC
void vpal_use_color(oobj view, int idx)
{
    struct vpal_context *C = o_user(view);
    o_log_s("views_palette", "color clicked: %i", idx);
    mp_brush_color_set(C->colors[idx], view);
    C->selected_color = idx;
}

O_STATIC
void vpal_pose(oobj view)
{
    struct vpal_context *C = o_user(view);

    oobj cam = a_app_cam();

    vec2 lt = vec2_(RCam_left(cam), RCam_top(cam));
    vec2 size = RCam_size(cam);
    
    struct u_grid grid = u_grid_new_lt(lt, size, vec2_(ITEM_SIZE), vec2_(0), true);

    for(int i=0; i<C->colors_num; i++) {
        struct r_quad *q_a = vpal_quad_bg(C->ro, i);
        struct r_quad *q_b = vpal_quad_color(C->ro, i);
        vec2 pos = u_grid_at(grid, 1+i);
        u_pose_set_xy(&q_a->pose, m_2(pos));
        q_b->pose = q_a->pose;
    }
}

O_STATIC
void vpal_pointer(oobj view)
{
    struct vpal_context *C = o_user(view);

    struct a_pointer p = a_pointer(0, 0);

    if(a_pointer_down(p, C->pointer_prev)) {
        bool gotcha = false;
        for(int i=0; i<C->colors_num; i++) {
            struct r_quad *q = vpal_quad_color(C->ro, i);
            if(u_pose_aa_contains(q->pose, p.pos.xy)) {
                C->pointer_clicked_color = i+1;
                C->pointer_start_pos = p.pos.xy;
                gotcha = true;
                break;
            }
        }
        if(!gotcha) {
            C->pointer_clicked_color = 0;
        }
    }

    if(p.active) {
//        a_pointer_handled(0);

        vec2 diff = vec2_sub_v(p.pos.xy, C->pointer_start_pos);
        float dist = vec2_length(diff);
        struct a_pointer pp = a_pointer_p(0, 0);
        if(C->pointer_clicked_color>0 && dist > CLICK_DIST) {
            C->pointer_clicked_color = 0;
        }
        if(!C->pointer_scrolling && dist > CLICK_DIST) {
            C->pointer_scrolling = true;
            C->pointer_scrolling_start = pp.pos.xy;
            C->pointer_scrolling_start_cam = C->cam_pos;
        } else if(C->pointer_scrolling) {
            vec2 diff = vec2_sub_v(pp.pos.xy, C->pointer_scrolling_start);
            
            float scroll = -diff.x + diff.y;
            
            C->cam_pos = C->pointer_scrolling_start_cam + scroll;
        }
    }

    if(a_pointer_up(p, C->pointer_prev) && C->pointer_clicked_color>0) {
        if(vec2_distance(C->pointer_start_pos, p.pos.xy) <= CLICK_DIST) {
            vpal_use_color(view, C->pointer_clicked_color-1);
        }
    }

    if(!p.active) {
        C->pointer_clicked_color = 0;
        C->pointer_scrolling = false;
    }

    struct a_pointer hover = a_pointer_p(0, 0);
    if(u_pose_aa_contains(RCam_pose(a_app_cam(), 0, 0), hover.pos.xy)) {
        // mouse wheel zoom
        if(a_input_wheel().y>0) {
            C->cam_pos -= WHEEL_STEPS;
        } else if(a_input_wheel().y<0) {
            C->cam_pos += WHEEL_STEPS;
        }
    }

    C->pointer_prev = p;
}

O_STATIC
void vpal_cam(oobj view)
{
    struct vpal_context *C = o_user(view);

    oobj cam = a_app_cam();

    C->cam_max = RCam_bottom(cam)
            - u_pose_aa_get_bottom(vpal_quad_color(C->ro, C->colors_num-1)->pose);
    C->cam_max = o_max(0, C->cam_max);
    C->cam_pos = o_clamp(C->cam_pos, 0, C->cam_max);
    float cam_scroll = C->cam_pos;
    cam_scroll = m_round(C->cam_pos / SCROLL_STEPS) * SCROLL_STEPS;
    RCam_pos_set(cam, vec2_(0, -cam_scroll), true);

    *RTex_proj(AView_tex(view)) = RCam_proj(cam);
}

O_STATIC
void vpal_select(oobj view)
{
    struct vpal_context *C = o_user(view);
    if(C->pointer_clicked_color>0) {
        vpal_quad_clicked(C->ro)->pose = vpal_quad_color(C->ro, C->pointer_clicked_color-1)->pose;
    } else {
        u_pose_set_hidden(&vpal_quad_clicked(C->ro)->pose);
    }

    if(mp_brush_color_origin() == view) {
        vpal_quad_select(C->ro)->pose = vpal_quad_color(C->ro, C->selected_color)->pose;
        RObjQuad_num_rendered_set(C->ro, RObjQuad_num(C->ro));
    } else {
        RObjQuad_num_rendered_set(C->ro, RObjQuad_num(C->ro)-1);
    }
}

O_STATIC
void vpal_setup(oobj view)
{
    struct vpal_context *C = o_user(view);

    struct o_img img = o_img_new_file(view, "res/mp/palette/color.png");
    // second pixel is part of the background
    C->bg_color = u_color_to_vec4(*(bvec4*)o_img_at_idx(img, 1));
    C->tex = RTex_new(view, img.rgba_data, img.cols, img.rows);
    o_img_free(&img);

    C->ro = RObjQuad_new_color(view, 1, C->tex, true);


    {
        struct o_img pal = o_img_new_file(view, "res/mp/palette/pixilmatt.png");
        vpal_colors_set(view, (bvec4*) pal.rgba_data, pal.cols * pal.rows);
        o_img_free(&pal);
    }

    vpal_use_color(view, 1);
}


O_STATIC
void vpal_update(oobj view, oobj tex, float dt)
{
    struct vpal_context *C = o_user(view);

    vpal_pose(view);
    vpal_pointer(view);
    vpal_cam(view);
    vpal_select(view);
}

O_STATIC
void vpal_render(oobj view, oobj tex, float dt)
{
    struct vpal_context *C = o_user(view);

    RTex_clear(tex, C->bg_color);
    RObj_render(C->ro, tex);

//    RTex_write_file(tex, "debug.palette.png");
}

//
// public
//

MPView *mp_views_new_palette(oobj parent)
{
    MPView *self = MPView_new(parent, vpal_setup, vpal_update, vpal_render, "PAL");
    AView_use_own_tex_set(self, true);
    o_user_set(self, o_new0(self, struct vpal_context, 1));
    return self;
}
