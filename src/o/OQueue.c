#ifdef MIA_OPTION_THREAD

#include "o/OQueue.h"
#include "o/OObj_builder.h"


OQueue *OQueue_init(oobj obj, oobj parent, oobj data)
{
    OQueue *self = obj;
    o_clear(self, sizeof *self, 1);

    OObj_init(self, parent);
    OObj_id_set(self, OQueue_ID);

    OObj_assert(data, OArray);
    self->data = data;

    self->cond = OOCondition_new(self);

    return self;
}

//
// functions
//


void OQueue_signal_close(oobj obj)
{
    OObj_assert(obj, OQueue);
    OQueue *self = obj;
    o_lock_block(obj) {
        self->closed = true;
        // broadcast to all waiting threads
        OCondition_broadcast(self->cond);
    }
}

bool OQueue_closed(oobj obj)
{
    OObj_assert(obj, OQueue);
    OQueue *self = obj;
    bool closed;
    o_lock_block(obj) {
        closed = self->closed;
    }
    return closed;
}


osize OQueue_num(oobj obj)
{
    OObj_assert(obj, OQueue);
    OQueue *self = obj;
    osize size;
    o_lock_block(obj) {
        size = OArray_num(self->data);
    }
    return size;
}


// returns the size of an element
osize OQueue_element_size(oobj obj)
{
    OObj_assert(obj, OQueue);
    OQueue *self = obj;
    osize element_size;
    o_lock_block(obj) {
        element_size = OArray_element_size(self->data);
    }
    return element_size;
}

// returns size * elements_size
osize OQueue_byte_size(oobj obj)
{
    OObj_assert(obj, OQueue);
    OQueue *self = obj;
    osize byte_size;
    o_lock_block(obj) {
        byte_size = OArray_byte_size(self->data);
    }
    return byte_size;
}

void OQueue_element_size_reset(oobj obj, osize new_element_size)
{
    OObj_assert(obj, OQueue);
    OQueue *self = obj;
    o_lock_block(obj) {
        OArray_element_size_reset(self->data, new_element_size);
        OCondition_signal(self->cond);
    }
}

bool OQueue_set(oobj obj, const void *elements, osize num, osize idx)
{
    OObj_assert(obj, OQueue);
    OQueue *self = obj;
    bool ok = false;
    o_lock_block(obj) {
        if (OArray_num(self->data) > idx) {
            ok = true;
            o_memcpy(OArray_at_void(self->data, idx), elements, OArray_element_size(self->data), num);
        }
    }
    return ok;
}

bool OQueue_get(oobj obj, void *out_elements, osize num, osize idx)
{
    OObj_assert(obj, OQueue);
    OQueue *self = obj;
    bool ok = false;
    o_lock_block(obj) {
        if (OArray_num(self->data) > idx) {
            ok = true;
            o_memcpy(out_elements, OArray_at_void(self->data, idx), OArray_element_size(self->data), num);
        }
    }
    return ok;
}


void OQueue_resize(oobj obj, osize num)
{
    OObj_assert(obj, OQueue);
    OQueue *self = obj;
    o_lock_block(obj) {
        OArray_resize(self->data, num);
        OCondition_signal(self->cond);
    }
}


void OQueue_append(oobj obj, const void *opt_elements_data, osize num)
{
    OObj_assert(obj, OQueue);
    OQueue *self = obj;
    o_lock_block(obj) {
        OArray_append(self->data, opt_elements_data, num);
        OCondition_signal(self->cond);
    }
}

bool OQueue_take(oobj obj, void *out_opt_elements_data, osize num)
{
    OObj_assert(obj, OQueue);
    OQueue *self = obj;
    bool valid;
    o_lock_block(obj) {
        // wait until queue got closed, or data is available
        while (!self->closed && OArray_num(self->data) < num) {
            OCondition_wait(self->cond, obj);
        }

        // valid if data is available, even when the queue is closed
        valid = OArray_num(self->data) >= num;
        if(valid) {
            OArray_take(self->data, out_opt_elements_data, num);
        }
    }
    return valid;
}

bool OQueue_take_try(oobj obj, void *out_opt_elements_data, osize num)
{
    OObj_assert(obj, OQueue);
    OQueue *self = obj;
    bool ok = false;
    o_lock_block(obj) {
        if (OArray_num(self->data) >= num) {
            OArray_take(self->data, out_opt_elements_data, num);
            ok = true;
        }
    }
    return ok;
}

void OQueue_append_at(oobj obj, osize idx, const void *opt_elements_data, osize num)
{
    OObj_assert(obj, OQueue);
    OQueue *self = obj;
    o_lock_block(obj) {
        OArray_append_front(self->data, opt_elements_data, num);
        OCondition_signal(self->cond);
    }
}

bool OQueue_take_at(oobj obj, osize idx, void *out_opt_elements_data, osize num)
{
    OObj_assert(obj, OQueue);
    OQueue *self = obj;
    bool valid;
    o_lock_block(obj) {
        // wait until queue got closed, or data is available
        while (!self->closed && OArray_num(self->data) < (idx + num)) {
            OCondition_wait(self->cond, obj);
        }

        // valid if data is available, even when the queue is closed
        valid = OArray_num(self->data) >= (idx + num);
        if(valid) {
            OArray_take_at(self->data, idx, out_opt_elements_data, num);
        }
    }
    return valid;
}

bool OQueue_take_at_try(oobj obj, osize idx, void *out_opt_elements_data, osize num)
{
    OObj_assert(obj, OQueue);
    OQueue *self = obj;
    bool ok = false;
    o_lock_block(obj) {
        if (OArray_num(self->data) >= (idx + num)) {
            OArray_take_at(self->data, idx, out_opt_elements_data, num);
            ok = true;
        }
    }
    return ok;
}


#endif // MIA_OPTION_THREAD
typedef int avoid_empty_translation_unit;
