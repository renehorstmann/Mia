#ifndef O_OSTREAMMEM_H
#define O_OSTREAMMEM_H

/**
 * @file OStreamMem.h
 *
 * Object
 *
 * OStream implementation which is based on memory (pre loaded file content, for example)
 * The OStreamMem ignores miss matching element sizes...
 */

#include "OStream.h"

/** object id */
#define OStreamMem_ID OStream_ID "Mem"


typedef struct {
    OStream super;

    void *memory;
    osize memory_size;

    // in bytes
    osize pos;
} OStreamMem;


/**
 * Initializes the object
 * @param obj OStreamMem object
 * @param parent to inherit from
 * @param memory The static memory to work on
 * @param memory_size size of the memory block, if <0: o_strlen(memory) is used instead
 * @return obj casted as OStreamMem
 */
O_EXTERN
OStreamMem *OStreamMem_init(oobj obj, oobj parent, void *memory, osize memory_size);


/**
 * Creates a new OStreamMem object
 * @param parent to inherit from
 * @param memory The static memory to work on
 * @param memory_size size of the memory block, if <0: o_strlen(memory) is used instead
 * @return The new object
 */
O_INLINE
OStreamMem *OStreamMem_new(oobj parent, void *memory, osize memory_size)
{
    OObj_DECL_IMPL_NEW(OStreamMem, parent, memory, memory_size);
}

//
// virtual implementations:
//

O_EXTERN
bool OStreamMem__v_valid(oobj obj);

O_EXTERN
osize OStreamMem__v_size(oobj obj);

O_EXTERN
osize OStreamMem__v_seek(oobj obj, osize offset, enum OStream_seek_whence whence);

O_EXTERN
osize OStreamMem__v_read_try(oobj obj, void *data, osize element_size, osize num);

O_EXTERN
osize OStreamMem__v_write(oobj obj, const void *data, osize element_size, osize num);

O_EXTERN
bool OStreamMem__v_close(oobj obj);

#endif //O_OSTREAMMEM_H
