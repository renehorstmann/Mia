#include "o/OStreamSdl.h"
#include "o/OObj_builder.h"
#include <SDL2/SDL_rwops.h>

OStreamSdl *OStreamSdl_init(oobj obj, oobj parent, struct SDL_RWops *rwops_sink)
{
    OStreamSdl *self = obj;
    o_clear(self, sizeof *self, 1);

    OStream_init(obj, parent,
                 OStreamSdl__v_valid,
                 OStreamSdl__v_size,
                 OStreamSdl__v_seek,
                 OStreamSdl__v_read_try,
                 OStreamSdl__v_write,
                 OStreamSdl__v_close);
    OObj_id_set(self, OStreamSdl_ID);

    // set the sink
    self->rwops = rwops_sink;

    return self;
}


bool OStreamSdl__v_valid(oobj obj)
{
    OObj_assert(obj, OStreamSdl);
    OStreamSdl *self = obj;
    return self->rwops != NULL;
}

osize OStreamSdl__v_size(oobj obj)
{
    OObj_assert(obj, OStreamSdl);
    OStreamSdl *self = obj;
    if (!self->rwops)
        return -1;
    return SDL_RWsize(self->rwops);
}

osize OStreamSdl__v_seek(oobj obj, osize offset, enum OStream_seek_whence whence)
{
    OObj_assert(obj, OStreamSdl);
    OStreamSdl *self = obj;
    if (!self->rwops)
        return -1;

    // should be 0, 1, 2, just in case...
    return SDL_RWseek(self->rwops, offset, (int[]) {RW_SEEK_SET, RW_SEEK_CUR, RW_SEEK_END}[whence]);
}

osize OStreamSdl__v_read_try(oobj obj, void *out_data, osize element_size, osize num)
{
    OObj_assert(obj, OStreamSdl);
    OStreamSdl *self = obj;
    if (!self->rwops)
        return 0;
    osize read = (osize) SDL_RWread(self->rwops, out_data, element_size, num);
    if (read <= 0) {
        OStream_close(self);
    }
    return read;
}

osize OStreamSdl__v_write(oobj obj, const void *data, osize element_size, osize num)
{
    OObj_assert(obj, OStreamSdl);
    OStreamSdl *self = obj;
    if (!self->rwops)
        return 0;
    osize written = (osize) SDL_RWwrite(self->rwops, data, element_size, num);
    if (written < num) {
        OStream_close(self);
    }
    return written;
}

bool OStreamSdl__v_close(oobj obj)
{
    OObj_assert(obj, OStreamSdl);
    OStreamSdl *self = obj;
    if (!self->rwops)
        return true;
    int ret = SDL_RWclose(self->rwops);
    self->rwops = NULL;
    return ret == 0;
}
