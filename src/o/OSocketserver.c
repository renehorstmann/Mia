#ifdef MIA_OPTION_SOCKET

#include "o/OSocketserver.h"
#include "o/OObj_builder.h"
#include "o/OStreamSocket.h"
#include "o/str.h"
#include "SDL2/SDL_net.h"

#define O_LOG_LIB "o"
#include "o/log.h"


#define RETRY_DELAY_MS 100


OSocketserver *OSocketserver_init(oobj obj, oobj parent, ou16 port)
{
    OSocketserver *self = obj;
    o_clear(self, sizeof *self, 1);

    OObj_init(self, parent);
    OObj_id_set(self, OSocketserver_ID);

    IPaddress ip;
    if(SDLNet_ResolveHost(&ip, INADDR_ANY, port) == -1) {
        o_log_warn_s(__func__, "failed to resolve host: %s", SDLNet_GetError());
        return self;
    }

    self->socket = SDLNet_TCP_Open(&ip);

    if(!self->socket) {
        o_log_warn_s(__func__, "failed to create the server socket");
        return self;
    }

    char buf[64];
    o_strf_buf(buf, "Server#:%i", port);
    OObj_name_set(self, buf);

    return self;
}


struct oobj_opt OSocketserver_accept_try(oobj obj)
{
    OObj_assert(obj, OSocketserver);
    OSocketserver *self = obj;
    if(!self->socket) {
        return oobj_opt(NULL);
    }

    TCPsocket socket = SDLNet_TCP_Accept(self->socket);
    if(!socket) {
        return oobj_opt(NULL);
    }

    IPaddress *client_ip = SDLNet_TCP_GetPeerAddress(socket);
    if(!client_ip) {
        o_log_debug_s(__func__, "failed to get client ip address");
    } else {
        o_log_s(__func__, "connected with: %s", SDLNet_ResolveIP(client_ip));
    }

    return oobj_opt(OStreamSocket_new(obj, socket));
}


OStream *OSocketserver_accept(oobj obj)
{
    for(;;) {
        struct oobj_opt client = OSocketserver_accept_try(obj);
        if(client.o) {
            return client.o;
        }
        o_sleep(RETRY_DELAY_MS);
    }
}

#endif // MIA_OPTION_SOCKET
typedef int avoid_empty_translation_unit;

