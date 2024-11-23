#ifndef MP_VIEWS_H
#define MP_VIEWS_H

/**
 * @file views.h
 *
 * Functions to create MPView's.
 */

#include "MPView.h"

O_EXTERN
MPView *mp_views_new_canvas(oobj parent);

O_EXTERN
MPView *mp_views_new_palette(oobj parent);

O_EXTERN
MPView *mp_views_new_pattern(oobj parent);


#endif //MP_VIEWS_H
