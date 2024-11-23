#ifdef MIA_OPTION_SOCKET
#ifndef O_SOCKET_H
#define O_SOCKET_H

/**
 * @file socket.h
 *
 * Open a client tcp socket as stream
 */

#include "OStream.h"

/**
 * Opens a connection to a TCP server as client
 * @param parent OStream will be a resource of parent.
 * @param address ip address of the server (pass NULL for localhost, aka "127.0.0.1"
 * @param port the port of the server
 * @return an OStream object to read and write with the name. NULL on error
 */
O_EXTERN
struct oobj_opt o_socket_open(oobj parent, const char *address, ou16 port);


#endif //O_SOCKET_H
#endif //MIA_OPTION_SOCKET
