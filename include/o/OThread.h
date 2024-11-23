#ifdef MIA_OPTION_THREAD
#ifndef O_OTHREAD_H
#define O_OTHREAD_H

/**
 * @file OThread.h
 *
 * Object
 *
 * A simple thread for multithreading.
 *
 * @note: Only the thread creator should call its functions (NOT threadsafe)
 *
 * Calls OThread_wait in its deletor.
 * Can be detached but be warned! Synchronize its data with the tree, which may have been deleted while detached!
 *
 * @note want to pass (or recv) data to the thread? use o_user before running or use OThread_new_run
 *
 * current o_log level and quiet settings are copied into the new thread
 */

#include "OObj.h"

/** object id */
#define OThread_ID OObj_ID "OThread"


enum OThread_state
{
    OThread_INIT,
    OThread_RUNNING,
    OThread_FINISHED,
    OThread_DETACHED,
    OThread_NUM_STATES
};

// forward declaration
struct SDL_Thread;

typedef struct {
    OObj super;

    enum OThread_state state;

    OObj__event_fn fn;

    struct SDL_Thread *thread;
    ou64 id;
    char *name;

    int parent_init_log_level;
    bool parent_init_log_quiet;
} OThread;


/**
 * Initializes the object
 * @param obj OThread object
 * @param parent to inherit from
 * @param fn the thread execution function
 * @param name the name for the thread
 * @return obj casted as OThread
 * @note want to pass (or recv) data to the thread? use o_user before running or use OThread_new_run.
 *       This OThread is passed into the thread fn. Be careful with race conditions.
 */
O_EXTERN
OThread *OThread_init(oobj obj, oobj parent, OObj__event_fn fn, const char *name);

/**
 * Creates a new the OThread object
 * @param parent to inherit from
 * @param fn the thread execution function
 * @param name the name for the thread
 * @param auto_run if true, OThread_run is called automatically
 * @return The new object
 * @note want to pass (or recv) data to the thread? use o_user before running or use OThread_new_run.
 *       This OThread is passed into the thread fn. Be careful with race conditions.
 */
O_INLINE
OThread *OThread_new(oobj parent, OObj__event_fn fn, const char *name)
{
    OObj_DECL_IMPL_NEW(OThread, parent, fn, name);
}
/**
 * Creates a new the OThread object. Automatically calls OThread_run
 * @param parent to inherit from
 * @param fn the thread execution function
 * @param name the name for the thread
 * @param user userdata set into the OThread with o_user_set
 * @return The new object
 * @note This OThread is passed into the thread fn. Be careful with race conditions.
 */
O_EXTERN
OThread *OThread_new_run(oobj parent, OObj__event_fn fn, const char *name, void *user);


//
// virtual implementations:
//

/**
 * Default deletor that waits for the thread (if not done by OThread_wait, or OThread_detach)
 * @param obj OThread object
 */
O_EXTERN
void OThread__v_del(oobj obj);


//
// object functions:
//


/**
 * @param obj OThread object
 * @return Current state of the thread
 */
OObj_DECL_GET(OThread, enum OThread_state, state)

/**
 * @param obj OThread object
 * @return the id of this thread
 */
OObj_DECL_GET(OThread, ou64, id)


/**
 * @param obj OThread object
 * @return the name of this thread
 */
OObj_DECL_GET(OThread, const char *, name)


/**
 * Starts the thread
 * @param obj OThread object
 * @note once started, a call results in a noop
 */
O_EXTERN
void OThread_run(oobj obj);


/**
 * Waits for the thread function to finish
 * @param obj OThread object
 */
O_EXTERN
void OThread_wait(oobj obj);


/**
 * Detaches the thread.
 * @warning A detached thread should NOT work on a detached tree!
 *          Synchronize it with the tree using a shared reference counter or smth.
 *          The thread function also receives a reference of this OThread object, which has to be handled with caution if detached (and deleted)
 *          May use a new OObjRoot for this purpose.
 * @param obj OThread object
 * @return the returned status code
 * @note deletor and wait will get noops
 */
O_EXTERN
void OThread_detach(oobj obj);

#endif //O_OTHREAD_H
#endif //MIA_OPTION_THREAD
