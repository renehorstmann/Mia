#ifndef R_QUAD_H
#define R_QUAD_H

/**
 * @file quad.h
 *
 * Defines a render quadrilaterals
 */

#include "m/types/flt.h"


struct r_quad {
    mat4 pose;      // 3d pose for the rect position (see u/pose.h)
    mat4 uv;        // 3d pose for the uv tex position (see u/pose.h)

    // additional "stuv"
    union {
        mat4 stuv;
        struct {
            vec4 s, t, u, v;
        };
    };
    
};

/**
 * Initializes the rect
 * @param tex_cols, tex_rows to set the size for pose and uv
 * @return a new default initialized rect
 * @note s = vec4_(1); t = u = v = vec4_(0);
 */
O_EXTERN
struct r_quad r_quad_new(int tex_cols, int tex_rows);

#endif //R_QUAD_H
