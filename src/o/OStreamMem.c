#include "o/OStreamMem.h"
#include "o/OObj_builder.h"

OStreamMem *OStreamMem_init(oobj obj, oobj parent, void *memory, osize memory_size)
{
    OStreamMem *self = obj;
    o_clear(self, sizeof *self, 1);

    OStream_init(obj, parent,
                 OStreamMem__v_valid,
                 OStreamMem__v_size,
                 OStreamMem__v_seek,
                 OStreamMem__v_read_try,
                 OStreamMem__v_write,
                 OStreamMem__v_close);
    OObj_id_set(self, OStreamMem_ID);

    self->memory = memory;
    self->memory_size = memory_size>=0? memory_size : o_strlen(memory);

    return self;
}


bool OStreamMem__v_valid(oobj obj)
{
    OObj_assert(obj, OStreamMem);
    OStreamMem *self = obj;
    return self->memory;
}

osize OStreamMem__v_size(oobj obj)
{
    OObj_assert(obj, OStreamMem);
    OStreamMem *self = obj;
    if (!self->memory) {
        return -1;
    }
    return self->memory_size;
}

osize OStreamMem__v_seek(oobj obj, osize offset, enum OStream_seek_whence whence)
{
    OObj_assert(obj, OStreamMem);
    OStreamMem *self = obj;
    if (!self->memory) {
        return -1;
    }
    osize pos = self->pos;
    osize size = self->memory_size;
    if (whence == OStream_SEEK_SET) {
        pos = offset;
    } else if (whence == OStream_SEEK_CUR) {
        pos += offset;
    } else if (whence == OStream_SEEK_END) {
        pos = size + offset;
    }
    self->pos = o_clamp(pos, 0, size);
    return self->pos;
}

osize OStreamMem__v_read_try(oobj obj, void *out_data, osize element_size, osize num)
{
    OObj_assert(obj, OStreamMem);
    OStreamMem *self = obj;
    if (!self->memory) {
        return 0;
    }
    osize size = self->memory_size;
    self->pos = o_clamp(self->pos, 0, size);

    osize num_left = (size - self->pos) / element_size;
    osize num_copy = o_min(num_left, num);

    obyte *mem = self->memory;
    o_memcpy(out_data, mem + self->pos, element_size, num_copy);
    self->pos += num_copy * element_size;
    return num_copy;
}

osize OStreamMem__v_write(oobj obj, const void *data, osize element_size, osize num)
{
    OObj_assert(obj, OStreamMem);
    OStreamMem *self = obj;
    if (!self->memory) {
        return 0;
    }
    osize size = self->memory_size;
    self->pos = o_clamp(self->pos, 0, size);

    osize num_left = (size - self->pos) / element_size;

    obyte *mem = self->memory;
    o_memcpy(mem + self->pos, data, element_size, num_left);
    self->pos += num_left * element_size;

    return num_left;
}

bool OStreamMem__v_close(oobj obj)
{
    OObj_assert(obj, OStreamMem);
    OStreamMem *self = obj;
    self->memory = NULL;
    self->memory_size = 0;
    return true;
}
