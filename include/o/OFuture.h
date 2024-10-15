#ifdef MIA_OPTION_THREAD
#ifndef O_OFUTURE_H
#define O_OFUTURE_H

/**
 * @file OFuture.h
 *
 * Object
 *
 * A Future obj simplifies background work with a thread.
 *
 * @note want to pass (or recv) data to the future? use o_user before running or use OFuture_new_run
 *       Be aware of race conditions. So set before running and recv after finished to avoid them easily.
 */


#include "OObj.h"

/** futures simple state machine */
enum OFuture_state {
    OFuture_INIT,
    OFuture_PREPARING,
    OFuture_RUNNING,
    OFuture_FINISHED,
    OFuture_NUM_STATES
};

/** object id */
#define OFuture_ID OObj_ID "OFuture"


typedef struct {
    OObj super;

    enum OFuture_state state;

    OObj__event_fn fn;

    oobj opt_threadpool;
    oobj thread;
    oobj finished_cond;
} OFuture;


/**
 * Initializes the object
 * @param obj OFuture object
 * @param parent to inherit from
 * @param fn the future execution function
 * @param opt_threadpool if not NULL, the function will get executed in the OThreadpool
 *                       if NULL, a new thread is created
 * @return obj casted as OFuture
 * @note want to pass (or recv) data to the future? use o_user before running or use OFuture_new_run
 *       Be aware of race conditions. So set before running and recv after finished to avoid them easily.
 */
O_EXTERN
OFuture *OFuture_init(oobj obj, oobj parent, OObj__event_fn fn, oobj opt_threadpool);

/**
 * Creates a new the OFuture object
 * @param parent to inherit from
 * @param fn the future execution function
 * @param opt_threadpool if not NULL, the function will get executed in the OThreadpool
 *                       if NULL, a new thread is created
 * @return The new object
 * @note want to pass (or recv) data to the future? use o_user before running or use OFuture_new_run
 *       Be aware of race conditions. So set before running and recv after finished to avoid them easily.
 */
O_INLINE
OFuture *OFuture_new(oobj parent, OObj__event_fn fn, oobj opt_threadpool)
{
    OObj_DECL_IMPL_NEW(OFuture, parent, fn, opt_threadpool);
}

/**
 * Creates a new the OFuture object. Automatically runs with OFuture_run
 * @param parent to inherit from
 * @param fn the future execution function
 * @param opt_threadpool if not NULL, the function will get executed in the OThreadpool
 *                       if NULL, a new thread is created
 * @param user user data passed into the OFuture with o_user_set
 * @return The new object
 * @note want to pass (or recv) data to the future? use o_user before running or use OFuture_new_run
 *       Be aware of race conditions. So set before running and recv after finished to avoid them easily.
 */
O_EXTERN
OFuture *OFuture_new_run(oobj parent, OObj__event_fn fn, oobj opt_threadpool, void *user);


//
// object functions:
//

/**
 * @param obj OFuture object
 * @return the current future state
 * @threadsafe
 */
O_EXTERN
enum OFuture_state OFuture_state(oobj obj);

/**
 * @param obj OFuture object
 * @return true if the future has finished
 * @threadsafe
 */
O_INLINE
bool OFuture_finished(oobj obj)
{
    enum OFuture_state state = OFuture_state(obj);
    assert(state != OFuture_INIT);
    return state == OFuture_FINISHED;
}


/**
 * Starts the future
 * @param obj OFuture object
 * @note once started, a call results in a noop
 */
O_EXTERN
void OFuture_run(oobj obj);

/**
 * Waits until the future has finished
 * @param obj OFuture object
 * @threadsafe
 */
O_EXTERN
void OFuture_wait(oobj obj);

/**
 * Waits until the future has finished, timed out
 * @param obj OFuture object
 * @param timeout_ms Millis until the wait will get aborted
 * @return false if timed out
 * @note: just calls o_thread_sleep(1) for timeout_ms times
 * @threadsafe
 */
O_EXTERN
bool OFuture_wait_timeout(oobj obj, int timeout_ms);


#endif //O_OFUTURE_H
#endif //MIA_OPTION_THREAD
