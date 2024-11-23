#include "o/OMap.h"
#include "o/OObj_builder.h"
#include "o/OArray.h"
#include "o/str.h"


//
// key string implementations
//

O_STATIC
ou32 key_string_hash(const void *key)
{
    const char * const * str = key;
    return o_str_hash(*str);
}

O_STATIC
bool key_string_equals(const void *key_a, const void *key_b)
{
    const char * const * str_a = key_a;
    const char * const * str_b = key_b;
    return o_str_equals(*str_a, *str_b);
}

O_STATIC
void *key_string_clone(oobj obj, const void *key)
{
    const char * const * str = key;
    return o_str_clone(obj, *str);
}


OMap *OMap_init(oobj obj, oobj parent, osize key_size, osize value_size, osize approx_num,
                OMap__key_hash_fn hash_fn, OMap__key_equals_fn equals_fn, OMap__key_clone_fn clone_fn)
{
    OMap *self = obj;
    o_clear(self, sizeof *self, 1);

    OObj_init(self, parent);
    OObj_id_set(self, OMap_ID);

    self->key_size = key_size;
    self->value_size = value_size;

    // pairs of key and value elements
    osize key_size_aligned = o_align_size_max(key_size);
    osize value_size_aligned = o_align_size_max(value_size);
    self->pairs = OArray_new_dyn(self, NULL, key_size_aligned + value_size_aligned, 0, approx_num);

    // the hashmap is at first a list of possible DataDyn arrays
    self->hashmap = OArray_new(self, NULL, sizeof(oobj), approx_num);

    // vfuncs
    self->super.v_op_num = OMap__v_op_num;
    self->super.v_op_at = OMap__v_op_at;
    self->v_hash = hash_fn;
    self->v_equals = equals_fn;
    self->v_clone = clone_fn;
    return self;
}

osize OMap__v_op_num(oobj obj)
{
    return OMap_num(obj);
}

void *OMap__v_op_at(oobj obj, osize idx)
{
    return OMap_value_at_void(obj, idx);
}


OMap *OMap_new_string_keys(oobj parent, osize value_size, osize approx_num)
{
    return OMap_new(parent, sizeof(char *), value_size, approx_num,
                     key_string_hash, key_string_equals, key_string_clone);
}

osize OMap_num(oobj obj)
{
    OObj_assert(obj, OMap);
    OMap *self = obj;
    return OArray_num(self->pairs);
}

const void *OMap_key_at_void(oobj obj, osize idx)
{
    OObj_assert(obj, OMap);
    OMap *self = obj;
    return OArray_at_void(self->pairs, idx);
}

void *OMap_value_at_void(oobj obj, osize idx)
{
    OObj_assert(obj, OMap);
    OMap *self = obj;
    obyte *kv_pair = OArray_at_void(self->pairs, idx);
    obyte *value = kv_pair + o_align_size_max(self->key_size);
    return value;
}

osize OMap_get_idx(oobj obj, const void *key)
{
    OObj_assert(obj, OMap);
    OMap *self = obj;
    ou32 hash = self->v_hash(key);

    // the first hashmap returns a list of keys that may fit
    // list is an OArray of osize elements, which are indices into the self->pairs list
    oobj list = *OArray_at(self->hashmap, hash % OArray_num(self->hashmap), oobj);
    if (!list) {
        // list under that hash% not available, so pair not available, too
        return -1;
    }

    // search in the list for the key
    osize idx = -1;
    for (osize i = 0; i < OArray_num(list); i++) {
        osize test_idx = *OArray_at(list, i, osize);
        if (self->v_equals(key, OMap_key_at_void(self, test_idx))) {
            idx = test_idx;
            break;
        }
    }

    // -1 if failed, else the valid idx
    return idx;
}

osize OMap_set(oobj obj, const void *key, const void *value)
{
    OObj_assert(obj, OMap);
    OMap *self = obj;
    ou32 hash = self->v_hash(key);

    // the first hashmap returns a list of keys that may fit
    // list is an OArray of osize elements, which are indices into the self->pairs list
    oobj *list_ptr = OArray_at(self->hashmap, hash % OArray_num(self->hashmap), oobj);
    if (!*list_ptr) {
        // list under that hash% not created yet:
        *list_ptr = OArray_new_dyn(self, NULL, sizeof(osize), 0, 4);
    }

    // search in the list for the key
    osize idx = -1;
    for (osize i = 0; i < OArray_num(*list_ptr); i++) {
        osize test_idx = *OArray_at(*list_ptr, i, osize);
        if (self->v_equals(key, OMap_key_at_void(self, test_idx))) {
            idx = test_idx;
            break;
        }
    }

    // key not found, add it
    if (idx < 0) {
        // add a pair and copy the key to it
        void *kv_pair = OArray_push(self->pairs, NULL);
        char **new_key_ptr = kv_pair;
        *new_key_ptr = self->v_clone(self, key);
        // update idx to the new pair and add it to the list
        idx = OArray_num(self->pairs) - 1;
        OArray_push(*list_ptr, &idx);
    }

    // set the value
    o_memcpy(OMap_value_at_void(self, idx), value, 1, self->value_size);
    return idx;
}

bool OMap_remove(oobj obj, const void *key)
{
    OObj_assert(obj, OMap);
    OMap *self = obj;
    ou32 hash = self->v_hash(key);

    // the first hashmap returns a list of keys that may fit
    // list is an OArray of osize elements, which are indices into the self->pairs list
    oobj list = *OArray_at(self->hashmap, hash % OArray_num(self->hashmap), oobj);
    if (!list) {
        // list under that hash% not available, so pair not available, too
        return false;
    }

    // search in the list for the key
    osize idx = -1;
    osize list_i = 0;
    for (osize i = 0; i < OArray_num(list); i++) {
        osize test_idx = *OArray_at(list, i, osize);
        if (self->v_equals(key, OMap_key_at_void(self, test_idx))) {
            idx = test_idx;
            list_i = i;
            break;
        }
    }

    // key not found
    if (idx < 0) {
        return false;
    }

    // remove the key value pair
    OArray_pop_at(self->pairs, idx, NULL);


    // remove the idx entry in the list
    OArray_pop_at(list, list_i, NULL);

    // traverse the whole hashmap to reduce indices, greater the removed idx
    for (osize i = 0; i < OArray_num(self->hashmap); i++) {
        list = *OArray_at(self->hashmap, i, oobj);
        if (!list) {
            continue;
        }
        for (osize l = 0; l < OArray_num(list); l++) {
            osize *index = OArray_at(list, l, osize);

            // check if index>idx and reduce it by one
            if (*index > idx) {
                (*index)--;
            }
        }
    }

    return true;
}
