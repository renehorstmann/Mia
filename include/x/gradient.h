#ifndef X_GRADIENT_H
#define X_GRADIENT_H

/**
 * @file gradient.h
 *
 * Creates a WGradient and sets thenum _pressed_event to open a viewnum edit scene
 */

#include "w/WGradient.h"



/**
 * Creates a new WGradient object.
 * Sets the num_pressed_event to open a viewgradient edit scene
 * @param parent to inherit from
 * @param uv_rect in WTheme Tex coordinates, like "u_atlas_rect(WTheme_atlas(...), WTheme_CUSTOM_8);"
 * @param min, max minimal and maximal num (clamped)
 * @param step size between a value, will determine the printed precision (asserts >0)
 * @param opt_label if not NULL set as text in the frame label
 * @return The new object
 */
O_EXTERN
WGradient *x_gradient(oobj parent, vec4 uv_rect, float min, float max, float step, const char *opt_label);


#endif //X_GRADIENT_H
