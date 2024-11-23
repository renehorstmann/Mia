#ifndef MP_CANVAS_H
#define MP_CANVAS_H

/**
 * @file canvas.h
 *
 * Holds the active image and manages layers and masks
 */

#include "o/common.h"
#include "m/types/int.h"
#include "m/types/flt.h"
#include "a/pointer.h"

O_EXTERN
void mp_canvas_render(oobj tex);

O_EXTERN
void mp_canvas_pointer(struct a_pointer pointer);


O_EXTERN
ivec2 mp_canvas_size_int(void);

O_INLINE
vec2 mp_canvas_size(void)
{
    ivec2 size = mp_canvas_size_int();
    return vec2_(m_2(size));
}



#endif //MP_CANVAS_H
