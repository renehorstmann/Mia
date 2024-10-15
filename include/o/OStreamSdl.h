#ifndef O_OSTREAMSDL_H
#define O_OSTREAMSDL_H

/**
 * @file OStreamSdl.h
 *
 * Object
 *
 * OStream implementation based on SDL_RWops
 * @sa name.h for functions that open files
 */

#include "OStream.h"

/** object id */
#define OStreamSdl_ID OStream_ID "Sdl"

// forward declaration
struct SDL_RWops;


typedef struct {
    OStream super;

    struct SDL_RWops *rwops;
} OStreamSdl;


/**
 * Initializes the object
 * @param obj OStreamSdl object
 * @param parent to inherit from
 * @param rwops_sink SDL_RWops object as sink (will be destroyed | closed by this stream)
 * @return obj casted as OStreamSdl
 */
O_EXTERN
OStreamSdl *OStreamSdl_init(oobj obj, oobj parent, struct SDL_RWops *rwops_sink);


/**
 * Creates a new OStreamSdl object
 * @param parent to inherit from
 * @param rwops_sink SDL_RWops object as sink (will be destroyed | closed by this stream)
 * @return The new object
 */
O_INLINE
OStreamSdl *OStreamSdl_new(oobj parent, struct SDL_RWops *rwops_sink)
{
    OObj_DECL_IMPL_NEW(OStreamSdl, parent, rwops_sink);
}

//
// virtual implementations:
//

O_EXTERN
bool OStreamSdl__v_valid(oobj obj);

O_EXTERN
osize OStreamSdl__v_size(oobj obj);

O_EXTERN
osize OStreamSdl__v_seek(oobj obj, osize offset, enum OStream_seek_whence whence);

O_EXTERN
osize OStreamSdl__v_read_try(oobj obj, void *data, osize element_size, osize num);

O_EXTERN
osize OStreamSdl__v_write(oobj obj, const void *data, osize element_size, osize num);

O_EXTERN
bool OStreamSdl__v_close(oobj obj);

#endif //O_OSTREAMSDL_H
