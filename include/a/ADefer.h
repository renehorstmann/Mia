#ifndef A_ADEFER_H
#define A_ADEFER_H

/**
 * @file ADefer.h
 *
 * Object
 *
 * Runs a callback on the beginning of the next frame, before scenes are active.
 * After the callback was run. The defer object is deleted automatically!
 *
 * Does NOT need a parent in the _init, _new call, ALWAYS uses a_app_root() as parent.
 * Don't use the _init call for creating the instance, use _new to allocate it automatically on a_app_root().
 */


#include "o/OObj.h"

/** object id */
#define ADefer_ID OObj_ID "ADefer"


typedef struct {
    OObj super;

    OObj__event_fn fn;
} ADefer;


/**
 * Initializes the object.
 * @param obj ADefer object
 * @param parent to inherit from
 * @param fn Deferred function. Called once at the beginning of the next frame
 * @return obj casted as ADefer
 * @note fn can work safely on the parent, because the parent will delete the defer if deleted
 *       asserts main thread
 * @sa o_user_ref for passing userdata with the object
 */
O_EXTERN
ADefer *ADefer_init(oobj obj, oobj parent, OObj__event_fn fn);

/**
 * Creates a new ADefer object
 * @param parent to inherit from
 * @param fn Deferred function. Called once at the beginning of the next frame
 * @return The new object
 * @note fn can work safely on the parent, because the parent will delete the defer if deleted
 *       asserts main thread
 * @sa o_user_ref for passing userdata with the object
 */
O_INLINE
ADefer *ADefer_new(oobj parent, OObj__event_fn fn)
{
    OObj_DECL_IMPL_NEW(ADefer, parent, fn);
}

/**
 * Creates a new ADefer object, to o_del an object
 * @param parent to inherit from
 * @param obj_to_del_deferred OObj object to delete deferred
 * @return The new object
 * @note uses o_user to save the obj for deletion
 */
O_EXTERN
ADefer *ADefer_new_del(oobj parent, oobj obj_to_del_deferred);

//
// virtual implementations:
//

/**
 * Deletor that unregisters this defer from a_app
 * @param obj ADefer object
 */
O_EXTERN
void ADefer__v_del(oobj obj);


//
// object functions:
//

/**
 * Resets the defered function.
 * @param obj ADefer object
 * @param fn new callback function, NULL to turn off
 * @sa o_user_ref for passing userdata with the object
 */
O_INLINE
void ADefer_set(oobj obj, OObj__event_fn fn)
{
    OObj_assert(obj, ADefer);
    ADefer *self = obj;
    self->fn = fn;
}


#endif //A_ADEFER_H
