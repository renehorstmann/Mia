#ifdef MIA_OPTION_THREAD

#include "o/OThreadpool.h"
#include "o/OObj_builder.h"
#include "o/OFuture.h"
#include "o/OQueue.h"
#include "o/OArray.h"
#include "o/OThread.h"
#include "o/str.h"

#define O_LOG_LIB "o"
#include "o/log.h"


O_STATIC
void OThreadpool__thread_runnable(oobj thread_obj)
{
    OThreadpool *self = o_user(thread_obj);
    OObj_assert(self, OThreadpool);

    o_log_debug_s(__func__,
               "pool thread started: " ou64_PRI, o_thread_id());

    for(;;) {
        OFuture *future;
        bool valid = OQueue_pop_front(self->task_queue, &future);

        // returns invalid if queue is closed
        if(!valid) {
            o_log_debug_s(__func__,
                       "pool thread finished: " ou64_PRI, o_thread_id());
        }

        OObj_assert(future, OFuture);


        o_lock_block(self) {
            self->running_futures[self->running_futures_size] = future;
            self->running_futures_size++;
            assert(self->running_futures_size<=self->threads_size);
        }

        //
        // run future
        //

        // protected in OFuture.c
        O_EXTERN
        void OFuture__thread_runnable(oobj obj);

        // the fn OFuture__thread_runnable expects obj's o_user to be set to the future.
        // So we create a placeholder to hold it
        oobj user = OObj_new(thread_obj);
        o_user_set(user, future);

        OFuture__thread_runnable(user);
        o_del(user);


        o_lock_block(self) {
            self->running_futures_size--;
            assert(self->running_futures_size>=0);
        }

    }
}

OThreadpool *OThreadpool_init(oobj obj, oobj parent, osize threads)
{
    assert(threads >= 1);

    OThreadpool *self = obj;
    o_clear(self, sizeof *self, 1);

    OObj_init(self, parent);
    OObj_id_set(self, OThreadpool_ID);

    self->threads_size = threads;
    self->threads = o_new0(self, oobj, threads);

    self->running_futures_size = 0;
    self->running_futures = o_new0(self, oobj, threads);

    self->task_queue = OQueue_new(self, OArray_new_dyn(self, NULL, sizeof(oobj), 0, threads*4));

    for(osize i=0; i<threads; i++) {
        char name[64];
        o_strf_buf(name, "OThreadpool_thread#%i/%i", (int) i, (int) threads);
        self->threads[i] = OThread_new_run(self, OThreadpool__thread_runnable, name, self);
    }

    // set virtual deletor
    self->super.v_del = OThreadpool__v_del;

    return self;
}

void OThreadpool__v_del(oobj obj)
{
    OObj_assert(obj, OThreadpool);
    OThreadpool *self = obj;

    // so that the threads stop...
    OQueue_signal_close(self->task_queue);

    // we need to call OThread_wait, so the threads are stopping work before the Queue gets deleted
    for(osize i=0; i<self->threads_size; i++) {
        OThread_wait(self->threads[i]);
    }
    OObj__v_del(obj);
}

osize OThreadpool_running_futures(oobj obj)
{
    OObj_assert(obj, OThreadpool);
    OThreadpool *self = obj;
    osize running;
    o_lock_block(self) {
        running = self->running_futures_size;
    }
    return running;
}


osize OThreadpool_queuing(oobj obj)
{
    OObj_assert(obj, OThreadpool);
    OThreadpool *self = obj;
    return OQueue_num(self->task_queue);
}


#endif // MIA_OPTION_THREAD
typedef int avoid_empty_translation_unit;
