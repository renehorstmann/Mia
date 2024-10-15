#ifdef MIA_OPTION_SOCKET

#include "o/OStreamSocket.h"
#include "o/str.h"
#include "SDL2/SDL_net.h"

#define O_LOG_LIB "o"
#include "o/log.h"


struct oobj_opt o_socket_open(oobj parent, const char *address, ou16 port)
{
    if(!address) {
        address = "127.0.0.1";
    }

    IPaddress ip;
    if(SDLNet_ResolveHost(&ip, address, port) == -1) {
        o_log_warn_s(__func__, "failed to resolve host: %s", SDLNet_GetError());
        return oobj_opt(NULL);
    }

    TCPsocket socket = SDLNet_TCP_Open(&ip);
    // impl->so will be NULL on error, so _valid check would fail

    if(!socket) {
        o_log_warn_s(__func__, "failed to create the connection");
        return oobj_opt(NULL);
    }

    oobj stream = OStreamSocket_new(parent, socket);

    char buf[64];
    o_strf_buf(buf, "Socket#%s:%i", address, port);
    OObj_name_set(stream, buf);

    return oobj_opt(stream);
}



#endif // MIA_OPTION_SOCKET
typedef int avoid_empty_translation_unit;

