#ifdef MIA_OPTION_THREAD

#include "o/OFuture.h"
#include "o/OObj_builder.h"
#include "o/OCondition.h"
#include "o/OThreadpool.h"
#include "o/OQueue.h"
#include "o/OThread.h"


//
// protected
// used by OThreadpool.c
//

O_EXTERN
void OFuture__thread_runnable(oobj future_holder)
{
    // future_holder may or may not be an OThread!
    // but it should have been packed with the OFuture self in o_user...

    OFuture *self = o_user(future_holder);
    OObj_assert(self, OFuture);

    o_lock_block(self) {
        self->state = OFuture_RUNNING;
    }

    self->fn(self);

    o_lock_block(self) {
        self->state = OFuture_FINISHED;
        OCondition_signal(self->finished_cond);
    }
}


OFuture *OFuture_init(oobj obj, oobj parent, OObj__event_fn fn, oobj opt_threadpool)
{
    OFuture *self = obj;
    o_clear(self, sizeof *self, 1);

    OObj_init(self, parent);
    OObj_id_set(self, OFuture_ID);

    self->state = OFuture_INIT;
    self->fn = fn;
    self->finished_cond = OOCondition_new(self);
    self->opt_threadpool = opt_threadpool;

    return self;
}

OFuture *OFuture_new_run(oobj parent, OObj__event_fn fn, oobj opt_threadpool, void *user)
{
    OFuture *self = OFuture_new(parent, fn, opt_threadpool);
    o_user_set(self, user);
    OFuture_run(self);
    return self;
}

enum OFuture_state OFuture_state(oobj obj)
{
    OObj_assert(obj, OFuture);
    OFuture *self = obj;
    enum OFuture_state state;
    o_lock_block(self) {
        state = self->state;
    }
    return state;
}

void OFuture_run(oobj obj)
{
    OObj_assert(obj, OFuture);
    OFuture *self = obj;
    o_lock_block(self) {
        if(self->state == OFuture_INIT) {
            self->state = OFuture_PREPARING;
            if(self->opt_threadpool) {
                OObj_assert(self->opt_threadpool, OThreadpool);
                OThreadpool *pool = self->opt_threadpool;
                OQueue_push(pool->task_queue, &self);
            } else {
                self->thread = OThread_new_run(self, OFuture__thread_runnable, "OFuture", self);
            }
        }
    }
}

void OFuture_wait(oobj obj)
{
    OObj_assert(obj, OFuture);
    OFuture *self = obj;
    o_lock_block(self) {
        assert(self->state != OFuture_INIT);
        while(!OFuture_finished(self)) {
            OCondition_wait(self->finished_cond, self);
        }
    }
}

bool OFuture_wait_timeout(oobj obj, int timeout_ms)
{
    OObj_assert(obj, OFuture);
    OFuture *self = obj;
    bool finished;
    for (int i = 0; i < timeout_ms; i++) {
        finished = OFuture_finished(self);
        if (finished) {
            break;
        }
        o_sleep(1);
    }

    // may be a race condition...
    o_lock_block(self) {
        finished = OFuture_finished(self);
    }
    return finished;
}


#endif // MIA_OPTION_THREAD
typedef int avoid_empty_translation_unit;
