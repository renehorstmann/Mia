#ifndef O_OARRAY_H
#define O_OARRAY_H

/**
 * @file OArray.h
 *
 * Object
 *
 * Holds a mutable (always null terminated) array of elements.
 * can also be used as a string.
 *
 * Operators:
 * o_num -> OArray_num
 * o_at -> OArray_at
 */


#include "OObj.h"

/** object id */
#define OArray_ID OObj_ID "OArray"


/**
 * Some dynamic modes for OArray_new_dyn_mode or OArray_dyn_realloc_mode_set.
 * Sets the virtual v_dyn_realloc function.
 */
enum OArray_dyn_realloc_mode {
    OArray_REALLOC_DEFAULT,
    OArray_REALLOC_ASSERT,
    OArray_REALLOC_DOUBLED,
    OArray_REALLOC_DOUBLED_FRONT,
    OArray_REALLOC_DOUBLED_CENTER,
    OArray_REALLOC_NUM_MODES,
};

/**
 * Virtual function that reallocates a new capacity
 * @param obj OArray object
 * @param min_needed is the minimal needed capacity to store the array
 * @param append_front, append_back elements reallocation must append minimum at front/back
 * @note calls OArray_realloc internally for the reallocation
 */
typedef void (*OArray__dyn_realloc_fn)(oobj obj, osize min_needed, osize append_front, osize append_back);

/**
 * Compare function to compare two elements in the OArray array.
 * @param a Element
 * @param b Other element
 * @return ("a" - "b") to sort in ascending order
 *         <0: a is less than b
 *         0:  equal
 *         >0: a is greater than b
 */
typedef int (*OArray__compare_fn)(const void *a, const void *b);


typedef struct {
    OObj super;

    // allocated memory, most of the time equal to data
    obyte *memory;

    osize element_size;

    // size of allocated memory buffer (counted in elements)
    osize capacity;
    // offsets (counted in elements) from memory to data
    osize reserve_front;

    // always null terminated, so the real data array size is num+1
    obyte *data;
    osize num;

    // vfuncs
    OArray__dyn_realloc_fn v_dyn_realloc;
} OArray;


/**
 * Initializes the object
 * @param obj OArray object
 * @param parent to inherit from
 * @param opt_data to copy from, or NULL (allocated by zeros)
 * @param element_size (sizeof(...))
 * @param num elements to create (or copied from data, if not NULL)
 * @param start_capacity = o_max(start_capacity, num)
 * @param mode dynamic realloc mode, like OArray_REALLOC_DOUBLED_FRONT
 * @return obj casted as OArray
 */
O_EXTERN
OArray *OArray_init(oobj obj, oobj parent, const void *opt_data, osize element_size, osize num,
                    osize start_capacity, enum OArray_dyn_realloc_mode mode);

/**
 * Creates a new the OArray object
 * @param parent to inherit from
 * @param opt_data to copy from, or NULL (allocated by zeros)
 * @param element_size (sizeof(...))
 * @param num elements to create (or copied from data, if not NULL)
 * @return The new object
 */
O_INLINE
OArray *OArray_new(oobj parent, const void *opt_data, osize element_size, osize num)
{
    OObj_DECL_IMPL_NEW(OArray, parent, opt_data, element_size, num, 0, OArray_REALLOC_DEFAULT);
}

/**
 * Creates a new the OArray object
 * Sets OArray__v_dyn_realloc_doubled function to add capacity at the back on reallocs
 * @param parent to inherit from
 * @param opt_data to copy from, or NULL (allocated by zeros)
 * @param element_size (sizeof(...))
 * @param num elements to create (or copied from data, if not NULL)
 * @param start_capacity = o_max(start_capacity, num)
 * @return The new object
 */
O_INLINE
OArray *OArray_new_dyn(oobj parent, const void *opt_data, osize element_size, osize num,
                       osize start_capacity)
{
    OObj_DECL_IMPL_NEW(OArray, parent, opt_data, element_size, num, start_capacity, OArray_REALLOC_DOUBLED);
}

/**
 * Creates a new the OArray object
 * Sets OArray__v_dyn_realloc_doubled_front function to add capacity at the front on reallocs
 * @param parent to inherit from
 * @param opt_data to copy from, or NULL (allocated by zeros)
 * @param element_size (sizeof(...))
 * @param num elements to create (or copied from data, if not NULL)
 * @param start_capacity = o_max(start_capacity, num)
 * @return The new object
 */
O_INLINE
OArray *OArray_new_dyn_mode(oobj parent, const void *opt_data, osize element_size, osize num,
                             osize start_capacity, enum OArray_dyn_realloc_mode mode)
{
    OObj_DECL_IMPL_NEW(OArray, parent, opt_data, element_size, num, start_capacity, mode);
}

