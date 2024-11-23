#ifndef R_RCAM_H
#define R_RCAM_H

/**
 * @file RCam.h
 *
 * Object
 *
 * A Camera for the render objects.
 * Origin is centered.
 * +x -> to the right
 * +y -> to the top
 */


#include "o/OObj.h"
#include "r/common.h"
#include "r/proj.h"
#include "m/sca/flt.h"

/** object id */
#define RCam_ID OObj_ID "RCam"

struct RCam_matrices
{
    // projection of the camera (perspective / orthogonal)
    // pass 'p' to a render object,
    //     to draw it at a static position on the screen (HUD)
    // use 'p_inv' to transform a touch position to the screen units
    mat4 p;
    mat4 p_inv; // inv(p)

    // view / pose of the camera in 3d space
    mat4 v;
    mat4 v_inv; // inv(v)

    // combination of view and projection
    // pass 'vp' to a render object,
    //     to draw it in the world,
    //     so that the camera movement of 'v' is used
    // use 'v_p_inv' to transform a touch position to the world
    mat4 vp; // p @ v_inv
    mat4 v_p_inv; // v @ p_inv
};

/**
 * Sets matrices to eye
 * @param self The camera matrices to init
 */
O_EXTERN
void RCam_matrices_init(struct RCam_matrices* self);

/**
 * Updates all matrices (but .v and .p) according to .v and .p
 * @param self The camera matrices to update
 */
O_EXTERN
void RCam_matrices_update(struct RCam_matrices* self);

/**
 * Virtual update function.
 * Updates the internal .cam matrices and other fields.
 * @param camera_obj RCam object
 */
typedef void (*RCam__update_fn)(oobj camera_obj, int back_cols, int back_rows);


typedef struct
{
    OObj super;

    // given by update
    ivec2 back_size;

    struct r_proj proj;
    struct RCam_matrices cam;

    // options
    bool pixelperfect_scale;
    bool pixelperfect_view;
    vec2 min_units_size;

    vec2 pos;
    float rotation;
    float zoom;

    // vfuncs
    RCam__update_fn v_update;
} RCam;


/**
 * Initializes the object
 * @param obj RCam object
 * @param parent to inherit from
 * @param pixelperfect if true, each unit as an integer amount of real pixelc
 *                      sets both, _scale and _view
 * @param min_units_size to calc the scale, use <= to ignore, both <=0 to set scale to 1.0f
 * @return obj casted as RCam
 */
O_EXTERN
RCam* RCam_init(oobj obj, oobj parent, bool pixelperfect, vec2 min_units_size);

/**
 * Creates a new the RCam object
 * @param parent to inherit from
 * @param pixelperfect if true, each unit as an integer amount of real pixelc
 *                      sets both, _scale and _view
 * @param min_units_size to calc the scale, use <= to ignore, both <=0 to set scale to 1.0f
 * @return The new object
 */
O_INLINE
RCam* RCam_new(oobj parent, bool pixelperfect, vec2 min_units_size)
{
    OObj_DECL_IMPL_NEW(RCam, parent, pixelperfect, min_units_size);
}

//
// virtual implementations:
//

/**
 * virtual function
 * Updates the internal .cam matrices and other fields.
 * @param obj RCam object
 */
O_EXTERN
void RCam__v_update(oobj obj, int back_cols, int back_rows);


//
// object functions
//

/**
 * Calls the virtual resize function.
 * Extended version.
 * Updates the internal .cam matrices and other fields.
 * @param obj RCam object
 */
O_INLINE
void RCam_update_ex(oobj obj, int back_cols, int back_rows)
{
    OObj_assert(obj, RCam);
    RCam* self = obj;
    self->v_update(self, back_cols, back_rows);
}

/**
 * Calls the virtual resize function.
 * Simplified version, using the old back size
 * Updates the internal .cam matrices and other fields.
 * @param obj RCam object
 */
O_INLINE
void RCam_update(oobj obj)
{
    OObj_assert(obj, RCam);
    RCam* self = obj;
    self->v_update(self, m_2(self->back_size));
}


/**
 * @param obj RCam object
 * @return The back_size given by RCam_update_ex
 */
OObj_DECL_GET(RCam, ivec2, back_size)

/**
 * @param obj RCam object
 * @return A reference to the p matrix (perspective | orthogonal matrix).
 *         To be used for hud, etc. when the v is moved, ...
 */
O_INLINE
const mat4* RCam_cam_p(oobj obj)
{
    OObj_assert(obj, RCam);
    RCam* self = obj;
    return &self->cam.p;
}

/**
 * @param obj RCam object
 * @return A reference to the inverted p matrix (perspective | orthogonal matrix)
 *         To be used for pointer events on huds, etc. when the v is moved, ...
 */
O_INLINE
const mat4* RCam_cam_p_inv(oobj obj)
{
    OObj_assert(obj, RCam);
    RCam* self = obj;
    return &self->cam.p_inv;
}


