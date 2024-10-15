#include "o/OArray.h"
#include "o/OObj_builder.h"
#include "o/str.h"
#include <stdlib.h> // qsort

#define O_LOG_LIB "o"
#include "o/log.h"


O_INLINE
void reset_reserve_front(OArray *self, osize new_valid_reserve_front)
{
    self->reserve_front = new_valid_reserve_front;
    self->data = &self->memory[self->reserve_front * self->element_size];
}

O_INLINE
void set_termination_element(OArray *self)
{
    obyte *term_start = &self->data[self->num * self->element_size];
    o_clear(term_start, self->element_size, 1);
}


O_STATIC
OArray *array_init_base(oobj obj, oobj parent, const void *opt_data, osize element_size, osize num,
                        osize start_capacity, osize reserve_front, OArray__dyn_realloc_fn realloc_fn)
{
    OArray *self = obj;
    o_clear(self, sizeof *self, 1);

    OObj_init(self, parent);
    OObj_id_set(self, OArray_ID);

    num = o_max(0, num);
    start_capacity = o_max(num, start_capacity);
    reserve_front = o_clamp(reserve_front, 0, start_capacity-num);

    self->element_size = element_size;
    self->capacity = start_capacity;
    self->memory = o_alloc0(self, self->element_size, self->capacity + 1);
    self->num = num;

    reset_reserve_front(self, reserve_front);


    if (opt_data) {
        o_memcpy(self->data, opt_data, element_size, num);
    }
    // termination element already init with 0

    // vfuncs
    self->super.v_op_num = OArray__v_op_num;
    self->super.v_op_at = OArray__v_op_at;
    self->v_dyn_realloc = realloc_fn;
    return self;
}


OArray *OArray_init(oobj obj, oobj parent, const void *opt_data, osize element_size, osize num,
                    osize start_capacity, enum OArray_dyn_realloc_mode mode)
{
    osize reserve_front = 0;
    OArray__dyn_realloc_fn realloc_fn = OArray__v_dyn_realloc;
    switch (mode) {
        case OArray_REALLOC_DEFAULT:
            break;
        case OArray_REALLOC_ASSERT:
            realloc_fn = OArray__v_dyn_realloc_assert;
            break;
        case OArray_REALLOC_DOUBLED:
            realloc_fn = OArray__v_dyn_realloc_doubled;
            break;
        case OArray_REALLOC_DOUBLED_FRONT:
            reserve_front = start_capacity - num;
            realloc_fn = OArray__v_dyn_realloc_doubled_front;
            break;
        case OArray_REALLOC_DOUBLED_CENTER:
            reserve_front = (start_capacity - num) / 2;
            realloc_fn = OArray__v_dyn_realloc_doubled_center;
            break;
        default:
            assert(false && "invalid mode");
    }
    return array_init_base(obj, parent, opt_data, element_size, num,
                           start_capacity, reserve_front, realloc_fn);
}


//
// virtual implementations
//

osize OArray__v_op_num(oobj obj)
{
    return OArray_num(obj);
}

void *OArray__v_op_at(oobj obj, osize idx)
{
    return OArray_at_void(obj, idx);
}

void OArray__v_dyn_realloc(oobj obj, osize min_needed, osize append_front, osize append_back)
{
    // no extra capacity
    OArray_realloc(obj, min_needed, append_front);
}

void OArray__v_dyn_realloc_assert(oobj obj, osize min_needed, osize append_front, osize append_back)
{
    assert(min_needed<=OArray_capacity(obj) && "reallocation turned off!");
    void *mem = OArray_memory(obj);
    OArray_realloc(obj, OArray_capacity(obj), append_front);
    assert(mem == OArray_memory(obj) && "realloc error!");
}

void OArray__v_dyn_realloc_doubled(oobj obj, osize min_needed, osize append_front, osize append_back)
{
    // doubled capacity at back
    osize cap = min_needed*2;
    OArray_realloc(obj, o_max(cap, OArray_capacity(obj)), append_front);
}

void OArray__v_dyn_realloc_doubled_front(oobj obj, osize min_needed, osize append_front, osize append_back)
{
    // doubled capacity at front, reserve_front is as much to right as possible
    osize cap = min_needed*2;
    OArray_realloc(obj, o_max(cap, OArray_capacity(obj)), min_needed * 2 - OArray_num(obj) - append_back);
}


void OArray__v_dyn_realloc_doubled_center(oobj obj, osize min_needed, osize append_front, osize append_back)
{
    // doubled capacity at front, reserve_front layed so that the data is nearly centered
    osize cap = min_needed*2;
    OArray_realloc(obj, o_max(cap, OArray_capacity(obj)), min_needed - OArray_num(obj) / 2 + append_front - append_back);
}

//
// object functions
//