/**
 * Creates a new the OArray object with a cloned string
 * @param parent to inherit from
 * @param string to copy from
 * @return The new object
 */
O_INLINE
OArray *OArray_new_string(oobj parent, const char *string)
{
    if (!string) {
        string = "";
    }
    // uses strlen instead of o_strlen, because we are in a static header function
    return OArray_new(parent, string, sizeof(char), (osize) strlen(string));
}


//
// virtual implementations:
//

/**
 * virtual operator function
 * @param obj OArray object
 * @return the number of elements stored in the array
 */
O_EXTERN
osize OArray__v_op_num(oobj obj);

/**
 * virtual operator function
 * @param obj OArray object
 * @param idx element index
 * @return void * of the element data
 */
O_EXTERN
void *OArray__v_op_at(oobj obj, osize idx);


/**
 * Default implementation that just allocates min_needed, so no extra capacity
 * @param obj OArray object
 * @param min_needed is the minimal needed capacity to store the array
 * @param append_front, append_back elements reallocation must append minimum at front/back
 * @note calls OArray_realloc internally for the reallocation
 */
O_EXTERN
void OArray__v_dyn_realloc(oobj obj, osize min_needed, osize append_front, osize append_back);


/**
 * asserts that NO reallocation is done, so memory will always use the same pointer (or custom...)
 * @param obj OArray object
 */
O_EXTERN
void OArray__v_dyn_realloc_assert(oobj obj, osize min_needed, osize append_front, osize append_back);


/**
 * Adds extra capacity at the back, so less actual reallocations needed for appending back
 * @param obj OArray object
 * @param min_needed is the minimal needed capacity to store the array
 * @param append_front, append_back elements reallocation must append minimum at front/back
 * @note calls OArray_realloc internally for the reallocation
 */
O_EXTERN
void OArray__v_dyn_realloc_doubled(oobj obj, osize min_needed, osize append_front, osize append_back);


/**
 * Adds extra capacity at the front, so less actual reallocations needed for appending at front
 * @param obj OArray object
 * @param min_needed is the minimal needed capacity to store the array
 * @param append_front, append_back elements reallocation must append minimum at front/back
 * @note calls OArray_realloc internally for the reallocation
 */
O_EXTERN
void OArray__v_dyn_realloc_doubled_front(oobj obj, osize min_needed, osize append_front, osize append_back);


/**
 * Adds extra capacity at the front and back equally,
 * so less actual reallocations needed for appending at front or back
 * @param obj OArray object
 * @param min_needed is the minimal needed capacity to store the array
 * @param append_front, append_back elements reallocation must append minimum at front/back
 * @note calls OArray_realloc internally for the reallocation
 */
O_EXTERN
void OArray__v_dyn_realloc_doubled_center(oobj obj, osize min_needed, osize append_front, osize append_back);

//
// object functions:
//

/**
 * @param obj OArray object
 * @return the memory array, most of the time equal to data
 */
OObj_DECL_GET(OArray, obyte *, memory)


/**
 * @param obj OArray object
 * @return the data pointer as void *
 *         always null terminated (num+1 is allocated with zeros)
 * @note the pointer is valid until the array is resized
 */
O_INLINE
void *OArray_data_void(oobj obj)
{
    OObj_assert(obj, OArray);
    OArray *self = obj;
    return self->data;
}


/**
 * @param obj OArray object
 * @param type to cast into
 * @return the data pointer as type *
 *         always null terminated (num+1 is allocated with zeros)
 * @note asserts that the element_size matches
 *       the pointer is valid until the array is resized
 */
#define OArray_data(obj, type)                           \
(assert(((OArray *)obj)->element_size == sizeof(type)),  \
(type*) OArray_data_void(obj))


/**
 * @param obj OArray object
 * @return the number of elements stored in the array
 * @note or use the o_num operator
 */
OObj_DECL_GET(OArray, osize, num)


/**
 * @param obj OArray object
 * @return the size of an element
 */
OObj_DECL_GET(OArray, osize, element_size)

/**
 * @param obj OArray object
 * @return size * elements_size
 */
O_INLINE
osize
OArray_byte_size(oobj obj)
{
    OObj_assert(obj, OArray);
    OArray *self = obj;
    return self->num * self->element_size;
}

/**
 * Reset the size of an element
 * @param obj OArray object
 * @param new_element_size the element_size to set
 * @note asserts: byte_size % new_element_size == 0
 */
O_EXTERN
void OArray_element_size_reset(oobj obj, osize new_element_size);

