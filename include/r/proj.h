#ifndef R_PROJ_H
#define R_PROJ_H

/**
 * @file proj.h
 *
 * projections used for render objects, created default for RTex or by an RCam for example
 */

#include "m/types/flt.h"
#include "m/types/int.h"
#include "m/sca/flt.h"

struct r_proj {
    mat4 cam;

    // viewport | texture size
    ivec2 viewport_size;
    // viewport_size half, to quickly set for render obejct uniforms
    vec2 vpsh;
    // if viewport_size is odd, positive offset to be even again (either 0.0 or 0.5)
    vec2 viewport_even_offset;

    // scaled size == viewport_size / scale
    vec2 size;
    float scale;
};

/**
 * @param viewport_size size of the viewport or texture
 * @param min_units_size to calc the scale, use <= to ignore, both <=0 to set scale to 1.0f
 * @param pixelperfect true to use integer scaling
 * @return a projection for the viewport
 */
O_EXTERN
struct r_proj r_proj_new(ivec2 viewport_size, vec2 min_units_size, bool pixelperfect);


/**
 * @param proj reference to the projection matrices
 * @return unit border
 */
O_INLINE
float r_proj_left(const struct r_proj *proj)
{
    return -proj->size.x / 2.0f;
}

/**
 * @param proj reference to the projection matrices
 * @return unit border
 */
O_INLINE
float r_proj_right(const struct r_proj *proj)
{
    return +proj->size.x / 2.0f;
}

/**
 * @param proj reference to the projection matrices
 * @return unit border
 */
O_INLINE
float r_proj_bottom(const struct r_proj *proj)
{
    return -proj->size.y / 2.0f;
}

/**
 * @param proj reference to the projection matrices
 * @return unit border
 */
O_INLINE
float r_proj_top(const struct r_proj *proj)
{
    return +proj->size.y / 2.0f;
}


/**
 * @param proj reference to the projection matrices
 * @return unit border as integer which may be slightly more in the view to be an integer
 */
O_INLINE
int r_proj_left_int(const struct r_proj *proj)
{
    return (int) m_ceil(r_proj_left(proj));
}


/**
 * @param proj reference to the projection matrices
 * @return unit border as integer which may be slightly more in the view to be an integer
 */
O_INLINE
int r_proj_right_int(const struct r_proj *proj)
{
    return (int) m_floor(r_proj_right(proj));
}


/**
 * @param proj reference to the projection matrices
 * @return unit border as integer which may be slightly more in the view to be an integer
 */
O_INLINE
int r_proj_bottom_int(const struct r_proj *proj)
{
    return (int) m_ceil(r_proj_bottom(proj));
}


/**
 * @param proj reference to the projection matrices
 * @return unit border as integer which may be slightly more in the view to be an integer
 */
O_INLINE
int r_proj_top_int(const struct r_proj *proj)
{
    return (int) m_floor(r_proj_top(proj));
}


/**
 * @param proj reference to the projection matrices
 * @return size of the integer bounds (may be slightly less than proj->size)
 */
O_INLINE
ivec2 r_proj_size_int(const struct r_proj *proj)
{
    return ivec2_(r_proj_right(proj) - r_proj_left(proj),
                  r_proj_top(proj) - r_proj_bottom(proj));
}


/**
 * @param proj reference to the projection matrices
 * @return unit bounds: left, bottom, width, height
 */
O_INLINE
vec4 r_proj_lbwh(const struct r_proj *proj)
{
    return vec4_(r_proj_left(proj),
                 r_proj_bottom(proj),
                 proj->size.x,
                 proj->size.y);
}

/**
 * @param proj reference to the projection matrices
 * @param lbwh rect in projection camera
 * @param opt_parent_viewport_offset current viewports .xy
 * @return viewport in pixels
 */
O_INLINE
ivec4 r_proj_lbwh_to_viewport(const struct r_proj *proj, vec4 lbwh, const ivec2 *opt_parent_viewport_offset)
{
    ivec4 vp = ivec4_(
            (lbwh.v0 - r_proj_left(proj)) * proj->scale,
            (lbwh.v1 - r_proj_bottom(proj)) * proj->scale,
            lbwh.v2 * proj->scale,
            lbwh.v3 * proj->scale,
            );
    if(opt_parent_viewport_offset) {
        vp.x += opt_parent_viewport_offset->x;
        vp.y += opt_parent_viewport_offset->y;
    }
    return vp;
}

/**
 * @param proj reference to the projection matrices
 * @param viewport rect in pixels
 * @param opt_parent_viewport_offset parents viewports .xy
 * @return lbwh in projection camera
 */
O_INLINE
vec4 r_proj_viewport_to_lbwh(const struct r_proj *proj, ivec4 viewport, const ivec2 *opt_parent_viewport_offset)
{
    if(opt_parent_viewport_offset) {
        viewport.x -= opt_parent_viewport_offset->x;
        viewport.y -= opt_parent_viewport_offset->y;
    }
    return vec4_(
            ((float) viewport.v0 / proj->scale) + r_proj_left(proj),
            ((float) viewport.v1 / proj->scale) + r_proj_bottom(proj),
            ((float) viewport.v2 / proj->scale),
            ((float) viewport.v3 / proj->scale)
            );
}

/**
 * @param proj reference to the projection matrices
 * @return unit bounds: left, right, bottom, top
 */
O_INLINE
vec4 r_proj_bounds(const struct r_proj *proj)
{
    return vec4_(r_proj_left(proj),
                 r_proj_right(proj),
                 r_proj_bottom(proj),
                 r_proj_top(proj));
}

/**
 * @param proj reference to the projection matrices
 * @param x position
 * @param y position
 * @return unit pose containing the full projection on position xy
 */
O_INLINE
mat4 r_proj_pose(const struct r_proj *proj, float x, float y)
{
    // mat4 has column major order
    return (mat4) {{
                           proj->size.x / 2.0f, 0, 0, 0,
                           0, proj->size.y / 2.0f, 0, 0,
                           0, 0, 1, 0,
                           x, y, 0, 1
                   }};
}

/**
 * @param proj reference to the projection matrices
 * @param x position
 * @param y position
 * @return unit rect containing the full projection on position xy
 */
O_INLINE
vec4 r_proj_rect(const struct r_proj *proj, float x, float y)
{
    return (vec4) {{ x, y,  proj->size.x, proj->size.y }};
}

/**
 * @param proj reference to the projection matrices
 * @return true if camera is in portrait mode (if back is the screen -> smartphone portrait mode)
 * @note according to the p matrix, independent to position, rotation and zoom
 */
O_INLINE
bool r_proj_is_portrait(const struct r_proj *proj)
{
    return proj->size.y > proj->size.x;
}

/**
 * @param proj reference to the projection matrices
 * @return x, y rounded on a real pixel, used internally by updating the pos for example.
 */
O_INLINE
vec2 r_proj_round_real_pixel(const struct r_proj *proj, float x, float y)
{
    x = (m_ceil(x * proj->scale) - proj->viewport_even_offset.x) / proj->scale;
    y = (m_ceil(y * proj->scale) - proj->viewport_even_offset.y) / proj->scale;
    return (vec2) {{x, y}};
}


#endif //R_PROJ_H
