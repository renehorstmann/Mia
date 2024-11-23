#include "o/OObj_builder.h"
#include "o/OObjRoot.h"
#include "o/str.h"
#include <SDL2/SDL_mutex.h>

#define O_LOG_LIB "o"

#include "o/log.h"


// pointer size should be maximal 8 bytes, so the default pool block size of 256
#define RESOURCES_START_CAPACITY 32

// protected
O_EXTERN
void OObj__init_base(oobj obj, struct o_allocator_i allocator)
{
    OObj *self = obj;
    o_clear(self, sizeof *self, 1);

    OObj_id_set(self, OObj_ID);
    self->allocator = allocator;

    // manually alloc memory array
    self->mem_capacity = RESOURCES_START_CAPACITY;

    self->mem = o_allocator_i_realloc_try(self->allocator,
                                          NULL, sizeof *self->mem,
                                          self->mem_capacity);


#ifdef MIA_OPTION_THREAD
    self->mutex = SDL_CreateMutex();
    o_assume(self->mutex, "SDL_CreateMutex failed");
#endif

    self->v_del = OObj__v_del;

    // the mem list is now normally functionable
    self->children = o_new(self, *self->children, RESOURCES_START_CAPACITY);
    self->children_capacity = RESOURCES_START_CAPACITY;
}

O_STATIC
void child_add(OObj *parent, OObj *child)
{
    osize old_num = parent->children_num++;
    if (parent->children_num > parent->children_capacity) {
        parent->children_capacity = old_num * 2;
        parent->children = o_renew(parent, parent->children, oobj, parent->children_capacity);
    }
    parent->children[old_num] = child;
}

O_STATIC
osize child_idx(OObj *parent, OObj *child)
{
    // search back, faster in most cases
    for (osize i = parent->children_num - 1; i >= 0; i--) {
        if (parent->children[i] == child) {
            return i;
        }
    }
    return -1;
}

// does NOT delete it
O_STATIC
void child_rem(OObj *parent, OObj *child)
{
    osize idx = child_idx(parent, child);
    assert(idx >= 0 && "invalid child remove");
    parent->children_num--;
    o_memmove(&parent->children[idx], &parent->children[idx + 1], sizeof *parent->children,
              (parent->children_num - idx));
}


O_STATIC
void mem_add(OObj *self, void *mem)
{
    osize old_num = self->mem_num++;
    if (self->mem_num > self->mem_capacity) {
        self->mem_capacity = old_num * 2;
        // manually relloc
        self->mem = o_allocator_i_realloc_try(self->allocator, self->mem, sizeof *self->mem, self->mem_capacity);
        o_assume(self->mem, "memory list allocation failed");
    }
    self->mem[old_num] = mem;
}

O_STATIC
osize mem_idx(OObj *self, void *mem)
{
    // search back, faster in most cases
    for (osize i = self->mem_num - 1; i >= 0; i--) {
        if (self->mem[i] == mem) {
            return i;
        }
    }
    return -1;
}

// does NOT free it
O_STATIC
void mem_rem(OObj *self, void *mem)
{
    osize idx = mem_idx(self, mem);
    assert(idx >= 0 && "invalid memory remove");
    self->mem_num--;
    o_memmove(&self->mem[idx], &self->mem[idx + 1], sizeof *self->mem, (self->mem_num - idx));
}


//
// public
//


OObj *OObj_init(oobj obj, oobj parent)
{
    OObj_assert(parent, OObj);
    OObj *par = parent;
    OObj *self = obj;

    OObj__init_base(self, par->allocator);

    self->parent = parent;
    child_add(parent, self);

    return self;
}