/**
 * Returns a void * of the element at the given index
 * @param obj OArray object
 * @param idx element index
 * @return void * of the element data
 * @note asserts idx bounds.
 *       the pointer is valid until the array is resized.
 *       DONT FORGET TO DEREFERENCE oobj ELEMENTS (return would be oobj *)
 * @note or use the o_at operator
 */
O_INLINE
void *OArray_at_void(oobj obj, osize idx)
{
    OObj_assert(obj, OArray);
    OArray *self = obj;
    assert(idx >= 0 && idx < self->num && "idx out of OArray bounds");
    return &self->data[idx * self->element_size];
}

/**
 * Returns a type * of the element at the given index
 * @param obj OArray object
 * @param idx element index
 * @param type to cast into (not checked for type equality)
 * @return type * of the element data
 * @note asserts idx bounds.
 *       the pointer is valid until the array is resized.
 *       DONT FORGET TO DEREFERENCE oobj ELEMENTS (return would be oobj *)
 * @note or use the o_at operator (does not asserts element_size...)
 */
#define OArray_at(obj, idx, type)                        \
(assert(((OArray *)obj)->element_size == sizeof(type)),  \
(type*) OArray_at_void((obj), (idx)))



/**
 * @param obj OArray object
 * @return the capacity for the array memory, counted in elements
 */
OObj_DECL_GET(OArray, osize, capacity)

/**
 * @param obj OArray object
 * @return the offset from memory to data, counted in elements
 */
OObj_DECL_GET(OArray, osize, reserve_front)

/**
 * @param obj OArray object
 * @return remaining (capactity) elements at the back
 */
O_INLINE
osize OArray_reserve(oobj obj)
{
    OObj_assert(obj, OArray);
    OArray *self = obj;
    return self->capacity - self->num - self->reserve_front;
}

/**
 * May o_memmove the data to apply the new offset, also may shrinken the num, it offset to large
 * @param obj OArray object
 * @param reserve_front from allocated memory to data, counted in elements
 */
O_EXTERN
void OArray_reserve_shift(oobj obj, osize reserve_front);

/**
 * Resets the capacity
 * @param obj OArray object
 * @param capacity new capacity, if < num, num will be reset to the new capacity
 * @param reserve_front from allocated memory to data, counted in elements
 */
O_EXTERN
void OArray_realloc(oobj obj, osize capacity, osize reserve_front);

/**
 * Resets the capacity to the current number of elements.
 * @param obj OArray object
 */
O_INLINE
void OArray_capacity_fit(oobj obj)
{
    OArray_realloc(obj, OArray_num(obj), 0);
}

/**
 * Resets the virtual realloc function. See one of OArray__v_realloc_* fns or use your own.
 * @param obj OArray
 * @param realloc virtual realloc function for new allocations
 */
O_INLINE
void OArray_dyn_realloc_fn_set(oobj obj, OArray__dyn_realloc_fn realloc)
{
    OObj_assert(obj, OArray);
    OArray *self = obj;
    self->v_dyn_realloc = realloc;
}

/**
 * @param obj OArray
 * @param mode just calls OArray_dyn_realloc_fn_set with the assiciated realloc_fn
 */
O_EXTERN
void OArray_dyn_realloc_mode_set(oobj obj, enum OArray_dyn_realloc_mode mode);

/**
 * Resizes the array, makes use of empty/unused capacity at back.
 * Else uses the virtual realloc function.
 * @param obj OArray object
 * @param num new element size
 */
O_EXTERN
void OArray_resize(oobj obj, osize num);

/**
 * Resizes the array in the front, makes use of empty/unused capacity at front.
 * Else uses the virtual realloc function.
 * @param obj OArray object
 * @param num new element size
 */
O_EXTERN
void OArray_resize_front(oobj obj, osize num);


/**
 * Just calls OArray_resize(..., 0) to remove all elements.
 * @ param obj OArray object
 */
O_INLINE
void OArray_clear(oobj obj)
{
    OArray_resize(obj, 0);
}


/**
 * Moves the internal memory buffer as resource into the object into.
 * Sets internal buffer to NULL and num|size to 0
 * @param obj OArray object
 * @param into OObj to move the array resource into
 * @return the moved data buffer
 */
O_INLINE
void *OArray_move(oobj obj, oobj into)
{
    OObj_assert(obj, OArray);
    OArray *self = obj;
    void *mem = self->memory;
    self->memory = self->data = NULL;
    self->num = 0;
    o_mem_move(self, into, mem);
    return mem;
}


/**
 * Clones the data (not capacity and settings!) of another OArray
 * @param obj OArray to be cloned from, also acts as parent
 * @return a new OArray with data clones
 */
