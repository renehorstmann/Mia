#include "o/OWeakjoin.h"
#include "o/OObj_builder.h"

O_EXTERN
void OWeakjoin__join_deleted(oobj obj, oobj join)
{
    OObj_assert(obj, OWeakjoin);
    OWeakjoin *self = obj;
    o_lock_block(self) {
        assert(self->join == join);
        self->join = NULL;
    }
}

OWeakjoin *OWeakjoin_init(oobj obj, oobj parent, oobj join)
{
    OObj_assert(join, OJoin);

    OWeakjoin *self = obj;
    o_clear(self, sizeof *self, 1);

    OObj_init(self, parent);
    OObj_id_set(self, OWeakjoin_ID);

    self->join = join;

    // protected
    O_EXTERN
    void OJoin__add_weak(oobj obj, oobj weak);
    OJoin__add_weak(join, self);

    return self;
}

//
// object functions:
//

struct oobj_opt OWeakjoin_acquire(oobj obj)
{
    OObj_assert(obj, OWeakjoin);
    OWeakjoin *self = obj;

    OJoin *join = NULL;
    o_lock_block(self) {
        join = self->join;
        if(join) {
            OJoin_add(join, self);
        }
    }
    return oobj_opt(join);
}

bool OWeakjoin_release(oobj obj)
{
    OObj_assert(obj, OWeakjoin);
    OWeakjoin *self = obj;

    bool ok = false;
    o_lock_block(self) {
        ok = self->join && OJoin_is_parent(self->join, self);
        if(ok) {
            OJoin_remove(self->join, self);
        }
    }
    return ok;
}
