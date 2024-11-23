#ifdef MIA_OPTION_SOCKET
#ifndef O_OSTREAMSOCKET_H
#define O_OSTREAMSOCKET_H

/**
 * @file OStreamSocket.h
 *
 * Object
 *
 * OStream implementation based ob SDL_net TCPsocket
 * @sa OSocketserver.h (server) and socket.h (client)
 */

#include "OStream.h"

/** object id */
#define OStreamSocket_ID OStream_ID "Socket"

// forward declaration
struct _TCPsocket;


typedef struct {
    OStream super;

    struct _TCPsocket *socket;
} OStreamSocket;


/**
 * Initializes the object
 * @param obj OStreamSocket object
 * @param parent to inherit from
 * @param socket_sink SDL_net TCPsocket object as sink (will be destroyed | closed by this stream)
 * @return obj casted as OStreamSocket
 */
O_EXTERN
OStreamSocket *OStreamSocket_init(oobj obj, oobj parent, struct _TCPsocket *socket_sink);


/**
 * Creates a new OStreamSocket object
 * @param parent to inherit from
 * @param socket_sink SDL_net TCPsocket object as sink (will be destroyed | closed by this stream)
 * @return The new object
 */
O_INLINE
OStreamSocket *OStreamSocket_new(oobj parent, struct _TCPsocket *socket_sink)
{
    OObj_DECL_IMPL_NEW(OStreamSocket, parent, socket_sink);
}

//
// virtual implementations:
//

O_EXTERN
bool OStreamSocket__v_valid(oobj obj);

O_EXTERN
osize OStreamSocket__v_size(oobj obj);

O_EXTERN
osize OStreamSocket__v_seek(oobj obj, osize offset, enum OStream_seek_whence whence);

O_EXTERN
osize OStreamSocket__v_read_try(oobj obj, void *data, osize element_size, osize num);

O_EXTERN
osize OStreamSocket__v_write(oobj obj, const void *data, osize element_size, osize num);

O_EXTERN
bool OStreamSocket__v_close(oobj obj);



#endif //O_OSTREAMSOCKET_H
#endif //MIA_OPTION_SOCKET
