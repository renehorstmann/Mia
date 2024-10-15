#ifndef MP_BRUSH_H
#define MP_BRUSH_H

/**
 * @file brush.h
 *
 * the brush module contains the current active brush draw settings
 */

#include "o/common.h"
#include "u/color.h"

O_EXTERN
bvec4 mp_brush_color(void);

O_EXTERN
oobj mp_brush_color_origin(void);

O_EXTERN
void mp_brush_color_set(bvec4 color, oobj origin);


O_EXTERN
oobj mp_brush_pattern(void);

O_EXTERN
oobj mp_brush_pattern_origin(void);

O_EXTERN
void mp_brush_pattern_set(oobj pattern, oobj origin);




#endif //MP_BRUSH_H
