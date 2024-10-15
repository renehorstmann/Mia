#ifdef MIA_OPTION_THREAD
#ifndef O_OQUEUE_H
#define O_OQUEUE_H

/**
 * @file OQueue.h
 *
 * Object
 *
 * The Queue obj binds to the OArray functions with locking (threadsafe)
 */


#include "OArray.h"
#include "OCondition.h"

/** object id */
#define OQueue_ID OObj_ID "OQueue"


typedef struct {
    OObj super;

    OArray *data;
    OCondition *cond;

    // a function like take or pop will return as invalid (false), when no members in the queue exists
    bool closed;
} OQueue;


/**
 * Initializes the object
 * @param obj OQueue object
 * @param parent to inherit from
 * @param data The OArray to work on
 * @return obj casted as OQueue
 * @note Only use OQueue_ functions when working on the given OArray (in multithread context)
 */
O_EXTERN
OQueue *OQueue_init(oobj obj, oobj parent, oobj data);

/**
 * Creates a new the OQueue object
 * @param parent to inherit from
 * @param data The OArray to work on
 * @return The new object
 * @note Only use OQueue_ functions when working on the given OArray (in multithread context)
 */
O_INLINE
OQueue *OQueue_new(oobj parent, oobj data)
{
    OObj_DECL_IMPL_NEW(OQueue, parent, data);
}


//
// object functions:
//

/**
 * Signal a closed event to the Queue.
 * Waiting threads may return at take, pop, etc. with an invalid result (false) when the queue is empty
 * @param obj OQueue object
 */
O_EXTERN
void OQueue_signal_close(oobj obj);

/**
 * @param obj OQueue object
 * @return true if closed has been signaled
 */
O_EXTERN
bool OQueue_closed(oobj obj);

/**
 * @param obj OQueue object
 * @return the number of elements stored in the data
 * @threadsafe
 */
O_EXTERN
osize OQueue_num(oobj obj);


/**
 * @param obj OQueue object
 * @return the size of an element from the data
 * @threadsafe
 */
O_EXTERN
osize OQueue_element_size(oobj obj);

/**
 * @param obj OQueue object
 * @return size * elements_size stored in the data
 * @threadsafe
 */
O_EXTERN
osize OQueue_byte_size(oobj obj);

// will reset the size of an element
// asserts: byte_size % new_element_size == 0

/**
 * Resets the (byte) size of a single element in the data
 * @param obj OQueue object
 * @param new_element_size: the new element_size for the data, asserts byte_size % new_element_size == 0
 * @threadsafe
 */
O_EXTERN
void OQueue_element_size_reset(oobj obj, osize new_element_size);

/**
 * Tries to set elements in the data at the given index position.
 * Will not resize the data.
 * @param obj OQueue object
 * @param elements to set|copy from
 * @param num Number of elements to set
 * @param idx Start element position in the stored data
 * @return false if failed
 * @threadsafe
 */
O_EXTERN
bool OQueue_set(oobj obj, const void *elements, osize num, osize idx);


/**
 * Tries to get elements from the data at the given index position.
 * Will not resize the data.
 * @param obj OQueue object
 * @param out_elements to get|copy into
 * @param num Number of elements to set
 * @param idx Start element position in the stored data
 * @return false if failed
 * @threadsafe
 */
O_EXTERN
bool OQueue_get(oobj obj, void *out_elements, osize num, osize idx);


/**
 * Sets and may allocate a new data array with the given size
 * Calls the virtual resize function
 * @param obj OQueue object
 * @param num New number of elements for the stored data
 * @threadsafe
 */
O_EXTERN
void OQueue_resize(oobj obj, osize num);


/**
 * Appends an array of elements at the end.
 * @param obj OQueue object
 * @param opt_elements_data elements to append, or NULL (new allocated elements will be cleared to 0)
 * @param num Number of elements to append
 * @threadsafe
 */
O_EXTERN
void OQueue_append(oobj obj, const void *opt_elements_data, osize num);

/**
 * Takes an array of elements at the end.
 * Waits until the data is able to be taken.
 * @param obj OQueue object
 * @param out_opt_elements_data elements to take, or NULL
 * @param num Number of elements to append
 * @return false if closed is signaled (OQueue_signal_close) and the queue is empty
 * @threadsafe
 */
O_EXTERN
bool OQueue_take(oobj obj, void *out_opt_elements_data, osize num);

/**
 * Takes an array of elements at the end.
 * @param obj OQueue object
 * @param out_opt_elements_data elements to take, or NULL
 * @param num Number of elements to append
 * @return False if failed
 * @threadsafe
 */
O_EXTERN
bool OQueue_take_try(oobj obj, void *out_opt_elements_data, osize num);

/**
 * Append an array of elements at the given index, shifting the rest back
 * @param obj OQueue object
 * @param idx the index to append to
 * @param opt_elements_data elements to append, or NULL (new allocated elements will be cleared to 0)
 * @param num Number of elements to append
 * @threadsafe
 */
O_EXTERN
void OQueue_append_at(oobj obj, osize idx, const void *opt_elements_data, osize num);

/**
 * Takes out an array of elements at the given index, shifting the rest front
 * Waits until the data is able to be taken.
 * @param obj OArray object
 * @param idx the index to take from
 * @param out_opt_elements_data elements to take, or NULL
 * @param num of elements to take
 * @return false if closed is signaled (OQueue_signal_close) and the queue is empty
 */
O_EXTERN
bool OQueue_take_at(oobj obj, osize idx, void *out_opt_elements_data, osize num);

/**
 * Takes out an array of elements at the given index, shifting the rest front
 * @param obj OArray object
 * @param idx the index to take from
 * @param out_opt_elements_data elements to take, or NULL
 * @param num of elements to take
 * @return False if failed
 */
