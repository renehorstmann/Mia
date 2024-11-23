#ifndef U_GRADIENT_H
#define U_GRADIENT_H

/**
 * @file gradient.h
 *
 * create gradient color rgba arrays to be put into an RTex for example.
 *
 */

#include "o/common.h"
#include "m/types/byte.h"

/**
 * Creates a hue gradient
 * @param rgba_data pixel data
 * @param size number of pixels
 */
O_EXTERN
void u_gradient_hue(bvec4 *rgba_data, int size);

/**
 * Creates a sat gradient
 * @param rgba_data pixel data
 * @param size number of pixels
 * @param hue for coloring the gradient [0:1]
 */
O_EXTERN
void u_gradient_sat(bvec4 *rgba_data, int size, float hue);

/**
 * Creates a hue gradient
 * @param rgba_data pixel data
 * @param size number of pixels
 * @param hue for coloring the gradient [0:1]
 * @param sat for coloring the gradient [0:1]
 */
O_EXTERN
void u_gradient_val(bvec4 *rgba_data, int size, float hue, float sat);

/**
 * Creates a red gradient
 * @param rgba_data pixel data
 * @param size number of pixels
 * @param green for coloring the gradient [0:1]
 * @param blue for coloring the gradient [0:1]
 */
O_EXTERN
void u_gradient_red(bvec4 *rgba_data, int size, float green, float blue);

/**
 * Creates a green gradient
 * @param rgba_data pixel data
 * @param size number of pixels
 * @param red for coloring the gradient [0:1]
 * @param blue for coloring the gradient [0:1]
 */
O_EXTERN
void u_gradient_green(bvec4 *rgba_data, int size, float red, float blue);

/**
 * Creates a blue gradient
 * @param rgba_data pixel data
 * @param size number of pixels
 * @param red for coloring the gradient [0:1]
 * @param green for coloring the gradient [0:1]
 */
O_EXTERN
void u_gradient_blue(bvec4 *rgba_data, int size, float red, float green);

/**
 * Creates a blue gradient
 * @param rgba_data pixel data
 * @param size number of pixels
 * @param red for coloring the gradient [0:1]
 * @param green for coloring the gradient [0:1]
 * @param blue for coloring the gradient [0:1]
 */
O_EXTERN
void u_gradient_alpha(bvec4 *rgba_data, int size, float red, float green, float blue);

#endif //U_GRADIENT_H
