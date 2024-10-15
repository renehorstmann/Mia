#ifndef O_OSTREAMARRAY_H
#define O_OSTREAMARRAY_H

/**
 * @file OStreamArray.h
 *
 * Object
 *
 * OStream implementation using OArray as memory
 * The OStreamArray ignores miss matching element sizes...
 */

#include "OStream.h"
#include "OArray.h"

/** object id */
#define OStreamArray_ID OStream_ID "Array"

/**
 * Some different modes for the OArray.
 * SEEKABLE pushes or overwrites on write, but never pops on read and is seekable.
 * FIFO pushes back on write and pops front on read. assert(element_size==Array_element_size())
 * LIFO pushes back on write and pops back on read. assert(element_size==OArray_element_size())
 * *_FRONT as the above, but working backwards / inversed to push_front
 * Searching for a mode which won't push nor pop? have a look at OStreamMem
 */
enum OStreamArray_mode {
    OStreamArray_SEEKABLE,
    OStreamArray_FIFO,
    OStreamArray_LIFO,
    OStreamArray_FIFO_FRONT,
    OStreamArray_LIFO_FRONT,
    OStreamArray_NUM_MODES
};


typedef struct {
    OStream super;

    oobj array;

    enum OStreamArray_mode mode;

    // in bytes, only for OStreamArray_SEEKABLE
    osize pos;
} OStreamArray;


/**
 * Initializes the object
 * @param obj OStreamArray object
 * @param parent to inherit from
 * @param array The OArray to work on
 * @param move_array if true, array is o_move'd into this object
 * @param mode for example OStreamArray_FIFO for a stream queue or LIFO for STACK or just SEEKABLE
 * @return obj casted as OStreamArray
 */
O_EXTERN
OStreamArray *OStreamArray_init(oobj obj, oobj parent, oobj array, bool move_array, enum OStreamArray_mode mode);


/**
 * Creates a new OStreamArray object
 * @param parent to inherit from
 * @param array The OArray to work on
 * @param move_array if true, array is o_move'd into this object
 * @param mode for example OStreamArray_FIFO for a stream queue or LIFO for STACK or just SEEKABLE
 * @return The new object
 */
O_INLINE
OStreamArray *OStreamArray_new(oobj parent, oobj array, bool move_array, enum OStreamArray_mode mode)
{
    OObj_DECL_IMPL_NEW(OStreamArray, parent, array, move_array, mode);
}

//
// virtual implementations:
//

O_EXTERN
bool OStreamArray__v_valid(oobj obj);

O_EXTERN
osize OStreamArray__v_size(oobj obj);

O_EXTERN
osize OStreamArray__v_seek(oobj obj, osize offset, enum OStream_seek_whence whence);

O_EXTERN
osize OStreamArray__v_read_try(oobj obj, void *data, osize element_size, osize num);

O_EXTERN
osize OStreamArray__v_write(oobj obj, const void *data, osize element_size, osize num);

O_EXTERN
bool OStreamArray__v_close(oobj obj);


//
// object functions
//

/**
 * @param obj OStreamArray object
 * @return internal array to work on
 */
OObj_DECL_GETSET(OStreamArray, oobj, array)

/**
 * @param obj OStreamArray object
 * @return for example OStreamArray_FIFO for a stream queue or LIFO for STACK or just SEEKABLE
 */
OObj_DECL_GETSET(OStreamArray, enum OStreamArray_mode, mode)

/**
 * @param obj OStreamArray object
 * @return in bytes, only for OStreamArray_SEEKABLE
 */
OObj_DECL_GETSET(OStreamArray, osize, pos)

#endif //O_OSTREAMARRAY_H