O_INLINE
OArray *OArray_clone(oobj obj)
{
    return OArray_new(obj, OArray_data_void(obj), OArray_element_size(obj), OArray_num(obj));
}

/**
 * Sorts the data array with qsort.
 * @param obj OArray object
 * @param comperator a comperator function, return ("a" - "b") to sort in ascending order
 *                   <0: a is less than b
 *                   0:  equal
 *                   >0: a is greater than b
 */
O_EXTERN
void OArray_sort(oobj obj, OArray__compare_fn comperator);

/**
 * Searches in the data array with bsearch.
 * @param obj OArray object
 * @param search_element the element to search the index for
 * @param comperator a comperator function, return ("a" - "b") to sort in ascending order
 *                   <0: a is less than b
 *                   0:  equal
 *                   >0: a is greater than b
 * @return the index in the array for the searched element, or -1 if not found
 * @note The array MUST be sorted (see OArray_sort)
 */
O_EXTERN
osize OArray_search_sorted(oobj obj, const void *search_element, OArray__compare_fn comperator);

/**
 * Searches in the data array with bsearch.
 * @param obj OArray object
 * @param search_element the element to search the index for
 * @param comperator a comperator function, return ("a" - "b") to sort in ascending order
 *                   <0: a is less than b
 *                   0:  equal
 *                   >0: a is greater than b
 * @return the index in the array for the searched element, or -1 if not found
 * @note not as fast as OArray_search_sorted, simply iterates through the array
 */
O_EXTERN
osize OArray_search(oobj obj, const void *search_element, OArray__compare_fn comperator);


/**
 * Append an array of elements at the end
 * @param obj OArray object
 * @param opt_elements_data elements to append, or NULL (new allocated elements will be cleared to 0)
 * @param num of elements to append
 * @return a pointer to the copied element in the data memory (or NULL id num <= 0)
 * @note the returned pointer is valid until the array is resized.
 *       DONT FORGET TO PASS A REFERENCE OF oobj ELEMENTS (would need oobj *)
 */
O_EXTERN
void *OArray_append(oobj obj, const void *opt_elements_data, osize num);

/**
 * Takes out an array of elements from the OArray at the end
 * @param obj OArray object
 * @param out_opt_elements_data elements to take, or NULL
 * @param num of elements to take
 * @note asserts that the OArray_num(obj) >= num.
 *       DONT FORGET TO PASS A REFERENCE OF oobj ELEMENTS (would need oobj *)
 */
O_EXTERN
void OArray_take(oobj obj, void *out_opt_elements_data, osize num);


/**
 * Append an array of elements at the front, shifting the rest back
 * @param obj OArray object
 * @param opt_elements_data elements to append, or NULL (new allocated elements will be cleared to 0)
 * @param num of elements to append
 * @return a pointer to the copied element in the data memory (or NULL id num <= 0)
 * @note the returned pointer is valid until the array is resized.
 *       DONT FORGET TO PASS A REFERENCE OF oobj ELEMENTS (would need oobj *)
 */
O_EXTERN
void *OArray_append_front(oobj obj, const void *opt_elements_data, osize num);

/**
 * Takes out an array of elements from the OArray at the beginning, shifting the rest front
 * @param obj OArray object
 * @param out_opt_elements_data elements to take, or NULL
 * @param num of elements to take
 * @note asserts that the OArray_num(obj) >= num.
 *       DONT FORGET TO PASS A REFERENCE OF oobj ELEMENTS (would need oobj *)
 */
O_EXTERN
void OArray_take_front(oobj obj, void *out_opt_elements_data, osize num);

/**
 * Append an array of elements at the given index, shifting the rest back
 * @param obj OArray object
 * @param idx
 * @param opt_elements_data elements to append, or NULL (new allocated elements will be cleared to 0)
 * @param num of elements to append
 * @return a pointer to the copied element in the data memory (or NULL id num <= 0)
 * @note asserts idx in bounds [0 : size]
 *       the returned pointer is valid until the array is resized.
 *       DONT FORGET TO PASS A REFERENCE OF oobj ELEMENTS (would need oobj *)
 */
O_EXTERN
void *OArray_append_at(oobj obj, osize idx, const void *opt_elements_data, osize num);


/**
 * Takes out an array of elements at the given index, shifting the rest front
 * @param obj OArray object
 * @param idx
 * @param out_opt_elements_data elements to take, or NULL
 * @param num of elements to take
 * @note asserts that the OArray_num(obj) >= num.
 *       DONT FORGET TO PASS A REFERENCE OF oobj ELEMENTS (would need oobj *)
 */
