#ifndef R_RBUFFERRECT_H
#define R_RBUFFERRECT_H

/**
 * @file RBuffer.h
 *
 * Object
 *
 * An RBuffer initialized for struct r_rect
 */

#include "RBuffer.h"


/** object id */
#define RBufferRect_ID RBuffer_ID "Rect"

typedef struct {
    RBuffer super;

} RBufferRect;


/**
 * Initializes the object
 * @param obj RBuffer object
 * @param parent to inherit from
 * @return obj casted as RBuffer
 */
O_EXTERN
RBufferRect *RBufferRect_init(oobj obj, oobj parent);

/**
 * Creates a new the RBuffer object
 * @param parent to inherit from
 * @return The new object
 */
O_INLINE
RBufferRect *RBufferRect_new(oobj parent)
{
    OObj_DECL_IMPL_NEW(RBufferRect, parent);
}


#endif //R_RBUFFERRECT_H
