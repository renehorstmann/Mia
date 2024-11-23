#ifndef R_RBUFFER_H
#define R_RBUFFER_H

/**
 * @file RBuffer.h
 *
 * Object
 *
 * A gl vertex buffer object with an associated vertex array object
 */

#include "o/OObj.h"


/** object id */
#define RBuffer_ID OObj_ID "RBuffer"

typedef struct {
    OObj super;
    
    osize element_size;
    osize num;

    ou32 gl_vao;
    ou32 gl_vbo;

} RBuffer;


/**
 * Initializes the object
 * @param obj RBuffer object
 * @param parent to inherit from
 * @param element_size size of a single element
 * @return obj casted as RBuffer
 */
O_EXTERN
RBuffer *RBuffer_init(oobj obj, oobj parent, osize element_size);

/**
 * Creates a new the RBuffer object
 * @param parent to inherit from
 * @param element_size size of a single element
 * @return The new object
 */
O_INLINE
RBuffer *RBuffer_new(oobj parent, osize element_size)
{
    OObj_DECL_IMPL_NEW(RBuffer, parent, element_size);
}


//
// virtual implementations:
//

/**
 * calls dlDeleteProgram on the shader program
 * @param obj RBuffer object
 */
O_EXTERN
void RBuffer__v_del(oobj obj);


//
// object functions:
//

/**
 * @param obj RBuffer object
 * @return the size of an element
 */
OObj_DECL_GET(RBuffer, osize, element_size)

/**
 * @param obj RBuffer object
 * @return current vertices num
 */
OObj_DECL_GET(RBuffer, int, num)


/**
 * @param obj RBuffer object
 * @return The vertex array object handle
 */
O_INLINE
ou32 RBuffer_vao(oobj obj)
{
    OObj_assert(obj, RBuffer);
    RBuffer *self = obj;
    return self->gl_vao;
}

/**
 * @param obj RBuffer object
 * @return The vertex buffer object handle
 */
O_INLINE
ou32 RBuffer_vbo(oobj obj)
{
    OObj_assert(obj, RBuffer);
    RBuffer *self = obj;
    return self->gl_vbo;
}

/**
 * Binds this vertex array and buffer object
 * @param obj RBuffer object
 * @param data to be updated into the buffer
 * @param num of elements in data with RBuffer_element_size
 */
O_EXTERN
void RBuffer_update(oobj obj, const void *data, osize num);

/**
 * Binds this vertex array and buffer object
 * @param obj RBuffer object, NULL safe -> calls bind with 0
 */
O_EXTERN
void RBuffer_use(oobj obj);

//
// location setter
//

/**
 * @param obj RBuffer object
 * @param location in the vertex shader in variable
 * @param offset of the vec4 in the element (offsetof)
 */
O_EXTERN
void RBuffer_location_vec4(oobj obj, int location, osize offset);

/**
 * @param obj RBuffer object
 * @param location in the vertex shader in variable
 * @param offset of the mat4 in the element (offsetof)
 * @note a mat4 needs 4 locations!
 */
O_EXTERN
void RBuffer_location_mat4(oobj obj, int location, osize offset);




#endif //R_RBUFFER_H
