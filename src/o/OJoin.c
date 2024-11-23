#include "o/OJoin.h"
#include "o/OObj_builder.h"
#include "o/OArray.h"
#include "o/ODelcallback.h"

#define O_LOG_LIB "o"
#include "o/log.h"


struct parent {
    OJoin *self;
    oobj parent;
    oobj delcb;
};


O_STATIC
void OJoin__remove_weak(oobj obj, oobj weak)
{
    OObj_assert(obj, OJoin);
    OJoin *self = obj;

    o_lock_block(self) {
        // check if it's the list
        osize idx = -1;
        struct parent *p = NULL;
        for (osize i = 0; i < OArray_num(self->weaks); i++) {
            p = *OArray_at(self->weaks, i, struct parent *);
            if (p->parent == weak) {
                idx = i;
                break;
            }
        }
        if (idx < 0) {
            o_log_error_s(__func__, "parent is not in the weaks list?");
            continue;
        }

        // protected
        O_EXTERN
        void OWeakjoin__join_deleted(oobj obj, oobj join);
        OWeakjoin__join_deleted(weak, self);

        // only delete if not called by autodelete ODelcallback
        if(p->delcb) {
            o_user_set(p->delcb, NULL);
            o_del(p->delcb);
        }

        o_free(self, p);
        OArray_pop_at(self->weaks, idx, NULL);
    }

}

O_STATIC
void autoremove_parent(oobj obj)
{
    struct parent *p = o_user(obj);
    if(p) {
        // will delete itself already after this function
        p->delcb = NULL;
        OJoin_remove(p->self, p->parent);
    }
}

O_STATIC
void autoremove_weak(oobj obj)
{
    struct parent *p = o_user(obj);
    if(p) {
        // will delete itself already after this function
        p->delcb = NULL;
        OJoin__remove_weak(p->self, p->parent);
    }
}


// protected
O_EXTERN
void OJoin__add_weak(oobj obj, oobj weak)
{
    OObj_assert(obj, OJoin);
    OJoin *self = obj;

    o_lock_block(self) {
        struct parent *p = o_new0(self, struct parent, 1);
        p->self = self;
        p->parent = weak;

        OArray_push(self->weaks, &p);

        // create a del callback that is called, when parent gets deleted
        p->delcb = ODelcallback_new(weak, autoremove_weak);
        o_user_set(p->delcb, p);
    }
}


O_STATIC
void join_init_base(OJoin *self, struct o_allocator_i allocator) {
    o_clear(self, sizeof *self, 1);

    // protected
    O_EXTERN
    void OObj__init_base(oobj obj, struct o_allocator_i allocator);

    OObj__init_base(self, allocator);
    OObj_id_set(self, OJoin_ID);

    // set deletor
    self->super.v_del = OJoin__v_del;
}

O_STATIC
void init_parents_weaks(OJoin *self, oobj *parents, osize parents_size)
{
    self->parents = OArray_new_dyn(self, NULL, sizeof(struct parent *), 0, 8);
    if(parents_size<=0) {
        parents_size = o_list_num(parents);
    }
    assert(parents_size > 0 && parents && parents[0]);
    for (osize i = 0; i < parents_size; i++) {
        OJoin_add(self, parents[i]);
    }

    self->weaks = OArray_new_dyn(self, NULL, sizeof(struct parent *), 0, 8);
}

//
// public
//


OJoin *OJoin_new_super(osize object_size, oobj *parents, osize parents_size, struct o_allocator_i allocator)
{
    assert(object_size >= (osize) sizeof(OJoin));
    OJoin stacked_self;
    join_init_base(&stacked_self, allocator);
    OJoin *self = o_alloc0(&stacked_self, object_size, 1);
    *self = stacked_self;
    // needs a fixed OJoin object handle...
    init_parents_weaks(self, parents, parents_size);
    return self;
}


//
// virtual implementations:
//

void OJoin__v_del(oobj obj)
{
    OObj_assert(obj, OJoin);
    OJoin *self = obj;

    bool delete = false;
    o_lock_block(self) {
        if (OArray_num(self->parents) > 0) {
            // nothing to do
            continue;
        }

        //
        // actual delete:
        //
        delete = true;

        while(OArray_num(self->weaks)) {
            struct parent *last = *OArray_at(self->weaks, OArray_num(self->weaks) - 1, struct parent*);
            OJoin__remove_weak(self, last->parent);
        }
    }

    if(delete) {
        OObj__v_del(self);
    }
}

//
// object functions:
//

void OJoin_del(oobj obj)
{
    OObj_assert(obj, OJoin);
    OJoin *self = obj;

    // manual locking, because OJoin_remove may delete self
    o_lock(self);

    // remove all parents
    while (OArray_num(self->parents)) {
        struct parent *last = *OArray_at(self->parents, OArray_num(self->parents) - 1, struct parent*);

        // manual locking, OJoin_remove may delete self
        o_unlock(self);

        if (OJoin_remove(self, last->parent)) {
            // got deleted
            return;
        }

        o_lock(self);
    }
    // should not get here
    assert(0);
}

osize OJoin_num_parents(oobj obj)
{
    OObj_assert(obj, OJoin);
    OJoin *self = obj;
    osize num = 0;
    o_lock_block(self) {
        num = o_num(self->parents);
    }
    return num;
}

void OJoin_add(oobj obj, oobj parent)
{
    OObj_assert(obj, OJoin);
    OJoin *self = obj;

    o_lock_block(self) {
        struct parent *p = o_new0(self, struct parent, 1);
        p->self = self;
        p->parent = parent;

        OArray_push(self->parents, &p);

        // create a del callback that is called, when parent gets deleted
        p->delcb = ODelcallback_new(parent, autoremove_parent);
        o_user_set(p->delcb, p);
    }
}

bool OJoin_is_parent(oobj obj, oobj parent)
{
    OObj_assert(obj, OJoin);
    OJoin *self = obj;

    bool found = false;
    o_lock_block(self) {
        for (osize i = 0; i < OArray_num(self->parents); i++) {
            struct parent *p = *OArray_at(self->parents, i, struct parent *);
            if (p->parent == parent) {
                found = true;
                break;
            }
        }
    }
    return found;
}

bool OJoin_remove(oobj obj, oobj parent)
{
    OObj_assert(obj, OJoin);
    OJoin *self = obj;

    bool deleted = false;
    o_lock_block(self) {
        // check if it's the list
        osize idx = -1;
        struct parent *p = NULL;
        for (osize i = 0; i < OArray_num(self->parents); i++) {
            p = *OArray_at(self->parents, i, struct parent *);
            if (p->parent == parent) {
                idx = i;

                // break for, not lock (which would be invalid)
                break;
            }
        }
        if (idx < 0) {
            o_log_error_s(__func__, "parent is not joined?");
            continue;
        }

        // only delete if not called by autodelete ODelcallback
        if(p->delcb) {
            o_user_set(p->delcb, NULL);
            o_del(p->delcb);
        }

        o_free(self, p);
        OArray_pop_at(self->parents, idx, NULL);

        deleted = OArray_num(self->parents) == 0;
    }
    if (deleted) {
        self->super.v_del(self);
    }
    return deleted;
}

