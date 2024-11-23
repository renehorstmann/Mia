#include "o/OStreamArray.h"
#include "o/OObj_builder.h"
#include "o/OArray.h"

OStreamArray *OStreamArray_init(oobj obj, oobj parent, oobj array, bool move_array, enum OStreamArray_mode mode)
{
    OStreamArray *self = obj;
    o_clear(self, sizeof *self, 1);

    OStream_init(obj, parent,
                 OStreamArray__v_valid,
                 OStreamArray__v_size,
                 OStreamArray__v_seek,
                 OStreamArray__v_read_try,
                 OStreamArray__v_write,
                 OStreamArray__v_close);
    OObj_id_set(self, OStreamArray_ID);

    OObj_assert(array, OArray);
    self->array = array;
    if(move_array) {
        o_move(array, self);
    }
    self->mode = mode;

    return self;
}


bool OStreamArray__v_valid(oobj obj)
{
    OObj_assert(obj, OStreamArray);
    OStreamArray *self = obj;
    return self->array;
}

osize OStreamArray__v_size(oobj obj)
{
    OObj_assert(obj, OStreamArray);
    OStreamArray *self = obj;
    if (!self->array) {
        return -1;
    }
    return OArray_byte_size(self->array);
}

osize OStreamArray__v_seek(oobj obj, osize offset, enum OStream_seek_whence whence)
{
    OObj_assert(obj, OStreamArray);
    OStreamArray *self = obj;
    if (!self->array || self->mode != OStreamArray_SEEKABLE) {
        return -1;
    }
    osize pos = self->pos;
    osize size = OArray_byte_size(self->array);
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

osize OStreamArray__v_read_try(oobj obj, void *out_data, osize element_size, osize num)
{
    OObj_assert(obj, OStreamArray);
    OStreamArray *self = obj;
    if (!self->array) {
        return 0;
    }
    if(self->mode == OStreamArray_SEEKABLE) {
        osize size = OArray_byte_size(self->array);
        self->pos = o_clamp(self->pos, 0, size);

        osize num_left = (size - self->pos) / element_size;
        osize num_copy = o_min(num_left, num);

        obyte *mem = OArray_data_void(self->array);
        o_memcpy(out_data, mem + self->pos, element_size, num_copy);
        self->pos += num_copy * element_size;
        return num_copy;
    }
    if(self->mode == OStreamArray_FIFO || self->mode == OStreamArray_LIFO_FRONT) {
        assert(element_size == OArray_element_size(self->array));
        self->pos = 0;
        osize num_take = o_min(OArray_num(self->array), num);
        OArray_take_front(self->array, out_data, num_take);
        return num_take;
    }
    if(self->mode == OStreamArray_LIFO || self->mode == OStreamArray_FIFO_FRONT) {
        assert(element_size == OArray_element_size(self->array));
        self->pos = 0;
        osize num_take = o_min(OArray_num(self->array), num);
        OArray_take(self->array, out_data, num_take);
        return num_take;
    }
    assert(false && "invalid mode?");
    return 0;
}

osize OStreamArray__v_write(oobj obj, const void *data, osize element_size, osize num)
{
    OObj_assert(obj, OStreamArray);
    OStreamArray *self = obj;
    if (!self->array) {
        return 0;
    }
    if(self->mode == OStreamArray_SEEKABLE) {
        osize size = OArray_byte_size(self->array);
        self->pos = o_clamp(self->pos, 0, size);

        osize num_left = (size - self->pos) / element_size;

        // we need more room...
        if (num_left < num) {
            // element_size of data may differ...
            osize data_num = OArray_num(self->array);
            osize data_element_size = OArray_element_size(self->array);
            osize bytes_more = (num - num_left) * element_size;
            data_num += bytes_more / data_element_size;
            if (bytes_more % data_element_size != 0) {
                data_num++;
            }
            OArray_resize(self->array, data_num);
        }

        obyte *mem = OArray_data_void(self->array);
        o_memcpy(mem + self->pos, data, element_size , num);
        self->pos += num * element_size;
        return num;
    }
    if(self->mode == OStreamArray_FIFO || self->mode == OStreamArray_LIFO) {
        assert(element_size == OArray_element_size(self->array));
        OArray_append(self->array, data, num);
        return num;
    }
    if(self->mode == OStreamArray_FIFO_FRONT || self->mode == OStreamArray_LIFO_FRONT) {
        assert(element_size == OArray_element_size(self->array));
        OArray_append_front(self->array, data, num);
        return num;
    }
    assert(false && "invalid mode?");
    return 0;
}

bool OStreamArray__v_close(oobj obj)
{
    OObj_assert(obj, OStreamArray);
    OStreamArray *self = obj;
    self->array = NULL;
    return true;
}
