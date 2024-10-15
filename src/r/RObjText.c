#include "r/RObjText.h"
#include "o/OObj_builder.h"
#include "o/OArray.h"
#include "o/img.h"
#include "m/vec/vec2.h"
#include "m/mat/mat4.h"
#include "r/RObjQuad.h"
#include "r/tex.h"
#include <ctype.h>

#define O_LOG_LIB "r"
#include "o/log.h"

O_INLINE
void pose_set_x(mat4 *p, float x)
{
    p->m30 = x;
}

O_INLINE
void pose_set_y(mat4 *p, float y)
{
    p->m31 = y;
}

O_INLINE
void pose_set_xy(mat4 *p, float x, float y)
{
    pose_set_x(p, x);
    pose_set_y(p, y);
}

O_INLINE
void pose_set_hidden(mat4 *p)
{
    pose_set_xy(p, m_MAX, m_MAX);
}

O_INLINE
void pose_set_size_angle(mat4 *p, float w, float h, float angle_rad)
{
    p->m00 = m_cos(angle_rad) * w / 2;
    p->m01 = m_sin(angle_rad) * w / 2;
    p->m10 = -m_sin(angle_rad) * h / 2;
    p->m11 = m_cos(angle_rad) * h / 2;
}

//
// public
//


RObjText *RObjText_init(oobj obj, oobj parent, int num, oobj tex, bool move_tex, 
        vec2 size, vec2 offset, RObjText__char_uv_fn char_uv)
{
    RObj *super = obj;
    RObjText *self = obj;
    o_clear(self, sizeof *self, 1);

    RObj_init(obj, parent, RObjText__v_update, RObjText__v_render);
    OObj_id_set(self, RObjText_ID);

    self->pose = mat4_eye();
    
    self->size = size;
    self->offset = offset;

    self->text_mode = RObjText_MODE_DEFAULT;
    
    self->quads = RObjQuad_new_color(parent, num, tex, move_tex);
    
    for(int i=0; i<num; i++) {
        struct r_quad *q = RObjQuad_at(self->quads, i);
        pose_set_hidden(&q->pose);
    }
    
    // calls RObj_update(self->quads)
    RObjText_uv_size_set(self, size);
    
    
    // vfuncs
    self->v_char_uv = char_uv;
    

    return self;
}




RObjText *RObjText_new_font35(oobj parent, int num, const char *opt_text)
{
    num = o_max(num, o_strlen(opt_text));

    oobj tex = r_tex_font35();
    RObjText *self = RObjText_new(parent, num, tex, false,
            vec2_(3, 5), vec2_(4, 6), RObjText__v_font35_char_uv);
    self->text_mode = RObjText_MODE_UPPER;
    if(opt_text) {
        RObjText_text_set(self, opt_text);
    }
    return self;
}

RObjText *RObjText_new_font35_shadow(oobj parent, int num, const char *opt_text, bvec4 *opt_shadow_color)
{
    num = o_max(num, o_strlen(opt_text));

    struct o_img img = o_img_new_file(parent, "res/r/font35_shadow.png");
    if(opt_shadow_color) {
        for(int i=0; i<img.cols * img.rows; i++) {
            bvec4 *rgba = (bvec4*) o_img_at_idx(img, i);
            if(rgba->a == 128) {
                *rgba = *opt_shadow_color;
            }
        }
    }
    oobj tex = RTex_new(parent, img.rgba_data, img.cols, img.rows);
    o_img_free(&img);
    RObjText *self = RObjText_new(parent, num, tex, true,
                         vec2_(3+2, 5+2), vec2_(4, 6), RObjText__v_font35_char_uv);
    self->text_mode = RObjText_MODE_UPPER;
    if(opt_text) {
        RObjText_text_set(self, opt_text);
    }
    return self;
}

//
// virtual implementations:
//

void RObjText__v_update(oobj obj)
{
    // noop
}

void RObjText__v_render(oobj obj, oobj tex, const struct r_proj *proj)
{
    OObj_assert(obj, RObjText);
    RObjText *self = obj;
    
    mat4 pose = self->pose;
    
    // pose position on real pixel
    pose.m30 = m_round(pose.m30*proj->scale) / proj->scale;
    pose.m31 = m_round(pose.m31*proj->scale) / proj->scale;

    struct r_proj combined = *proj;
    combined.cam = mat4_mul_mat(proj->cam, pose);

    RObj_render_ex(self->quads, tex, &combined, false);
}


vec2 RObjText__v_font35_char_uv(oobj obj, char c)
{
    const int columns = 16;
    const int rows = 6;
    const float top = rows * 8;
    const float left = -columns * 8;
    const vec2 base_offset = {{left + 8 + 1.5, top -8 - 2.5}};
    const vec2 stride = {{16, -16}};

    if (c < ' ' || c > '~') {
        c = ' ';
    }
    c -= ' ';
    int col = c % columns;
    int row = c / columns;
    return vec2_add_scaled_v(base_offset,
                             vec2_(col, row), stride);
}

//
// object functions:
//

void RObjText_uv_size_set(oobj obj, vec2 size)
{
    OObj_assert(obj, RObjText);
    RObjText *self = obj;
    for(int i=0; i < RObjQuad_num(self->quads); i++) {
        struct r_quad *q = RObjQuad_at(self->quads, i);
        pose_set_size_angle(&q->pose, m_2(size), 0);
        pose_set_size_angle(&q->uv, m_2(size), 0);
    }
    
    RObj_update(self->quads);
}

vec2 RObjText_text_set(oobj obj, const char *text)
{
    OObj_assert(obj, RObjText);
    RObjText *self = obj;
    
    int i = 0;
    int col = 0;
    int row = 0;
    int cols = 0;
    while (*text && i < RObjQuad_num(self->quads)) {
        struct r_quad *q = RObjQuad_at(self->quads, i);

        char c = *text;
        if(self->text_mode == RObjText_MODE_UPPER) {
            c = toupper(c);
        } else if(self->text_mode == RObjText_MODE_LOWER) {
            c = tolower(c);
        }
        
        vec2 uv = self->v_char_uv(self, c);
        
        pose_set_xy(&q->uv, m_2(uv));
        
        pose_set_xy(&q->pose,
                self->size.x/2 + col * self->offset.x,
                -self->size.y/2 - row * self->offset.y);
        
        col++;
        if (*text == '\n') {
            col = 0;
            row++;
        }
        cols = o_max(cols, col);
        text++;
        i++;
    }

    // set maximal rendered
    RObjQuad_num_rendered_set(self->quads, i);

    // no need to hide, not rendered anymore... (see above call)
//    // hide remaining quads
//    for(; i < RObjQuad_num(self->quads); i++) {
//        struct r_quad *q = RObjQuad_at(self->quads, i);
//        pose_set_hidden(&q->pose);
//    }
    
    RObj_update(self->quads);

    
    if (cols == 0) {
        self->text_size = vec2_(0);
    } else {
        self->text_size = vec2_(
                (cols - 1) * self->offset.x + self->size.x,
                row * self->offset.y + self->size.y
        );
    }
    
    return self->text_size;
}

void RObjText_color_set(oobj obj, vec4 color)
{
    OObj_assert(obj, RObjText);
    RObjText *self = obj;
    for(osize i=0; i<o_num(self->quads); i++)
    {
        struct r_quad *q = o_at(self->quads, i);
        q->s = color;
    }
    RObj_update(self->quads);
}