void OObj__v_del(oobj obj)
{
    OObj_assert(obj, OObj);
    OObj *self = obj;

    // delete children in recursion
    for (osize i = 0; i < self->children_num; i++) {
        OObj *child = self->children[i];
        child->v_del(child);
    }

    bool self_in_mem = false;

    // free memory (also contains the children list)
    for (osize i = 0; i < self->mem_num; i++) {
        if (self->mem[i] == self) {
            self_in_mem = true;
            continue;
        }
        o_allocator_i_realloc_try(self->allocator, self->mem[i], 0, 0);
    }
    // manually free the mem list
    o_allocator_i_realloc_try(self->allocator, self->mem, 0, 0);

#ifdef MIA_OPTION_THREAD
    // kill the mutex
    SDL_DestroyMutex(self->mutex);
#endif

    if (self_in_mem) {
        // manually free self, if it was in the mem list
        o_allocator_i_realloc_try(self->allocator, self, 0, 0);
    } else {
        o_clear(self, sizeof *self, 1);
    }
}


void OObj_id_mismatch_log_wtf(oobj obj, const char *id, const char *func)
{
    if (OObj_check(obj, OObj)) {
        o_log_wtf_s(__func__, "OObj type error, wanted: \"%s\", got: \"%s\", at: %s", id, OObj_id(obj), func);
    } else if (!obj) {
        o_log_wtf_s(__func__, "OObj type error, wanted: \"%s\", got NULL!, at: %s", id, func);
    } else {
        char first_chars[32] = {0};
        strncpy(first_chars, obj, (sizeof first_chars) - 1);
        o_log_wtf_s(__func__, "OObj type error, wanted: \"%s\", got garbage: \"%s\", at: %s", id, first_chars, func);
    }
}

void OObj_name_set(oobj obj, const char *opt_name)
{
    OObj_assert(obj, OObj);
    OObj *self = obj;
    o_lock_block(obj) {
        self->opt_name = o_str_clone_realloc(obj, self->opt_name, opt_name);
    }
}


#ifdef MIA_OPTION_THREAD

void o_lock(oobj obj)
{
    OObj_assert(obj, OObj);
    OObj *self = obj;
    int ret = SDL_LockMutex(self->mutex);
    o_assume(ret != -1, "SDL_LockMutex failed");
}

bool o_lock_try(oobj obj)
{
    OObj_assert(obj, OObj);
    OObj *self = obj;
    int ret = SDL_TryLockMutex(self->mutex);
    o_assume(ret != -1, "SDL_TryLockMutex failed");
    return ret == 0;
}

void o_unlock(oobj obj)
{
    OObj_assert(obj, OObj);
    OObj *self = obj;
    int ret = SDL_UnlockMutex(self->mutex);
    o_assume(ret != -1, "SDL_UnLockMutex failed");
}

#endif // MIA_OPTION_THREAD


// recursice function
// returns NULL if not found, else the object in which mem exists in the memory list
O_STATIC
OObj *mem_search_r(OObj *self, void *mem, oi32 r_level)
{
    // mem is in the memory list?
    if (mem_idx(self, mem) >= 0) {
        return self;
    }

    if (r_level <= 0) {
        return NULL;
    }

    // recursion
    for (int i = 0; i < self->children_num; i++) {
        OObj *found = mem_search_r(self->children[i], mem, r_level - 1);
        if (found) {
            return found;
        }
    }
    return NULL;
}


// recursice function
// returns NULL if not found, else the object in which mem exists in the child is parent
O_STATIC
OObj *child_search_r(OObj *self, OObj *child, oi32 r_level)
{
    // child is in the children list?
    if (child_idx(self, child) >= 0) {
        return self;
    }

    if (r_level <= 0) {
        return NULL;
    }

    // recursion
    for (int i = 0; i < self->children_num; i++) {
        OObj *found = child_search_r(self->children[i], child, r_level - 1);
        if (found) {
            return found;
        }
    }
    return NULL;
}

struct oobj_opt OObj_mem_search_parent(oobj obj, void *mem, oi32 r_level)
{
    OObj_assert(obj, OObj);
    OObj *self = obj;