void OArray_element_size_reset(oobj obj, osize new_element_size)
{
    OObj_assert(obj, OArray);
    OArray *self = obj;
    osize byte_size = OArray_byte_size(obj);
    assert(byte_size % new_element_size == 0 && "must match the data size!");
    self->element_size = new_element_size;
    self->num = byte_size / new_element_size;
}

void OArray_reserve_shift(oobj obj, osize reserve_front)
{
    OObj_assert(obj, OArray);
    OArray *self = obj;

    reserve_front = o_max(reserve_front, 0);
    if (self->reserve_front == reserve_front) {
        return;
    }
    osize new_num = self->capacity - reserve_front;
    if (new_num > 0) {
        self->num = o_min(new_num, self->num);
        self->reserve_front = reserve_front;
        o_memmove(&self->memory[self->reserve_front*self->element_size], self->data, self->element_size, self->num);
    } else {
        self->num = 0;
        self->reserve_front = self->capacity;
    }
    reset_reserve_front(self, self->reserve_front);
    set_termination_element(self);
}

void OArray_realloc(oobj obj, osize capacity, osize reserve_front)
{
    OObj_assert(obj, OArray);
    OArray *self = obj;

    capacity = o_max(capacity, 0);

    if (capacity < self->capacity) {
        // shrinken array, o_memmove before reallocation
        OArray_reserve_shift(self, reserve_front);
    }

    if (self->capacity != capacity) {
        self->capacity = capacity;
        self->memory = o_realloc(self, self->memory, self->element_size, self->capacity + 1);
        self->num = o_min(self->num, self->capacity);
        reset_reserve_front(self, self->reserve_front);
    }

    // widen array, or reserve_front only, o_memmove after reallocation
    OArray_reserve_shift(self, reserve_front);
}

void OArray_dyn_realloc_mode_set(oobj obj, enum OArray_dyn_realloc_mode mode)
{
    switch (mode) {
        case OArray_REALLOC_DEFAULT:
            OArray_dyn_realloc_fn_set(obj, OArray__v_dyn_realloc);
            return;
        case OArray_REALLOC_ASSERT:
            OArray_dyn_realloc_fn_set(obj, OArray__v_dyn_realloc_assert);
            return;
        case OArray_REALLOC_DOUBLED:
            OArray_dyn_realloc_fn_set(obj, OArray__v_dyn_realloc_doubled);
            return;
        case OArray_REALLOC_DOUBLED_FRONT:
            OArray_dyn_realloc_fn_set(obj, OArray__v_dyn_realloc_doubled_front);
            return;
        case OArray_REALLOC_DOUBLED_CENTER:
            OArray_dyn_realloc_fn_set(obj, OArray__v_dyn_realloc_doubled_center);
            return;
        default:
            assert(false && "invalid mode");
    }
}

void OArray_resize(oobj obj, osize num) {
    OObj_assert(obj, OArray);
    OArray *self = obj;

    num = o_max(0, num);
    if(num == self->num) {
        return;
    }

    // popping elements
    if(num < self->num) {
        self->num = num;
        set_termination_element(self);
        return;
    }

    // adding elements in capacity
    osize add = num - self->num;
    osize room_left = OArray_reserve(self);
    if(add <= room_left) {
        self->num = num;
        set_termination_element(self);
        return;
    }

    // actual resize
    osize append = add - room_left;
    self->v_dyn_realloc(self, num, 0, append);
    self->num = num;
    set_termination_element(self);
}

void OArray_resize_front(oobj obj, osize num) {
    OObj_assert(obj, OArray);
    OArray *self = obj;

    num = o_max(0, num);
    if(num == self->num) {
        return;
    }

    // popping elements
    if(num < self->num) {
        osize pop = self->num - num;
        self->num = num;
        reset_reserve_front(self, self->reserve_front + pop);
        set_termination_element(self);
        return;
    }

    // adding elements in capacity
    osize add = num - self->num;
    osize room_left = self->reserve_front;
    if(add <= room_left) {
        self->num = num;
        reset_reserve_front(self, self->reserve_front-add);
        set_termination_element(self);
        return;
    }

    // actual resize
    osize append = add - room_left;
    self->v_dyn_realloc(self, num, append, 0);
    self->num = num;
    reset_reserve_front(self, self->reserve_front-add);
    set_termination_element(self);
}

void OArray_sort(oobj obj, OArray__compare_fn comperator)
{
    OObj_assert(obj, OArray);
    OArray *self = obj;
    qsort(self->data, self->num, self->element_size, comperator);
}

osize OArray_search_sorted(oobj obj, const void *search_element, OArray__compare_fn comperator)
{
    OObj_assert(obj, OArray);
    OArray *self = obj;
    const obyte *searched = (const obyte *) bsearch(search_element,
                                                    self->data, self->num, self->element_size,
                                                    comperator);
    if (!searched) {
        return -1;
    }
    osize bytes_diff = searched - (const obyte *) self->data;
    return bytes_diff / self->element_size;
}

