#ifndef MIA_TEX_H
#define MIA_TEX_H

/**
 * @file tex.h
 *
 * RTex's shipped with the 'r' module.
 * And some default/basic textures like kernels or a white pixel
 * All loaded lazy on first need.
 */

#include "o/common.h"

/**
 * @return A shared RTex which is a 1x1 white pixel.
 * @note DO NOT delete it, will assert fail.
 *       DO NOT change the internal buffer.
 *       This tex is used as default mask.
 */
O_EXTERN
oobj r_tex_white(void);

/**
 * @param normalized if true, the kernel will get normalized to a sum of 1.0f, false all values 1.0
 * @return A shared RTex which is a 2x2 kernel
 * @note DO NOT delete it, will assert fail.
 *       DO NOT change the internal buffer.
 *       This tex is used as default mask.
 */
O_EXTERN
oobj r_tex_kernel_block2(bool normalized);

/**
 * @param normalized if true, the kernel will get normalized to a sum of 1.0f, false all values 1.0
 * @return A shared RTex which is a 3x3 kernel
 * @note DO NOT delete it, will assert fail.
 *       DO NOT change the internal buffer.
 *       This tex is used as default mask.
 */
O_EXTERN
oobj r_tex_kernel_block3(bool normalized);

/**
 * @param normalized if true, the kernel will get normalized to a sum of 1.0f, false plus values 1.0
 * @return A shared RTex which is a 3x3 plus kernel
 * @note DO NOT delete it, will assert fail.
 *       DO NOT change the internal buffer.
 *       This tex is used as default mask.
 */
O_EXTERN
oobj r_tex_kernel_plus3(bool normalized);

/**
 * @param normalized if true, the kernel will get normalized to a sum of 1.0f, false max value 1.0
 * @return A shared RTex which is a 9x9 gauss kernel
 * @note DO NOT delete it, will assert fail.
 *       DO NOT change the internal buffer.
 *       This tex is used as default mask.
 */
O_EXTERN
oobj r_tex_kernel_gauss9(bool normalized);


//
// files
//

/**
 * @return RProgram of the 'r' module, lazy loaded
 */
#define r_tex_DECL(file)\
O_EXTERN \
oobj r_tex_ ## file (void);


r_tex_DECL(font35)
r_tex_DECL(font35_shadow)


#undef r_tex_DECL

#endif //MIA_TEX_H