    OObj *owner;
    o_lock_block(self) {
        owner = mem_search_r(self, mem, o_max(0, r_level));
    }
    return oobj_opt(owner);
}


void o_mem_move(oobj obj, oobj into, void *mem)
{
    if (mem == NULL) {
        // noop
        return;
    }
    OObj_assert(obj, OObj);
    OObj_assert(into, OObj);
    OObj *self = obj;
    OObj *in = into;

    assert(self->allocator.impl == in->allocator.impl
           && self->allocator.realloc_try == in->allocator.realloc_try
           && "must share the same allocator!");

    o_lock_block(self) {
        // get the object which owns the resource
        struct oobj_opt owner_opt = OObj_mem_search_parent(self, mem, oi32_MAX);
        OObj *owner = owner_opt.o;
        assert(owner != NULL && "memory not found in the hierarchy?");

        // move
        mem_rem(owner, mem);
        mem_add(in, mem);
    }
}


struct oobj_opt OObj_child_search_parent(oobj obj, oobj child, oi32 r_level)
{
    OObj_assert(obj, OObj);
    OObj *self = obj;

    OObj *owner;
    o_lock_block(self) {
        owner = child_search_r(self, child, o_max(0, r_level));
    }
    return oobj_opt(owner);
}


void o_move(oobj obj, oobj into)
{
    if (obj == NULL) {
        // noop
        return;
    }

    assert(OObj_child_search_parent(obj, into, oi32_MAX).o == NULL
           && "into must not be inside the obj children hierarchy");

    OObj_assert(obj, OObj);
    OObj_assert(into, OObj);
    OObj *self = obj;
    OObj *in = into;

    OObj_assert(self->parent, OObj);
    OObj *parent = self->parent;


    assert(parent->allocator.impl == in->allocator.impl
           && parent->allocator.realloc_try == in->allocator.realloc_try
           && "must share the same allocator!");

    o_lock_block(self) {
        // move
        child_rem(parent, self);
        child_add(in, self);
        self->parent = in;
    }

}


oobj *OObj_list_id(oobj obj, osize *opt_out_num, const char *id)
{
    OObj_assert(obj, OObj);
    OObj *self = obj;

    oobj *list = NULL;
    osize num = 0;
    o_lock_block(self) {
        list = o_new(obj, oobj, self->children_num + 1);
        for (osize i = 0; i < self->children_num; i++) {
            if (OObj_id_check(self->children[i], id)) {
                list[num++] = self->children[i];
            }
        }
        // null terminated list
        list[num] = NULL;
    }

    o_opt_set(opt_out_num, num);
    return list;
}





// recursive find child (if full is true)
O_STATIC
OObj *find_child_r(OObj *self, const char *id, const char *opt_name, oi32 r_level)
{
    for (osize i = 0; i < self->children_num; i++) {
        if (!OObj_id_check(self->children[i], id)) {
            continue;
        }
        OObj *child = self->children[i];
        if (!opt_name || o_str_equals(child->opt_name, opt_name)) {
            return child;
        }
    }

    if (r_level <= 0) {
        return NULL;
    }
    for (osize i = 0; i < self->children_num; i++) {
        OObj *found = find_child_r(self->children[i], id, opt_name, r_level - 1);
        if (found) {
            return found;
        }
    }
    return NULL;
}


struct oobj_opt OObj_find_id(oobj obj, const char *id, const char *opt_name, oi32 r_level)
{
    if (!obj) {
        return oobj_opt(NULL);
    }
    OObj_assert(obj, OObj);
    OObj *self = obj;
    OObj *search = NULL;
    o_lock_block(obj) {
        if (OObj_id_check(self, id)) {
            if (!opt_name || o_str_equals(self->opt_name, opt_name)) {
                search = self;
            }
        }

        if (!search) {
            search = find_child_r(self, id, opt_name, o_max(0, r_level));
        }
    }
    return oobj_opt(search);
}


