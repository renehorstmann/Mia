#include "a/ADefer.h"
#include "o/OObj_builder.h"
#include "o/str.h"


O_STATIC
void deferred_delete(oobj obj)
{
    oobj to_delete = o_user(obj);
    o_del(to_delete);
}

ADefer *ADefer_init(oobj obj, oobj parent, OObj__event_fn fn) {
    o_thread_assert_main();

    ADefer *self = obj;
    o_clear(self, sizeof *self, 1);

    OObj_init(self, parent);
    OObj_id_set(self, ADefer_ID);

    // register in a_app protected function
    O_EXTERN
    void a_app__defer_register(ADefer *defer);
    a_app__defer_register(self);

    assert(fn);
    self->fn = fn;

    // deletor
    self->super.v_del = ADefer__v_del;

    return self;
}

ADefer *ADefer_new_del(oobj parent, oobj obj_to_del_deferred)
{
    ADefer *self = ADefer_new(parent, deferred_delete);
    o_user_set(self, obj_to_del_deferred);
    return self;
}

void ADefer__v_del(oobj obj) {
    OObj_assert(obj, ADefer);
    ADefer *self = obj;

    // unregister from a_app protected function
    O_EXTERN
    void a_app__defer_unregister(ADefer *defer);
    a_app__defer_unregister(self);

    OObj__v_del(self);
}

