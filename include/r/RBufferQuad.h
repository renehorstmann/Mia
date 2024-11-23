#ifndef R_RBUFFERQUAD_H
#define R_RBUFFERQUAD_H

/**
 * @file RBuffer.h
 *
 * Object
 *
 * An RBuffer initialized for struct r_quad
 */

#include "RBuffer.h"


/** object id */
#define RBufferQuad_ID RBuffer_ID "Quad"

typedef struct {
    RBuffer super;

} RBufferQuad;


/**
 * Initializes the object
 * @param obj RBuffer object
 * @param parent to inherit from
 * @return obj casted as RBuffer
 */
O_EXTERN
RBufferQuad *RBufferQuad_init(oobj obj, oobj parent);

/**
 * Creates a new the RBuffer object
 * @param parent to inherit from
 * @return The new object
 */
O_INLINE
RBufferQuad *RBufferQuad_new(oobj parent)
{
    OObj_DECL_IMPL_NEW(RBufferQuad, parent);
}


#endif //R_RBUFFERQUAD_H
