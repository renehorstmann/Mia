#ifndef M_SCA_INT_H
#define M_SCA_INT_H

/**
 * @file sca/int.h
 *
 * some defines in the mi_ namespace
 */

#include "m/common.h"

/** maximum possible value */
#define mi_MAX INT_MAX

/** minimum possible value */
#define mi_MIN INT_MIN

/** for printing in IO */
#define mi_PRI "i"


//
// macro copies of o/common
//

/** @return the absolute value */
#define mi_abs(x) ((x) < 0 ? -(x) : (x))

/** @return the smaller value */
#define mi_min(a, b) ((a)<(b)?(a):(b))

/** @return the bigger value */
#define mi_max(a, b) ((a)>(b)?(a):(b))

/** @return x clamped in the range [min : max] (so can be exactly min and exactly max) */
#define mi_clamp(x, min, max) ((x) < (min) ? (min) : ((x) > (max) ? (max) : (x)))

/** @return x % y as positive version for integers */
#define mi_mod(x, y) (((x) % (y) + (y)) % (y))

/** @return x>0: +1; x==0: 0; x<0: -1 (the sign of the value, or 0 on 0) */
#define mi_sign(x) ((x) > 0 ? 1 : ((x) < 0 ? -1 : 0))

/** @return x>=edge? 1:0 (true or 1 if x has reached the step) */
#define mi_step(edge, x) (((x) < (edge)) ? 0 : 1)

//
// integer based functions
//

/** dst = a * (1-t) + b * t */
O_INLINE
int mi_mix(int a, int b, float t)
{
    return (int) ((float) a * (1.0f - t) + (float) b * t);
}

#endif //M_SCA_INT_H
