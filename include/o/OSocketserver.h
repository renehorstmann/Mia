#ifdef MIA_OPTION_SOCKET
#ifndef O_OSOCKETSERVER_H
#define O_OSOCKETSERVER_H

/**
 * @file OSocketserver.h
 *
 * Object
 *
 * A SocketServer object starts a TCP server and accepts clients as OStreamSocket
 */


#include "OObj.h"
#include "OStream.h"

/** object id */
#define OSocketserver_ID OObj_ID "OSocketserver"


// forward declaration
struct _TCPsocket;

typedef struct {
    OObj super;

    struct _TCPsocket *socket;
} OSocketserver;


/**
 * Initializes the object
 * @param obj OSocketserver object
 * @param parent to inherit from
 * @param port the port for the server
 * @return obj casted as OSocketserver
 */
O_EXTERN
OSocketserver *OSocketserver_init(oobj obj, oobj parent, ou16 port);

/**
 * Creates a new the OSocketserver object
 * @param parent to inherit from
 * @param port the port for the server
 * @return The new object
 */
O_INLINE
OSocketserver *OSocketserver_new(oobj parent, ou16 port)
{
    OObj_DECL_IMPL_NEW(OSocketserver, parent, port);
}


//
// object functions:
//


/**
 * @param obj OSocketStream object
 * @return true if valid
 */
O_INLINE
bool OSocketserver_valid(oobj obj)
{
    OObj_assert(obj, OSocketserver);
    OSocketserver *self = obj;
    return self->socket != NULL;
}


/**
 * Accepts a client, if available, does not block
 * @param obj OSocketserver object
 * @return an OStream object of the client to read and write with the name, or NULL if not available
 */
O_EXTERN
struct oobj_opt OSocketserver_accept_try(oobj obj);


/**
 * Accepts a client, blocks until connected
 * @param obj OSocketserver object
 * @return an OStream object of the client to read and write with the name.
 */
O_EXTERN
OStream *OSocketserver_accept(oobj obj);


#endif //O_OSOCKETSERVER_H
#endif //MIA_OPTION_SOCKET
