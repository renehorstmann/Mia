#ifndef R_PROGRAM_H
#define R_PROGRAM_H

/**
 * @file program.h
 *
 * RProgram's shipped with the 'r' module.
 * All loaded lazy on first need.
 */


#include "r/common.h"

/**
 * just here to jump to src with an ide
 */
O_EXTERN
void r_program__foo(void);

/**
 * @return RProgram of the 'r' module, lazy loaded
 */
#define r_program_DECL(file, num_draw_buffer)\
O_EXTERN \
oobj r_program_ ## file (void);


r_program_DECL(Quad, 1)
r_program_DECL(Quad_addscaled, 1)
r_program_DECL(Quad_color, 1)
r_program_DECL(Quad_color_hsva, 1)
r_program_DECL(Quad_hsva_from_rgba, 1)
r_program_DECL(Quad_hsva_into_rgba, 1)
r_program_DECL(Quad_mixer, 1)
r_program_DECL(QuadChannelmerge, 1)
r_program_DECL(QuadDab, 2)
r_program_DECL(QuadKernel_bloom, 1)
r_program_DECL(QuadKernel_contour, 1)
r_program_DECL(QuadKernel_contour_inv, 1)
r_program_DECL(QuadKernel_conv, 1)
r_program_DECL(QuadMerge, 1)
r_program_DECL(Rect, 1)
r_program_DECL(Rect_color, 1)


#undef r_program_DECL


#endif //R_PROGRAM_H