O_EXTERN
bool OQueue_take_at_try(oobj obj, osize idx, void *out_elements_data, osize num);

/**
 * Append an array of elements at the front, shifting the rest back
 * @param obj OQueue object
 * @param opt_elements_data elements to append, or NULL (new allocated elements will be cleared to 0)
 * @param num Number of elements to append
 * @threadsafe
 */
O_INLINE
void OQueue_append_front(oobj obj, const void *opt_elements_data, osize num)
{
    OQueue_append_at(obj, 0, opt_elements_data, num);
}

/**
 * Takes an array of elements at the beginning, shifting the rest front.
 * Waits until the data is able to be taken.
 * @param obj OQueue object
 * @param out_opt_elements_data elements to take, or NULL
 * @param num Number of elements to append
 * @return false if closed is signaled (OQueue_signal_close) and the queue is empty
 * @threadsafe
 */
O_INLINE
bool OQueue_take_front(oobj obj, void *out_opt_elements_data, osize num)
{
    return OQueue_take_at(obj, 0, out_opt_elements_data, num);
}

/**
 * Takes an array of elements at the beginning, shifting the rest front.
 * @param obj OQueue object
 * @param out_opt_elements_data elements to take, or NULL
 * @param num Number of elements to append
 * @return False if failed
 * @threadsafe
 */
O_INLINE
bool OQueue_take_front_try(oobj obj, void *out_elements_data, osize num)
{
    return OQueue_take_at_try(obj, 0, out_elements_data, num);
}


/**
 * Appends a single element at the end
 * @param obj OQueue object
 * @param element_data element to append, or NULL (new allocated element will be cleared to 0)
 * @note element_size should match OQueue_element_size(obj)
 * @threadsafe
 */
O_INLINE
void OQueue_push(oobj obj, const void *element_data)
{
    OQueue_append(obj, element_data, 1);
}

/**
 * Takes out a single element at the end
 * Waits until the data is able to be taken.
 * @param obj OArray object
 * @param out_opt_element_data element to take out, or NULL
 * @return false if closed is signaled (OQueue_signal_close) and the queue is empty
 * @note element_size should match OQueue_element_size(obj)
 * @threadsafe
 */
O_INLINE
bool OQueue_pop(oobj obj, void *out_opt_element_data)
{
    return OQueue_take(obj, out_opt_element_data, 1);
}

/**
 * Takes out a single element at the end
 * @param obj OArray object
 * @param out_opt_element_data element to take out, or NULL
 * @note element_size should match OQueue_element_size(obj)
 * @return False if failed
 * @threadsafe
 */
O_INLINE
bool OQueue_pop_try(oobj obj, void *out_opt_element_data)
{
    return OQueue_take_try(obj, out_opt_element_data, 1);
}


/**
 * Appends a single element at the given index, shifting the rest back
 * @param obj OQueue object
 * @param idx the index to push
 * @param element_data element to append, or NULL (new allocated element will be cleared to 0)
 * @note element_size should match OQueue_element_size(obj)
 * @threadsafe
 */
O_INLINE
void OQueue_push_at(oobj obj, osize idx, const void *element_data)
{
    OQueue_append_at(obj, idx, element_data, 1);
}

/**
 * Takes out a single element at the given index, shifting the rest front
 * Waits until the data is able to be taken.
 * @param obj OArray object
 * @param idx the index to pop
 * @param out_opt_element_data element to take out, or NULL
 * @return false if closed is signaled (OQueue_signal_close) and the queue is empty
 * @note element_size should match OQueue_element_size(obj)
 * @threadsafe
 */
O_INLINE
bool OQueue_pop_at(oobj obj, osize idx, void *out_opt_element_data)
{
    return OQueue_take_at(obj, idx, out_opt_element_data, 1);
}

/**
 * Takes out a single element at the given index, shifting the rest front
 * @param obj OArray object
 * @param idx the index to pop
 * @param out_opt_element_data element to take out, or NULL
 * @note element_size should match OQueue_element_size(obj)
 * @return False if failed
 * @threadsafe
 */
O_INLINE
bool OQueue_pop_at_try(oobj obj, osize idx, void *out_opt_element_data)
{
    return OQueue_take_at_try(obj, idx, out_opt_element_data, 1);
}

/**
 * Appends a single element at the beginning, shifting the rest back
 * @param obj OArray object
 * @param opt_element_data element to append, or NULL (new allocated element will be cleared to 0)
 * @note element_size should match OQueue_element_size(obj)
 */
O_INLINE
void OQueue_push_front(oobj obj, const void *element_data)
{
    OQueue_append_front(obj, element_data, 1);
}

/**
 * Takes out a single element at the beginning, shifting the rest front
 * Waits until the data is able to be taken.
 * @param obj OArray object
 * @param out_opt_element_data element to take out, or NULL
 * @return false if closed is signaled (OQueue_signal_close) and the queue is empty
 * @note element_size should match OQueue_element_size(obj)
 */
O_INLINE
bool OQueue_pop_front(oobj obj, void *out_opt_element_data)
{
    return OQueue_take_front(obj, out_opt_element_data, 1);
}

/**
 * Takes out a single element at the beginning, shifting the rest front
 * @param obj OArray object
 * @param out_opt_element_data element to take out, or NULL
 * @note element_size should match OQueue_element_size(obj)
 * @return False if failed
 */
O_INLINE
bool OQueue_pop_front_try(oobj obj, void *out_opt_element_data)
{
    return OQueue_take_front_try(obj, out_opt_element_data, 1);
}

#endif //O_OQUEUE_H
#endif //MIA_OPTION_THREAD