O_EXTERN
void OArray_take_at(oobj obj, osize idx, void *out_opt_elements_data, osize num);


/**
 * Appends a single element at the end
 * @param obj OArray object
 * @param element_data element to append, or NULL (new allocated element will be cleared to 0)
 * @return a pointer to the copied element in the data memory
 * @note element_size should match OArray_element_size(obj)
 *       the returned pointer is valid until the array is resized.
 *       DONT FORGET TO PASS A REFERENCE OF oobj ELEMENTS (would need oobj *)
 */
O_INLINE
void *OArray_push(oobj obj, const void *opt_element_data)
{
    return OArray_append(obj, opt_element_data, 1);
}

/**
 * Takes out a single element at the end
 * @param obj OArray object
 * @param out_opt_element_data element to take out, or NULL
 * @note element_size should match OArray_element_size(obj).
 *       DONT FORGET TO PASS A REFERENCE OF oobj ELEMENTS (would need oobj *)
 */
O_INLINE
void OArray_pop(oobj obj, void *out_opt_element_data)
{
    OArray_take(obj, out_opt_element_data, 1);
}

/**
 * Appends a single element at the given index, shifting the rest back
 * @param obj OArray object
 * @param idx the start index at which the element should be appended
 * @param opt_element_data element to append, or NULL (new allocated element will be cleared to 0)
 * @return a pointer to the copied element in the data memory
 * @note element_size should match OArray_element_size(obj)
 *       the returned pointer is valid until the array is resized.
 *       DONT FORGET TO PASS A REFERENCE OF oobj ELEMENTS (would need oobj *)
 */
O_INLINE
void *OArray_push_at(oobj obj, osize idx, const void *opt_element_data)
{
    return OArray_append_at(obj, idx, opt_element_data, 1);
}

/**
 * Takes out a single element at the given index, shifting the rest front
 * @param obj OArray object
 * @param idx the start index at which the element should be taken
 * @param out_opt_element_data element to take out, or NULL
 * @note element_size should match OArray_element_size(obj)
 * @note asserts idx in bounds [0 : size).
 *       DONT FORGET TO PASS A REFERENCE OF oobj ELEMENTS (would need oobj *)
 */
O_INLINE
void OArray_pop_at(oobj obj, osize idx, void *out_opt_element_data)
{
    OArray_take_at(obj, idx, out_opt_element_data, 1);
}

/**
 * Appends a single element at the beginning, shifting the rest back
 * @param obj OArray object
 * @param opt_element_data element to append, or NULL (new allocated element will be cleared to 0)
 * @return a pointer to the copied element in the data memory
 * @note element_size should match OArray_element_size(obj)
 *       the returned pointer is valid until the array is resized.
 *       DONT FORGET TO PASS A REFERENCE OF oobj ELEMENTS (would need oobj *)
 */
O_INLINE
void *OArray_push_front(oobj obj, const void *opt_element_data)
{
    return OArray_append_front(obj, opt_element_data, 1);
}

/**
 * Takes out a single element at the beginning, shifting the rest front
 * @param obj OArray object
 * @param out_opt_element_data element to take out, or NULL
 * @note element_size should match OArray_element_size(obj).
 *       DONT FORGET TO PASS A REFERENCE OF oobj ELEMENTS (would need oobj *)
 */
O_INLINE
void OArray_pop_front(oobj obj, void *out_opt_element_data)
{
    OArray_take_front(obj, out_opt_element_data, 1);
}


//
// string stuff
//

/**
 * Appends a null terminated string at the end
 * @param obj OArray object
 * @param string to append
 * @return num of chars appended
 * @note elements_size must have been set to char | 1
 *       inlined to make use of strlen for static strings (not used o_strlen to not need to import o/str.h)
 */
O_INLINE
osize OArray_append_string(oobj obj, const char *string)
{
    OObj_assert(obj, OArray);
    OArray *self = obj;
    assert(self->element_size == sizeof(char));
    assert(string);
    osize len = (osize) strlen(string);
    OArray_append(obj, string, len);
    return len;
}


/**
 * Appends a formatted string at the end.
 * va_list version
 * @param obj OArray object
 * @param format formatted string
 * @param args va_list version
 * @return num of chars appended
 * @note elements_size must have been set to char | 1
 */
O_EXTERN
osize OArray_append_stringf_va(oobj obj, const char *format, va_list args);


/**
 * Appends a formatted string at the end.
 * @param obj OArray object
 * @param format formatted string
 * @note elements_size must have been set to char | 1
 */
O_EXTERN
osize OArray_append_stringf(oobj obj, const char *format, ...);


#endif //O_OARRAY_H
