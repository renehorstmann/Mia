#include "mp/canvas.h"
#include "r/RObjQuad.h"
#include "r/RShaderQuad.h"
#include "r/RShaderQuadDab.h"
#include "r/RTex_manip.h"
#include "r/program.h"
#include "u/pose.h"
#include "mp/brush.h"
#include "a/app.h"
#include "a/AView.h"
#include "o/file.h"
#include "o/ODelcallback.h"

#define O_LOG_LIB "mp"
#include "o/log.h"

#define DAB_RADIUS_DIST 0.2

static struct {
    oobj o;
    RTex *img;
    oobj ro;

    oobj brush_ro;
    oobj brush_tex;

    struct a_pointer prev;
    vec2 last_dab;

    float dbl_tap_time;

    RTex *layer;
    RTex *mask;
    RTex *merged;

} canvas_L;


O_STATIC
vec2 lb_to_center(vec2 pos)
{
    return vec2_sub_v(pos, vec2_div(RTex_size(canvas_L.img), 2));
}

// protected, called by mp_main
O_EXTERN
oobj mp_canvas__init(oobj parent)
{
    o_clear(&canvas_L, sizeof canvas_L, 1);

    canvas_L.o = OObj_new(parent);
    canvas_L.img = RTex_new_ex(canvas_L.o, NULL, 256 * 4, 256 * 4, R_FORMAT_RGBA_8, R_FORMAT_RGBA_8);
    RTex_clear(canvas_L.img, R_GRAY);

    canvas_L.merged = RTex_clone(canvas_L.img);

    canvas_L.layer = RTex_clone(canvas_L.img);
    canvas_L.mask = RTex_new_ex(canvas_L.o, NULL, m_2(RTex_size(canvas_L.img)), R_FORMAT_R_8, R_FORMAT_R_8);
    RTex_attach_set(canvas_L.layer, canvas_L.mask);
    RTex_clear(canvas_L.layer, R_TRANSPARENT);
    RTex_clear(canvas_L.mask, R_BLACK);

    canvas_L.ro = RObjQuad_new(canvas_L.o, 1, canvas_L.img, false);
    struct r_quad *q = o_at(canvas_L.ro, 0);
    q->pose = u_pose_new_lb(0, 0, m_2(RTex_size(canvas_L.img)));

    canvas_L.brush_tex = RTex_new(canvas_L.o, NULL, 32, 32);
    RTex_clear_full(canvas_L.brush_tex, R_WHITE);
    
    oobj shader = RShaderQuadDab_new(canvas_L.o, canvas_L.brush_tex, false);
    RShaderQuad_stuv(shader)->col[0].a = 1.0f;
    RShaderQuad_stuv(shader)->col[3] = vec4_(1);
    canvas_L.brush_ro = RObjQuad_new_shader(canvas_L.o, 64, canvas_L.brush_tex, false, shader, false);

    return canvas_L.o;
}

void mp_canvas_render(oobj tex)
{

    // not full, as we just want to clear the active viewport here
    RTex_clear(tex, R_GREEN);

    RTex_merge_into(canvas_L.img, canvas_L.layer, canvas_L.mask, NULL, R_WHITE, canvas_L.merged);
    //RTex_filter_set(merged, RTex_filter_LINEAR);
    RObjQuad_tex_set(canvas_L.ro, canvas_L.merged, false);
    RObj_render(canvas_L.ro, tex);

    if(canvas_L.dbl_tap_time > 0) {
        canvas_L.dbl_tap_time -= a_app_dt();
    }
}

O_STATIC
void set_quad(struct r_quad *q, vec2 pos, float pressure)
{
    vec4 rgba = vec4_cast_byte_1(mp_brush_color().v);
    float hardening = m_mix(0.2f, 0.6f, pressure);
    float opacity = m_mix(0.25, 1.0, pressure);
    float size_scale = m_mix(1.0, 1.33, pressure);

//    hardening = 1;
//    size_scale *= 3;

    vec2 sprite_size = RTex_size(canvas_L.brush_tex);
    sprite_size = vec2_scale(sprite_size, size_scale);
    q->s = rgba;
    q->v.x = hardening;
    q->v.y = opacity;
    u_pose_set(&q->pose, m_2(pos), m_2(sprite_size), 0);
}

#include "r/gl.h"
void mp_canvas_pointer(struct a_pointer pointer)
{

    RShaderQuadDab_pattern_set(RObjQuad_shader(canvas_L.brush_ro, 0), mp_brush_pattern());

    if(a_pointer_down(pointer, canvas_L.prev)) {
        vec2 cpos = lb_to_center(pointer.pos.xy);
        canvas_L.last_dab = cpos;
        set_quad(o_at(canvas_L.brush_ro, 0), cpos, pointer.pressure);
        RObjQuad_num_rendered_set(canvas_L.brush_ro, 1);
        RTex_ro(canvas_L.layer, canvas_L.brush_ro);

        if(canvas_L.dbl_tap_time > 0) {
                o_log("download by dbl tap!");
                RTex_write_file(canvas_L.img, "image.png");
                o_file_download("image.png", NULL, NULL);
        }
        canvas_L.dbl_tap_time = 0.5;
    } else if(pointer.active) {
        // move
        vec2 cpos = lb_to_center(pointer.pos.xy);
        float dist = vec2_distance(cpos, canvas_L.last_dab);
        float dab_dist = DAB_RADIUS_DIST * o_max(RTex_size(canvas_L.brush_tex).x, RTex_size(canvas_L.brush_tex).y);
        int dabs = (int) m_floor(dist / dab_dist);
        dabs = o_min(dabs, o_num(canvas_L.brush_ro));
        if(dabs>=1) {
            vec2 new_last;
            for(int i=0; i<dabs; i++) {
                float t = (float) (i+1) * dab_dist / dist;
                vec2 pos = vec2_mix(canvas_L.last_dab, cpos, t);
                new_last = pos;
                set_quad(o_at(canvas_L.brush_ro, i), pos, pointer.pressure);
            }
            canvas_L.last_dab = new_last;
            RObjQuad_num_rendered_set(canvas_L.brush_ro, dabs);
            RTex_ro(canvas_L.layer, canvas_L.brush_ro);
        }
    } else if(a_pointer_up(pointer, canvas_L.prev)) {
        RTex_merge_into(canvas_L.img, canvas_L.layer, canvas_L.mask, NULL, R_WHITE, canvas_L.merged);
        RTex_blit(canvas_L.img, canvas_L.merged, 0, 0);
        RTex_clear_full(canvas_L.layer, R_TRANSPARENT);
        RTex_clear_full(canvas_L.mask, R_BLACK);
    }

    canvas_L.prev = pointer;
}

ivec2 mp_canvas_size_int(void)
{
    return RTex_size_int(canvas_L.img);
}
