#include "r/RCam.h"
#include "o/OObj_builder.h"
#include "m/mat/mat4.h"
#include "m/vec/ivec2.h"
#include "r/RTex.h"


// new pose with a rotation
O_STATIC
mat4 pose_new_angle(float x, float y, float w, float h, float angle_rad)
{
    // mat4 has column major order
    return mat4_new(
        cosf(angle_rad) * w, sinf(angle_rad) * w, 0, 0,
        -sinf(angle_rad) * h, cosf(angle_rad) * h, 0, 0,
        0, 0, 1, 0,
        x, y, 0, 1
    );
}

//
// public
//


void RCam_matrices_init(struct RCam_matrices* self)
{
    self->p = mat4_eye();
    self->p_inv = mat4_eye();

    self->v = mat4_eye();
    self->v_inv = mat4_eye();

    self->vp = mat4_eye();
    self->v_p_inv = mat4_eye();
}

void RCam_matrices_update(struct RCam_matrices* self)
{
    self->p_inv = mat4_inv(self->p);
    self->v_inv = mat4_inv(self->v);
    self->vp = mat4_mul_mat(self->p, self->v_inv);
    self->v_p_inv = mat4_mul_mat(self->v, self->p_inv);
}


RCam* RCam_init(oobj obj, oobj parent, bool pixelperfect, vec2 min_units_size)
{
    RCam* self = obj;
    o_clear(self, sizeof *self, 1);

    OObj_init(self, parent);
    OObj_id_set(self, RCam_ID);

    self->pixelperfect_scale = self->pixelperfect_view = pixelperfect;
    self->min_units_size = min_units_size;
    self->proj.scale = 1;
    self->zoom = 1;

    RCam_matrices_init(&self->cam);

    // vfuncs
    self->v_update = RCam__v_update;

    return self;
}


void RCam__v_update(oobj obj, int back_cols, int back_rows)
{
    OObj_assert(obj, RCam);
    RCam* self = obj;

    self->back_size = ivec2_(back_cols, back_rows);


    self->proj = r_proj_new(self->back_size, self->min_units_size, self->pixelperfect_scale);
    self->cam.p = self->proj.cam;


    self->cam.v = pose_new_angle(m_2(self->pos), self->zoom, self->zoom, self->rotation);


    RCam_matrices_update(&self->cam);

    // update cam
    self->proj.cam = self->cam.vp;
}


vec2 RCam_pos_set(oobj obj, vec2 pos, bool update)
{
    OObj_assert(obj, RCam);
    RCam* self = obj;
    if (self->pixelperfect_view) {
        pos = r_proj_round_real_pixel(&self->proj, m_2(pos));
    }
    self->pos = pos;
    if (update) {
        RCam_update(self);
    }
    return self->pos;
}

float RCam_zoom_set(oobj obj, float zoom, bool update)
{
    OObj_assert(obj, RCam);
    RCam* self = obj;
    assert(zoom>0);
    if (self->pixelperfect_view) {
        if (zoom > 1.0) {
            zoom = m_round(zoom);
        }
        else {
            zoom = 1.0f / m_round(1.0f / zoom);
        }
    }
    self->zoom = zoom;
    if (update) {
        RCam_update(self);
    }
    return self->zoom;
}


float RCam_rotation_set(oobj obj, float rotation_rad, bool update)
{
    OObj_assert(obj, RCam);
    RCam* self = obj;
    if (self->pixelperfect_view) {
        rotation_rad = m_round(rotation_rad * m_2_PI) * m_PI_2;
    }
    self->rotation = rotation_rad;
    if (update) {
        RCam_update(self);
    }
    return self->rotation;
}

void RCam_apply_proj(oobj obj, oobj tex)
{
    *RTex_proj(tex) = RCam_proj(obj);
}

void RCam_apply_proj_p(oobj obj, oobj tex)
{
    *RTex_proj(tex) = RCam_proj_p(obj);
}
