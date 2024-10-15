#ifndef O_OSTREAMBUFFERED_H
#define O_OSTREAMBUFFERED_H

/**
 * @file OStreamBuffered.h
 *
 * Object
 *
 * OStream implementation which adds a reading buffer to an existing stream
 * so always seekable within the buffer
 */

#include "OStream.h"

/** object id */
#define OStreamBuffered_ID OStream_ID "Buffered"


typedef struct {
    OStream super;

    OStream *stream;
    bool auto_close;

    // ring buffer
    obyte *buffer;
    osize buffer_size;

    // relative to buffer
    osize start;

    // internal valid buffer [0:buffer_size]
    osize size;

    // position relative to start
    //      so the current byte read is buffer[mod(start + pos, buffer_size)]
    osize pos;

} OStreamBuffered;


/**
 * Initializes the object
 * @param obj OStreamBuffered object
 * @param parent to inherit from
 * @param stream The OStream to work on
 * @param buffer_size The buffer size in which seeks work
 * @param auto_close calls close on the stream if this stream is closed
 * @return obj casted as OStreamBuffered
 */
O_EXTERN
OStreamBuffered *OStreamBuffered_init(oobj obj, oobj parent, oobj stream, osize buffer_size, bool auto_close);


/**
 * Creates a new OStreamBuffered object
 * @param parent to inherit from
 * @param stream The OStream to work on
 * @param buffer_size The buffer size in which seeks work
 * @param auto_close calls close on the stream if this stream is closed
 * @return The new object
 */
O_INLINE
OStreamBuffered *OStreamBuffered_new(oobj parent, oobj stream, osize buffer_size, bool auto_close)
{
    OObj_DECL_IMPL_NEW(OStreamBuffered, parent, stream, buffer_size, auto_close);
}

//
// virtual implementations:
//

O_EXTERN
bool OStreamBuffered__v_valid(oobj obj);

O_EXTERN
osize OStreamBuffered__v_size(oobj obj);

O_EXTERN
osize OStreamBuffered__v_seek(oobj obj, osize offset, enum OStream_seek_whence whence);

O_EXTERN
osize OStreamBuffered__v_read_try(oobj obj, void *data, osize element_size, osize num);

O_EXTERN
osize OStreamBuffered__v_write(oobj obj, const void *data, osize element_size, osize num);

O_EXTERN
bool OStreamBuffered__v_close(oobj obj);



#endif //O_OSTREAMBUFFERED_H
