#ifndef R_RSHADERQUADKERNEL_H
#define R_RSHADERQUADKERNEL_H

/**
 * @file RShaderQuadKernel.h
 *
 * Object
 *
 * Kernel shader for quads (programs):
 * - _contour (dilate) for outlines, shadows, ...
 * - _contour_inv (erode)
 * - _bloom
 * - _conv Convolution
 * 
 */

#include "RShaderQuad.h"

/** object id */
#define RShaderQuadKernel_ID RShaderQuad_ID "Kernel"

typedef struct {
    RShaderQuad super;
    
    // RTex, .r used as kernel
    oobj kernel;

    // optional parameters, not all used in all programs
    ivec2 offset;
    vec4 color;
    vec4 mask;

} RShaderQuadKernel;


/**
 * Initializes the object.
 * Program needs to be set manually!
 * Shader can also be used for morph dilate.
 * @param obj RShaderQuadKernel object
 * @param parent to inherit from
 * @param tex RTex object, NULL safe
 * @param move_tex if true, tex is o_move'd into this object
 * @param kernel RTex object for the kernel (.r used), NULL safe
 * @param move_kernel if true, kernel is o_move'd into this object
 * @return obj casted as RShaderQuadKernel
 * @note mask set for alpha: vec4(0, 0, 0, 1)
 *       color set to R_WHITE
 */
O_EXTERN
RShaderQuadKernel *RShaderQuadKernel_init(oobj obj, oobj parent, oobj tex, bool move_tex, 
        oobj kernel, bool move_kernel);


/**
 * Creates a new RShaderQuadKernel object.
 * Program needs to be set manually!
 * Shader can also be used for morph dilate.
 * @param parent to inherit from
 * @param tex RTex object, NULL safe;
 * @param move_tex if true, tex is o_move'd into this object
 * @param kernel RTex object for the kernel (.r used), NULL safe
 * @param move_kernel if true, kernel is o_move'd into this object
 * @return The new object
 * @note mask set for alpha: vec4(0, 0, 0, 1)
 *       color set to R_WHITE
 */
O_INLINE
RShaderQuadKernel *RShaderQuadKernel_new(oobj parent, oobj tex, bool move_tex, 
        oobj kernel, bool move_kernel) {
    OObj_DECL_IMPL_NEW(RShaderQuadKernel, parent, tex, move_tex, kernel, move_kernel);
}


//
// virtual implementations:
//

O_EXTERN
void RShaderQuadKernel__v_render(oobj obj, oobj program, int num, const struct r_proj *proj);


//
// object functions:
//

/**
 * @param obj RShaderQuadKernel object
 * @return RTex kernel
 */
O_INLINE
oobj RShaderQuadKernel_kernel(oobj obj) {
    OObj_assert(obj, RShaderQuadKernel);
    RShaderQuadKernel *self = obj;
    return self->kernel;
}

/**
 * @param obj RShaderQuadKernel object
 * @param set RTex kernel, .r used
 * @param del_old if true, the old kernel is o_del'ed
 * @return RTex kernel
 */
O_EXTERN
oobj RShaderQuadKernel_kernel_set(oobj obj, oobj set, bool del_old);


/**
 * @param obj RShaderQuadKernel object
 * @return kernel center offset
 */
OObj_DECL_GETSET(RShaderQuadKernel, ivec2, offset)

/**
 * @param obj RShaderQuadKernel object
 * @return resulting color, defaults to R_WHITE
 */
OObj_DECL_GETSET(RShaderQuadKernel, vec4, color)

/**
 * @param obj RShaderQuadKernel object
 * @return masked tex color, defaults to alpha: vec4_(0, 0, 0, 1)
 */
OObj_DECL_GETSET(RShaderQuadKernel, vec4, mask)



#endif //R_RSHADERQUADKERNEL_H