/**
 * @param obj RCam object
 * @return A reference to the vp matrix.
 *         To be used to render.
 */
O_INLINE
const mat4* RCam_cam(oobj obj)
{
    OObj_assert(obj, RCam);
    RCam* self = obj;
    return &self->cam.vp;
}

/**
 * @param obj RCam object
 * @return A reference to the inverted vp matrix.
 *         To be used for pointer events.
 */
O_INLINE
const mat4* RCam_cam_inv(oobj obj)
{
    OObj_assert(obj, RCam);
    RCam* self = obj;
    return &self->cam.v_p_inv;
}

/**
 * @param obj RCam object
 * @return The pixelperfect scale option.
 */
OObj_DECL_GETSET(RCam, bool, pixelperfect_scale)

/**
 * @param obj RCam object
 * @return The pixelperfect view option.
 */
OObj_DECL_GETSET(RCam, bool, pixelperfect_view)

/**
 * @param obj RCam object
 * @return to calc the scale, use <= to ignore, both <=0 to set scale to 1.0f
 *         If changed, call update...
 */
OObj_DECL_GETSET(RCam, vec2, min_units_size)

/**
 * @param obj RCam object
 * @return The used pos for the v matrix
 * @note if pixelperfect: RCam_round_real_pixel is used on pos
 *
 */
OObj_DECL_GET(RCam, vec2, pos)

/**
 * @param obj RCam object
 * @param pos new position for the v matrix, call RCam_update to use it!
 * @param update if true, RCam_update is called
 * @return The used pos for the v matrix
 * @note if pixelperfect: RCam_round_real_pixel is used on pos
 *
 */
O_EXTERN
vec2 RCam_pos_set(oobj obj, vec2 pos, bool update);

/**
 * @param obj RCam object
 * @return The zoom of the v matrix (default = 1.0)
 *         If >1.0, the rendered objects appear smaller; <1.0: bigger
 *         If changed, call update...
 * @note if pixelperfect: rounded
 */
OObj_DECL_GET(RCam, float, zoom)

/**
 * @param obj RCam object
 * @param zoom new zoom for the v matrix, call RCam_update to use it!
 *        If >1.0, the rendered objects appear smaller; <1.0: bigger
 * @param update if true, RCam_update is called
 * @return The zoom of the v matrix (default = 1.0)
 * @note if pixelperfect: if > 1, rounded
 *                        if < 1, rounded to inv "zoom = 1.0 / round(1.0 / zoom)"
 *       asserts zoom > 0
 */
O_EXTERN
float RCam_zoom_set(oobj obj, float zoom, bool update);

/**
 * @param obj RCam object
 * @return A reference to the rotation of the v matrix [rad]
 * @note if pixelperfect: rounded to m_PI_2
 */
OObj_DECL_GET(RCam, float, rotation)

/**
 * @param obj RCam object
 * @param rotation_rad new rotation for the v matrix, call RCam_update to use it!
 * @param update if true, RCam_update is called
 * @return A reference to the rotation of the v matrix [rad]
 * @note if pixelperfect: rounded to m_PI_2
 */
O_EXTERN
float RCam_rotation_set(oobj obj, float rotation_rad, bool update);


/**
 * @param obj RCam object
 * @return ref to r_proj matrices for rendering, etc.
 */
O_INLINE
const struct r_proj* RCam_proj_ref(oobj obj)
{
    OObj_assert(obj, RCam);
    RCam* self = obj;
    return &self->proj;
}

/**
 * @param obj RCam object
 * @return copy of r_proj matrices for rendering, etc.
 */
O_INLINE
struct r_proj RCam_proj(oobj obj)
{
    return *RCam_proj_ref(obj);
}


/**
 * @param obj RCam object
 * @return copy of r_proj matrices for rendering to the perspective only cam, etc.
 */
O_INLINE
struct r_proj RCam_proj_p(oobj obj)
{
    struct r_proj res = *RCam_proj_ref(obj);
    res.cam = *RCam_cam_p(obj);
    return res;
}

/**
 * Applies RCam_proj onto RTex_proj
 * @param obj RCam object
 * @param tex RTex object to apply the proj on
 */
O_EXTERN
void RCam_apply_proj(oobj obj, oobj tex);

/**
 * Applies RCam_proj_p onto RTex_proj
 * @param obj RCam object
 * @param tex RTex object to apply the perspective only proj on
 */
O_EXTERN
void RCam_apply_proj_p(oobj obj, oobj tex);


//
// r_proj delegation
//

/**
 * @param obj RCam object
 * @return The unit size given by update
 * @note according to the p matrix, independent to position, rotation and zoom
 */
O_INLINE
vec2 RCam_size(oobj obj)
{
    OObj_assert(obj, RCam);
    RCam* self = obj;
    return self->proj.size;
}

