#ifdef MIA_OPTION_SOCKET

#include "o/OStreamSocket.h"
#include "o/OObj_builder.h"
#include "SDL2/SDL_net.h"

#define O_LOG_LIB "o"
#include "o/log.h"


OStreamSocket *OStreamSocket_init(oobj obj, oobj parent, struct _TCPsocket *socket_sink)
{
    OStreamSocket *self = obj;
    o_clear(self, sizeof *self, 1);

    OStream_init(obj, parent,
                 OStreamSocket__v_valid,
                 OStreamSocket__v_size,
                 OStreamSocket__v_seek,
                 OStreamSocket__v_read_try,
                 OStreamSocket__v_write,
                 OStreamSocket__v_close);
    OObj_id_set(self, OStreamSocket_ID);

    // set the sink
    self->socket = socket_sink;

    return self;
}


bool OStreamSocket__v_valid(oobj obj)
{
    OObj_assert(obj, OStreamSocket);
    OStreamSocket *self = obj;
    return self->socket != NULL;
}

osize OStreamSocket__v_size(oobj obj)
{
    // a socket stream has no size
    return -1;
}

osize OStreamSocket__v_seek(oobj obj, osize offset, enum OStream_seek_whence whence)
{
    // a socket stream cant be seeked
    return -1;
}

osize OStreamSocket__v_read_try(oobj obj, void *out_data, osize element_size, osize num)
{
    OObj_assert(obj, OStreamSocket);
    OStreamSocket *self = obj;
    if (!self->socket)
        return 0;

    osize bytes_num = element_size * num;
    int bytes_num_int = (int) bytes_num;
    assert(bytes_num >= 0 && bytes_num_int == bytes_num);

    osize read = SDLNet_TCP_Recv(self->socket, out_data, bytes_num_int);

    osize read_elements = read / element_size;
    int remaining = (int) (element_size - (read % element_size));

    // hopefully, we wont ever get here...
    if(remaining != element_size) {
        o_log_debug_s("OStreamSocket_read_try", "need remaining bytes of an element: %i/%i",
                    remaining, (int) element_size);

        obyte *it = out_data;
        it += read;

        // add the remaining read element
        read_elements++;

        while(remaining > 0) {
            int rem_read = SDLNet_TCP_Recv(self->socket, it, remaining);
            if(rem_read <= 0) {
                o_log_debug_s("OStreamSocket_read_try", "failed reading the remaining bytes",
                           remaining, (int) element_size);
                read_elements = 0;
                break;
            }
            remaining -= rem_read;
            it += rem_read;
        }
    }

    if (read_elements <= 0) {
        OStream_close(self);
    }
    return read_elements;
}

osize OStreamSocket__v_write(oobj obj, const void *data, osize element_size, osize num)
{
    OObj_assert(obj, OStreamSocket);
    OStreamSocket *self = obj;
    if (!self->socket)
        return 0;

    osize bytes_num = element_size * num;
    int bytes_num_int = (int) bytes_num;
    assert(bytes_num >= 0 && bytes_num_int == bytes_num);

    osize written = SDLNet_TCP_Send(self->socket, data, bytes_num_int);

    osize elements_written = written / element_size;

    if (elements_written < num) {
        OStream_close(self);
    }
    return elements_written;
}

bool OStreamSocket__v_close(oobj obj)
{
    OObj_assert(obj, OStreamSocket);
    OStreamSocket *self = obj;
    if (!self->socket)
        return true;

    // cant fail?
    SDLNet_TCP_Close(self->socket);
    self->socket = NULL;
    return true;
}


#endif // MIA_OPTION_SOCKET
typedef int avoid_empty_translation_unit;

