#ifndef O_ALLOCATOR_H
#define O_ALLOCATOR_H

/**
 * @file allocator.h
 *
 * Allocator abstraction and implementations.
 * Each object (OObj) has an allocator and inherits it from the parents.
 * A typical allocator is the heap allocator.
 *
 * @threadsafe should be every realloc implementation
 *
 * @note Apart from ALL other objects, etc.. An allocator is NOT an OObj.
 *       Because all objects need an allocator.
 *
 * @sa o_allocator_heap_new, OObjRoot_new
 */

#include "common.h"

// forward declaration for the virtual function
struct o_allocator_i;

/**
 * Virtual allocator function.
 * The realloc serves for all necessary allocator functions.
 *
 * @param iface Allocator interface.
 * @param mem  mem == NULL -> alloc.
 *             mem != NULL -> realloc | free.
 * @param element_size Size of a single element (sizeof(...)).
 * @param num  num <= 0 -> free (if mem != NULL).
 *
 * @return the (re)allocated memory or NULL, if it failed, or (element_size*num)==0
 */
typedef void *(*o_allocator_i__realloc_try_fn)(struct o_allocator_i iface, void *restrict mem, osize element_size,
                                             osize num);

/**
 * Allocator interface.
 * Consisting only of an implementation pointer and the virtual realloc function.
 * name is just for debugging purpose, pointer to static memory
 */
struct o_allocator_i {
    void *impl;
    o_allocator_i__realloc_try_fn realloc_try;
    const char *name;
};

/**
 * handy wrapper for the realloc_try function
 *
 * @param iface Allocator interface.
 * @param mem  mem == NULL -> alloc.
 *             mem != NULL -> realloc | free.
 * @param element_size Size of a single element (sizeof(...)).
 * @param num  num <= 0 -> free (if mem != NULL).
 *
 * @return the (re)allocated memory or NULL, if it failed, or (element_size*num)==0
 */
O_INLINE
void *o_allocator_i_realloc_try(struct o_allocator_i iface, void *mem, osize element_size, osize num)
{
    return iface.realloc_try(iface, mem, element_size, num);
}

/**
 * Creates an allocator, that allocates on the heap (may be the default allocator).
 *
 * @return the allocator interface.
 */
O_EXTERN
struct o_allocator_i o_allocator_heap_new(void);


//
// Pool
//

/**
 * Creates an allocator, that allocates pools of the given size for small objects, bigger fall bag to the heap.
 * @param block_size maximal size for an allocation to make use of a pool (<=0 for default)
 * @param blocks_in_pool number of blocks in a pool (<=0 for default)
 * @param start_pools number of pools to be created now (<=0 for default)
 * @return the allocator interface.
 */
O_EXTERN
struct o_allocator_i o_allocator_pool_new(int block_size, int blocks_in_pool, int start_pools);

/**
 * Deletes the allocator and its allocated pools, but NOT the heap allocated pointers!
 * @param self a reference to the pool interface, which will he cleared
 * @note >NOT< thread safe
 */
O_EXTERN
void o_allocator_pool_del(struct o_allocator_i *self);

/**
 * @param self a reference to the pool interface
 * @return number of the size of a block (maximal byte size to be pooled) 
           or 0 if not a pool allocator
 */
O_EXTERN
int o_allocator_pool_block_size(struct o_allocator_i self);

/**
 * @param self a reference to the pool interface
 * @return number of blocks in a pool
           or 0 if not a pool allocator
 */
O_EXTERN
int o_allocator_pool_blocks_in_pool(struct o_allocator_i self);

/**
 * @param self a reference to the pool interface
 * @return number of allocated pools
           or 0 if not a pool allocator
 */
O_EXTERN
int o_allocator_pool_pools_num(struct o_allocator_i self);


/**
 * @param self a reference to the pool interface
 * @return number of total allocated blocks
           or 0 if not a pool allocator
 */
O_INLINE
int o_allocator_pool_blocks_num(struct o_allocator_i self)
{
    return o_allocator_pool_blocks_in_pool(self)
             * o_allocator_pool_pools_num(self);
}

/**
 * @param self a reference to the pool interface
 * @return number of available blocks
           or 0 if not a pool allocator
 */
O_EXTERN
int o_allocator_pool_blocks_available(struct o_allocator_i self);


/**
 * @param self a reference to the pool interface
 * @return number of total allocated blocks
           or 0 if not a pool allocator
 */
O_INLINE
int o_allocator_pool_blocks_used(struct o_allocator_i self)
{
    return o_allocator_pool_blocks_num(self)
             - o_allocator_pool_blocks_available(self);
}


/**
 * @param self a reference to the pool interface
 * @param mem a pointer which was allocated with this allocator
 * @return true if the given pointer was allocated in a pool (false if allocator is not a pool)
 * @note assumes that the pointer is the returned allocated pointer, when in the pool
 */
O_EXTERN
bool o_allocator_pool_pointer_pooled(struct o_allocator_i self, const void *mem);



//
// Arena
//

/**
 * Creates an allocator, that allocates an arena.
 * An arena is a pre allocated region, which results in a super fast allocation.
 * Just the arena position is checked and updated on an allocation.
 *      Ok. plus saving some allocation infos in the buffer to enable reallocs o_memcpy...
 *      The position is also updated on an aligned chunk O_ALIGN_SYSTEM_MAX
 * data can only be actually freed, if it was the last one allocated (as with realloc...)
 * The arena can be completely "freed" with the call to o_allocator_arena_clear
 *
 * @param size in bytes for the arena memory region, allocated on the heap
 * @return the allocator interface.
 * @note see the o_allocator_arena_* functions
 *       delete with o_allocator_arena_del     
 * @sa OObjRoot_new to use the object system with this allocator
 *     If you know what you do, just call o_allocator_arena_clear
 *     If unsure, call o_del first
 */
O_EXTERN
struct o_allocator_i o_allocator_arena_new(osize size);

/**
 * Deletes the allocator and its allocated region
 * @param self a reference to the arena interface, which will he cleared
 */
O_EXTERN
void o_allocator_arena_del(struct o_allocator_i *self);

/**
 * frees all memory allocated by the arena.
 * Simply resets the internal used size to 0
 * @param self arena interface
 */
O_EXTERN
void o_allocator_arena_clear(struct o_allocator_i self);

/**
 * @param self arena interface
 * @return the size allocated by o_allocator_arena_new
 */
O_EXTERN
osize o_allocator_arena_size(struct o_allocator_i self);

/**
 * @param self arena interface
 * @return the used bytss in the arena
 */
O_EXTERN
osize o_allocator_arena_used(struct o_allocator_i self);

/**
 * @param self arena interface
 * @return the remaining bytes in the arena
 */
O_INLINE
osize o_allocator_arena_remaining(struct o_allocator_i self) 
{
    return o_allocator_arena_size(self) - o_allocator_arena_used(self);
}


#endif //O_ALLOCATOR_H