osize OArray_search(oobj obj, const void *search_element, OArray__compare_fn comperator)
{
    OObj_assert(obj, OArray);
    OArray *self = obj;
    for (osize i = 0; i < self->num; i++) {
        if (comperator(search_element, OArray_at_void(self, i)) == 0) {
            return i;
        }
    }
    return -1;
}

void *OArray_append(oobj obj, const void *opt_elements_data, osize num)
{
    if (num <= 0)
        return NULL;

    OObj_assert(obj, OArray);
    OArray *self = obj;
    osize old_num = self->num;
    OArray_resize(obj, self->num + num);

    obyte *new_element_data = &self->data[old_num * self->element_size];
    if (opt_elements_data) {
        o_memcpy(new_element_data, opt_elements_data, self->element_size, num);
    } else {
        o_clear(new_element_data, self->element_size , num);
    }
    return new_element_data;
}

void OArray_take(oobj obj, void *out_opt_elements_data, osize num)
{
    if (num <= 0) {
        return;
    }

    OObj_assert(obj, OArray);
    OArray *self = obj;
    assert(self->num >= num);
    if (out_opt_elements_data) {

        obyte *taken_element_data = &self->data[(self->num - num) * self->element_size];
        o_memcpy(out_opt_elements_data, taken_element_data, self->element_size, num);
    }
    OArray_resize(obj, self->num - num);
}

void *OArray_append_front(oobj obj, const void *opt_elements_data, osize num)
{
    if (num <= 0)
        return NULL;

    OObj_assert(obj, OArray);
    OArray *self = obj;
    OArray_resize_front(obj, self->num + num);

    if (opt_elements_data) {
        o_memcpy(self->data, opt_elements_data, self->element_size, num);
    } else {
        o_clear(self->data, self->element_size, num);
    }
    return self->data;
}

void OArray_take_front(oobj obj, void *out_opt_elements_data, osize num)
{
    if (num <= 0) {
        return;
    }

    OObj_assert(obj, OArray);
    OArray *self = obj;
    assert(self->num >= num);
    if (out_opt_elements_data) {
        o_memcpy(out_opt_elements_data, self->data, self->element_size, num);
    }
    OArray_resize_front(obj, self->num - num);
}

void *OArray_append_at(oobj obj, osize idx, const void *opt_elements_data, osize num)
{
    if (num <= 0)
        return NULL;

    OObj_assert(obj, OArray);
    OArray *self = obj;

    if (idx == 0) {
        return OArray_append_front(self, opt_elements_data, num);
    }
    if (idx == self->num) {
        return OArray_append(self, opt_elements_data, num);
    }

    assert(idx > 0 && idx < self->num);
    OArray_resize(obj, self->num + num);

    obyte *new_element_data = &self->data[idx * self->element_size];
    obyte *shift_element_data = &self->data[(idx + num) * self->element_size];
    osize elements_to_shift = self->num - idx;
    o_memmove(shift_element_data, new_element_data, self->element_size, elements_to_shift);
    if (opt_elements_data) {
        o_memcpy(new_element_data, opt_elements_data, self->element_size, num);
    } else {
        o_clear(new_element_data, self->element_size, num);
    }
    return new_element_data;
}

void OArray_take_at(oobj obj, osize idx, void *out_opt_elements_data, osize num)
{
    if (num <= 0) {
        return;
    }

    OObj_assert(obj, OArray);
    OArray *self = obj;

    if (idx == 0) {
        OArray_take_front(self, out_opt_elements_data, num);
        return;
    }
    if (idx == self->num - num) {
        OArray_take(self, out_opt_elements_data, num);
        return;
    }


    assert(idx > 0 && idx < self->num - num);
    assert(self->num >= num);

    obyte *taken_element_data = &self->data[idx * self->element_size];
    obyte *shift_element_data = &self->data[(idx + num) * self->element_size];
    osize elements_to_shift = self->num - num - idx;
    if (out_opt_elements_data) {
        o_memcpy(out_opt_elements_data, taken_element_data, self->element_size, num);
    }
    o_memmove(taken_element_data, shift_element_data, self->element_size, elements_to_shift);
    OArray_resize(obj, self->num - num);
}

osize OArray_append_stringf_va(oobj obj, const char *format, va_list args)
{
    OObj_assert(obj, OArray);
    OArray *self = obj;
    assert(self->element_size == 1);

    char *text = o_strf_va(obj, format, args);
    osize size = o_strlen(text);
    OArray_append(obj, text, size);
    o_free(obj, text);
    return size;
}

osize OArray_append_stringf(oobj obj, const char *format, ...)
{

    va_list args;
    va_start(args, format);
    osize size = OArray_append_stringf_va(obj, format, args);
    va_end(args);
    return size;
}
