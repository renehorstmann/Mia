#ifndef R_RECT_H
#define R_RECT_H

/**
 * @file rect.h
 *
 * Defines a render rectangle
 */

#include "m/types/flt.h"


struct r_rect {
    vec4 rect;      // xywh rect position
    vec4 uv_rect;   // xywh rect for the uv tex position

    // additional "st(uv)
    vec4 s, t;
    
};

/**
 * Initializes the rect
 * @param tex_cols, tex_rows to set the size for pose and uv
 * @return a new default initialized rect
 * @note s = vec4_(1); t = vec4_(0);
 */
O_EXTERN
struct r_rect r_rect_new(int tex_cols, int tex_rows);

#endif //R_RECT_H
