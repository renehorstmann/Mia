#include "mp/views.h"
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
#include "mp/log.h"
#include "mp/brush.h"
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

#define RES_DIR "res/mp/pattern/"

struct vpat_context {
    oobj tex;
    vec4 bg_color;
    oobj ro_back;
    oobj ro_front;
    oobj ro_pattern;

    // OArray of RTex
    oobj patterns;
    
    struct u_atlas atlas;

    struct a_pointer pointer_prev;
    vec2 pointer_start_pos;
    bool pointer_scrolling;
    vec2 pointer_scrolling_start;
    float pointer_scrolling_start_cam;
    int pointer_clicked_pattern;

    float cam_pos;
    float cam_max;

    int selected_pattern;
};

O_STATIC
void use_pattern(oobj view, int idx)
{
    struct vpat_context *C = o_user(view);
    o_log_s("views_pattern", "pattern clicked: %i", idx);
    mp_brush_pattern_set(*OArray_at(C->patterns, idx, oobj), view);
    C->selected_pattern = idx;
}



O_STATIC
void quad_pose_set(oobj view, int idx, struct r_quad *q)
{
    struct context *C = o_user(view);

    oobj cam = a_app_cam();

    vec2 lt = vec2_(RCam_left(cam), RCam_top(cam));
    vec2 size = RCam_size(cam);
    
    struct u_grid grid = u_grid_new_lt(lt, size, vec2_(ITEM_SIZE), vec2_(0), true);
    vec2 pos = u_grid_at(grid, 1+idx);
    u_pose_set_xy(&q->pose, m_2(pos));
}


O_STATIC
void vpat_pose(oobj view)
{
    struct vpat_context *C = o_user(view);
    
    for(int i=0; i<o_num(C->patterns); i++) {
        quad_pose_set(view, i, o_at(C->ro_back, i));
        quad_pose_set(view, i, o_at(C->ro_front, i));
    }
}