/**
 * @param obj RCam object
 * @return size of the integer bounds (may be slightly less than proj->size)
 * @note according to the p matrix, independent to position, rotation and zoom
 */
O_INLINE
ivec2 RCam_size_int(oobj obj)
{
    OObj_assert(obj, RCam);
    RCam* self = obj;
    return r_proj_size_int(&self->proj);
}

/**
 * @param obj RCam object
 * @return The scale (real pixels used for a unit)
 */
O_INLINE
float RCam_scale(oobj obj)
{
    OObj_assert(obj, RCam);
    RCam* self = obj;
    return self->proj.scale;
}

/**
 * @param obj RCam object
 * @return The unit border
 * @note according to the p matrix, independent to position, rotation and zoom
 */
O_INLINE
float RCam_left(oobj obj)
{
    OObj_assert(obj, RCam);
    RCam* self = obj;
    return r_proj_left(&self->proj);
}

/**
 * @param obj RCam object
 * @return The unit border
 * @note according to the p matrix, independent to position, rotation and zoom
 */
O_INLINE
float RCam_right(oobj obj)
{
    OObj_assert(obj, RCam);
    RCam* self = obj;
    return r_proj_right(&self->proj);
}

/**
 * @param obj RCam object
 * @return The unit border
 * @note according to the p matrix, independent to position, rotation and zoom
 */
O_INLINE
float RCam_bottom(oobj obj)
{
    OObj_assert(obj, RCam);
    RCam* self = obj;
    return r_proj_bottom(&self->proj);
}

/**
 * @param obj RCam object
 * @return The unit border
 * @note according to the p matrix, independent to position, rotation and zoom
 */
O_INLINE
float RCam_top(oobj obj)
{
    OObj_assert(obj, RCam);
    RCam* self = obj;
    return r_proj_top(&self->proj);
}

/**
 * @param obj RCam object
 * @return The unit border as integer (may be slightly in the view to be an integer)
 * @note according to the p matrix, independent to position, rotation and zoom
 */
O_INLINE
int RCam_left_int(oobj obj)
{
    OObj_assert(obj, RCam);
    RCam* self = obj;
    return r_proj_left_int(&self->proj);
}

/**
 * @param obj RCam object
 * @return The unit border as integer (may be slightly in the view to be an integer)
 * @note according to the p matrix, independent to position, rotation and zoom
 */
O_INLINE
int RCam_right_int(oobj obj)
{
    OObj_assert(obj, RCam);
    RCam* self = obj;
    return r_proj_right_int(&self->proj);
}

/**
 * @param obj RCam object
 * @return The unit border as integer (may be slightly in the view to be an integer)
 * @note according to the p matrix, independent to position, rotation and zoom
 */
O_INLINE
int RCam_bottom_int(oobj obj)
{
    OObj_assert(obj, RCam);
    RCam* self = obj;
    return r_proj_bottom_int(&self->proj);
}

/**
 * @param obj RCam object
 * @return The unit border as integer (may be slightly in the view to be an integer)
 * @note according to the p matrix, independent to position, rotation and zoom
 */
O_INLINE
int RCam_top_int(oobj obj)
{
    OObj_assert(obj, RCam);
    RCam* self = obj;
    return r_proj_top_int(&self->proj);
}

/**
 * @param obj RCam object
 * @return left, bottom, width, height
 */
O_INLINE
vec4 RCam_lbwh(oobj obj)
{
    OObj_assert(obj, RCam);
    RCam* self = obj;
    return r_proj_lbwh(&self->proj);
}

/**
 * @param obj RCam object
 * @return left, right, bottom, top
 */
O_INLINE
vec4 RCam_bounds(oobj obj)
{
    OObj_assert(obj, RCam);
    RCam* self = obj;
    return r_proj_bounds(&self->proj);
}

/**
* @param obj RCam object
 * @param x position
 * @param y position
 * @return unit pose according to the p matrix, containing the full projection on position xy
 */
O_INLINE
mat4 RCam_pose(oobj obj, float x, float y)
{
    OObj_assert(obj, RCam);
    RCam* self = obj;
    return r_proj_pose(&self->proj, x, y);
}

/**
 * @param obj RCam object
 * @param x position
 * @param y position
 * @return unit rect according to the p matrix, containing the full projection on position xy
 */
O_INLINE
vec4 RCam_rect(oobj obj, float x, float y)
{
    OObj_assert(obj, RCam);
    RCam* self = obj;
    return r_proj_rect(&self->proj, x, y);
}


/**
 * @param obj RCam object
 * @return true if camera is in portrait mode (if back is the screen -> smartphone portrait mode)
 * @note according to the p matrix, independent to position, rotation and zoom
 */
O_INLINE
bool RCam_is_portrait(oobj obj)
{
    OObj_assert(obj, RCam);
    RCam* self = obj;
    return r_proj_is_portrait(&self->proj);
}

#endif //R_RCAM_H
