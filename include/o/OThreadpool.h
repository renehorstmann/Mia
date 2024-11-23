#ifdef MIA_OPTION_THREAD
#ifndef O_OTHREADPOOL_H
#define O_OTHREADPOOL_H

/**
 * @file OThreadpool.h
 *
 * Object
 *
 * The ThreadPool obj creates multiple threads to fast'n up OFuture calls
 * @sa OFuture.h
 */

#include "OObj.h"

/** object id */
#define OThreadpool_ID OObj_ID "OThreadpool"

typedef struct {
    OObj super;

    // size is fix (OThread)
    oobj *threads;
    osize threads_size;

    // actively running
    oobj *running_futures;
    osize running_futures_size;

    // an OQueue of Futures to start
    oobj task_queue;
} OThreadpool;


/**
 * Initializes the object
 * @param obj OThreadpool object
 * @param parent to inherit from
 * @param threads number of threads for this pool
 * @return obj casted as OThreadpool
 */
O_EXTERN
OThreadpool *OThreadpool_init(oobj obj, oobj parent, osize threads);

/**
 * Creates a new the OThreadpool object
 * @param parent to inherit from
 * @param threads number of threads for this pool
 * @return The new object
 */
O_INLINE
OThreadpool *OThreadpool_new(oobj parent, osize threads)
{
    OObj_DECL_IMPL_NEW(OThreadpool, parent, threads);
}


//
// virtual implementations:
//

/**
 * Default deletor that waits for the threads
 * @param obj OThreadpool object
 */
O_EXTERN
void OThreadpool__v_del(oobj obj);


//
// object functions:
//

/**
 * @param obj OThreadpool object
 * @return the number of threads in this pool
 */
O_INLINE
osize OThreadpool_threads(oobj obj)
{
    OObj_assert(obj, OThreadpool);
    OThreadpool *self = obj;
    return self->threads_size;
}

/**
 * @param obj OThreadpool object
 * @return the amount of actively running futures
 */
O_EXTERN
osize OThreadpool_running_futures(oobj obj);

/**
 * @param obj OThreadpool object
 * @return the amount of waiting threads, that are directly ready to take a future
 */
O_INLINE
osize OThreadpool_available_threads(oobj obj)
{
    return OThreadpool_threads(obj) - OThreadpool_running_futures(obj);
}

/**
 * @param obj OThreadpool object
 * @return the amount of waiting futures
 */
O_EXTERN
osize OThreadpool_queuing(oobj obj);


#endif //O_OTHREADPOOL_H
#endif //MIA_OPTION_THREAD