O_STATIC
void vpat_pointer(oobj view)
{
    struct vpat_context *C = o_user(view);

    struct a_pointer p = a_pointer(0, 0);

    if(a_pointer_down(p, C->pointer_prev)) {
        bool gotcha = false;
        for(int i=0; i<o_num(C->patterns); i++) {
            struct r_quad *q = o_at(C->ro_front, i);
            if(u_pose_aa_contains(q->pose, p.pos.xy)) {
                C->pointer_clicked_pattern = i+1;
                C->pointer_start_pos = p.pos.xy;
                gotcha = true;
                break;
            }
        }
        if(!gotcha) {
            C->pointer_clicked_pattern = 0;
        }
    }

    if(p.active) {
//        a_pointer_handled(0);

        vec2 diff = vec2_sub_v(p.pos.xy, C->pointer_start_pos);
        float dist = vec2_length(diff);
        struct a_pointer pp = a_pointer_p(0, 0);
        if(C->pointer_clicked_pattern>0 && dist > CLICK_DIST) {
            C->pointer_clicked_pattern = 0;
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

    if(a_pointer_up(p, C->pointer_prev) && C->pointer_clicked_pattern>0) {
        if(vec2_distance(C->pointer_start_pos, p.pos.xy) <= CLICK_DIST) {
            use_pattern(view, C->pointer_clicked_pattern-1);
        }
    }

    if(!p.active) {
        C->pointer_clicked_pattern = 0;
        C->pointer_scrolling = false;
    }

    struct a_pointer hover = a_pointer_p( 0, 0);
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
void vpat_cam(oobj view)
{
    struct vpat_context *C = o_user(view);

    oobj cam = a_app_cam();

    struct r_quad *last = o_at(C->ro_front, o_num(C->patterns)-1);
    C->cam_max = RCam_bottom(cam)
            - u_pose_aa_get_bottom(last->pose);
    C->cam_max = o_max(0, C->cam_max);
    C->cam_pos = o_clamp(C->cam_pos, 0, C->cam_max);
    float cam_scroll = C->cam_pos;
    cam_scroll = m_round(C->cam_pos / SCROLL_STEPS) * SCROLL_STEPS;
    RCam_pos_set(cam, vec2_(0, -cam_scroll), true);

    *RTex_proj(AView_tex(view)) = RCam_proj(cam);
}

O_STATIC
void vpat_select(oobj view)
{
    struct vpat_context *C = o_user(view);
    
    int select = -1;
    if(mp_brush_pattern_origin() == view) {
        select = C->selected_pattern;
    }
    
    for(int i=0; i<o_num(C->patterns); i++) {
        struct r_quad *q = o_at(C->ro_front, i);
        if(i==select) {
            q->uv = u_atlas_uv(C->atlas, 3);
        } else if(i==C->pointer_clicked_pattern-1) {
            q->uv = u_atlas_uv(C->atlas, 2);
        } else {
            q->uv = u_atlas_uv(C->atlas, 1);
        }
    }
}

O_STATIC
void vpat_setup(oobj view)
{
    struct vpat_context *C = o_user(view);

    struct o_img img = o_img_new_file(view, RES_DIR "border.png");
    // last pixel is part of the background
    C->bg_color = u_color_to_vec4(*(bvec4*)o_img_at_idx(img, img.cols*img.rows-1));
    C->tex = RTex_new(view, img.rgba_data, img.cols, img.rows);
    o_img_free(&img);

    C->patterns = OArray_new(view, NULL, sizeof(oobj), 0);
    
    // load patterns
    const char *pattern_files[] = {
            RES_DIR "white.png",
            RES_DIR "dither.png",
            RES_DIR "dither_inv.png",
            RES_DIR "dither_h.png",
            RES_DIR "dither_h_inv.png",
            RES_DIR "dither_v.png",
            RES_DIR "dither_v_inv.png",
    };
    int pattern_files_num = sizeof(pattern_files) / sizeof(*pattern_files);
    C->ro_back = RObjQuad_new(view, pattern_files_num, C->tex, false);
    C->ro_front = RObjQuad_new(view, pattern_files_num, C->tex, false);
    C->ro_pattern = RObjQuad_new(view, 1, NULL, false);

    C->atlas = u_atlas_new_fill(view, vec2_(ITEM_SIZE), 2, 2, u_atlas_START_LT, true);

    struct r_quad def_bg = r_quad_new(ITEM_SIZE, ITEM_SIZE);
    struct r_quad def_col = r_quad_new(ITEM_SIZE, ITEM_SIZE);
    def_bg.uv = u_atlas_uv( C->atlas, 0);
    def_col.uv = u_atlas_uv( C->atlas, 1);
    
    for(int i=0; i<pattern_files_num; i++) {
        oobj pat = RTex_new_file(view, pattern_files[i]);
        RTex_wrap_set(pat, RTex_wrap_REPEAT);
        OArray_push(C->patterns, &pat);
        
        struct r_quad *q_back = o_at(C->ro_back, i);
        struct r_quad *q_front = o_at(C->ro_front, i);
        
        *q_back = def_bg;
        *q_front = def_col;
    }
    
}


O_STATIC
void vpat_update(oobj view, oobj tex, float dt)
{
    struct vpat_context *C = o_user(view);

    vpat_pose(view);
    vpat_pointer(view);
    vpat_cam(view);
    vpat_select(view);
}

O_STATIC
void vpat_render(oobj view, oobj tex, float dt)
{
    struct vpat_context *C = o_user(view);

    RTex_clear(tex, C->bg_color);
    RTex_ro(tex, C->ro_back);

    for(int i=0; i<o_num(C->patterns); i++) {
        oobj *tex_ref = o_at(C->patterns, i);
        RObjQuad_tex_set(C->ro_pattern, *tex_ref, false);
        struct r_quad *q = o_at(C->ro_pattern, 0);
        q->pose = u_pose_new(0, 0, ITEM_SIZE, ITEM_SIZE);
        quad_pose_set(view, i, q);
        q->uv = u_pose_new(0, 0, m_2(RTex_size(*tex_ref)));
        RTex_ro(tex, C->ro_pattern);
    }

    RTex_ro(tex, C->ro_front);

}

//
// public
//

MPView *mp_views_new_pattern(oobj parent)
{
    MPView *self = MPView_new(parent, vpat_setup, vpat_update, vpat_render, "PAT");
    o_user_set(self, o_new0(self, struct vpat_context, 1));
    return self;
}
