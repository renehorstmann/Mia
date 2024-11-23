#ifdef MIA_OPTION_THREAD
#ifndef O_OCONDITION_H
#define O_OCONDITION_H

/**
 * @file OCondition.h
 *
 * Object
 *
 * The condition obj holds a variable to manage and notify multiple threads within a mutex
 * Just an obj wrapper for sdl cond
 */


#include "o/OObj.h"

#define OCondition_ID OObj_ID "OCondition"

typedef struct {
    OObj super;

    void *sdl_cond;
} OCondition;


/**
 * Initializes the object
 * @param obj OCondition object
 * @param parent to inherit from
 * @return obj casted as OCondition
 */
O_EXTERN
OCondition *OCondition_init(oobj obj, oobj parent);

/**
 * Creates a new OCondition object
 * @param parent to inherit from
 * @return The new object
 */
O_INLINE
OCondition *OOCondition_new(oobj parent)
{
    OObj_DECL_IMPL_NEW(OCondition, parent);
}

//
// v implementations
//

/**
 * Virtual function
 * Default deletor that calls SDL_DestroyCond
 * @param obj OCondition object
 */
O_EXTERN
void OCondition__v_del(oobj obj);


//
// object functions:
//

/**
 * Restarts all threads that are waiting on the condition
 * @param obj OCondition object
 * @threadsafe
 */
O_EXTERN
void OCondition_broadcast(oobj obj);

/**
 * Restart one of the threads that are waiting on the condition.
 * @param obj OCondition object
 * @threadsafe
 */
O_EXTERN
void OCondition_signal(oobj obj);

/**
 * Wait until a condition variable is signaled.
 * @param obj OCondition object
 * @param mutex The object, which is used as mutex (o_lock, etc.)
 * @threadsafe
 */
O_EXTERN
void OCondition_wait(oobj obj, oobj mutex);

/**
 * Wait until a condition variable is signaled, timed out
 * @param obj OCondition object
 * @param mutex The object, which is used as mutex (o_lock, etc.)
 * @param timeout_ms Millis until the wait will get aborted
 * @return false if timed out
 * @threadsafe
 */
O_EXTERN
bool OCondition_wait_timeout(oobj obj, oobj mutex, int timeout_ms);

#endif //O_OCONDITION_H
#endif //MIA_OPTION_THREAD
