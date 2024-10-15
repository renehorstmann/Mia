#ifndef MIA_ODELCALLBACK_H
#define MIA_ODELCALLBACK_H

/**
 * @file ODelcallback.h
 *
 * Object
 *
 * Simply calls a function in the objects deletor
 */

#include "OObj.h"


/** object id */
#define ODelcallback_ID OObj_ID "ODelcallback"


typedef struct {
    OObj super;

    OObj__event_fn fn;
} ODelcallback;


/**
 * Initializes the object.
 * @param obj ODelcallback object
 * @param parent to inherit from
 * @param fn the deletor callback function
 * @return obj casted as ODelcallback
 * @sa o_user_ref for passing userdata with the object
 */
O_EXTERN
ODelcallback *ODelcallback_init(oobj obj, oobj parent, OObj__event_fn fn);

/**
 * Creates a new the ODelcallback object
 * @param parent to inherit from
 * @param fn the deletor callback function
 * @return The new object
 * @sa o_user_ref for passing userdata with the object
 */
O_INLINE
ODelcallback *ODelcallback_new(oobj parent, OObj__event_fn fn)
{
    OObj_DECL_IMPL_NEW(ODelcallback, parent, fn);
}


/**
 * Creates a log_trace when deleted
 * @param parent to inherit from
 * @param name to identify, set with OObj_name_set, NULL safe
 * @param msg to identify, set with o_user_ref, NULL safe
 * @return The new object
 */
O_EXTERN
ODelcallback *ODelcallback_new_log_trace(oobj parent, const char *name, const char *msg);


/**
 * Creates an assert(0) when deleted (for objects that should not be deleted...)
 * @param parent to inherit from
 * @param name to identify, set with OObj_name_set, NULL safe
 * @param msg to identify, set with o_user_ref, NULL safe
 * @return The new object
 */
O_EXTERN
ODelcallback *ODelcallback_new_assert(oobj parent, const char *name, const char *msg);


//
// virtual implementations:
//

/**
 * Object deletor that calls the callback function, if not NULL
 * @param obj ODelcallback object
 */
O_EXTERN
void ODelcallback__v_del(oobj obj);


//
// object functions:
//

/**
 * Resets the callback function.
 * @param obj ODelcallback object
 * @param fn new callback function, NULL to turn off
 * @sa o_user_ref for passing userdata with the object
 */
O_INLINE
void ODelcallback_set(oobj obj, OObj__event_fn fn)
{
    OObj_assert(obj, ODelcallback);
    ODelcallback *self = obj;
    self->fn = fn;
}

#endif //MIA_ODELCALLBACK_H