// recursive find parent (if full is true)
O_STATIC
OObj *find_parent_r(OObj *self, const char *id, const char *opt_name, oi32 r_level)
{
    OObj *parent = self->parent;
    if (parent == NULL) {
        return NULL;
    }
    OObj *found = NULL;
    if (OObj_id_check(parent, id)) {
        found = parent;
        if (opt_name && !o_str_equals(parent->opt_name, opt_name)) {
            found = NULL;
        }
    }
    if (found) {
        return found;
    }

    if (r_level <= 0) {
        return NULL;
    }
    return find_parent_r(parent, id, opt_name, r_level - 1);
}

struct oobj_opt OObj_find_parent_id(oobj obj, const char *id, const char *opt_name, oi32 r_level)
{
    if (!obj) {
        return oobj_opt(NULL);
    }
    OObj_assert(obj, OObj);
    OObj *self = obj;
    OObj *search = NULL;
    o_lock_block(obj) {
        search = find_parent_r(self, id, opt_name, o_max(0, r_level));
    }
    return oobj_opt(search);
}

O_STATIC
void print_indent(int lvl)
{
    for (int ident = 0; ident < 2 * lvl; ident++) {
        printf(" ");
    }
}

O_STATIC
void children_print_r(OObj *self, int lvl, oi32 r_level)
{
    for (osize i = 0; i < self->children_num; i++) {
        OObj *child = self->children[i];
        print_indent(lvl);
        if (child->opt_name) {
            printf("%s (%s) at: %p\n", child->id, child->opt_name, (void *) child);
        } else {
            printf("%s at: %p\n", child->id, (void *) child);
        }
        if (r_level > 0) {
            children_print_r(child, lvl + 1, r_level - 1);
        }
    }
}

void OObj_children_debug_print(oobj obj, oi32 r_level)
{
    OObj_assert(obj, OObj);
    OObj *self = obj;
    o_lock_block(self) {
        children_print_r(self, 0, o_max(0, r_level));
    }
    fflush(stdout);
}

void *o_realloc_try(oobj obj, void *mem, osize element_size, osize num)
{
    OObj_assert(obj, OObj);
    OObj *self = obj;

    void *ret = NULL;

    o_lock_block(self) {

        // new allocation -> new entry in the memory list
        if (!mem) {
            mem = o_allocator_i_realloc_try(self->allocator, NULL, element_size, num);
            if (mem) {
                mem_add(self, mem);
            }
            ret = mem;
            continue;
        }

        // search object which holds the memory
        struct oobj_opt owner_opt = OObj_mem_search_parent(obj, mem, oi32_MAX);
        OObj *owner = owner_opt.o;
        assert(owner && "allocation not found in the object hierarchy tree");

        o_lock_block(owner) {

            osize idx = mem_idx(owner, mem);
            assert(idx >= 0 && "allocation not found in the object");

            osize n = element_size * num;

            // realloc or free
            void *old_mem = mem;
            mem = o_allocator_i_realloc_try(owner->allocator, mem, element_size, num);

            if (n <= 0) {
                // free
                mem_rem(owner, old_mem);
                ret = NULL;
            } else {
                // realloc
                owner->mem[idx] = mem;
                ret = mem;
            }

        } //lock owner
    } //lock self

    return ret;
}

void OObj_del(oobj obj)
{
    if (!obj) {
        return;
    }
    OObj_assert(obj, OObj);
    OObj *self = obj;

    if (self->parent) {
        OObj_assert(self->parent, OObj);
        OObj *parent = self->parent;
        child_rem(parent, self);
    }

    self->v_del(self);
}

OObj *OObjRoot_init(oobj obj, struct o_allocator_i allocator)
{
    o_init_assert();

    OObj *self = obj;
    OObj__init_base(self, allocator);
    OObj_id_set(self, OObjRoot_ID);
    return self;
}
