#ifndef O_OOBJ_ROOT_H
#define O_OOBJ_ROOT_H

/**
 * @file ORoot.h
 *
 * Object
 *
 * The root object type is the only one to have no parent at all.
 * They're the roots of the managed hierarchy.
 * Deleting them (o_del) will delete the whole managed resource tree
 *      and all its allocated memory and objects.
 */

#include "OObj.h"

/** object id */
#define OObjRoot_ID OObj_ID "ORoot"


/**
 * Initializes a object root, (so no parent).
 * Call its deletor to delete the full managed object tree
 * @param obj The object to initialize
 * @param allocator the allocator to allocate from in the whole tree
 * @return obj casted as OObj*
 */
O_EXTERN
OObj *OObjRoot_init(oobj obj, struct o_allocator_i allocator);


/**
 * Creates a object root, (so no parent).
 * Call its deletor to delete the full managed object tree
 * @param allocator the allocator to allocate from in the whole tree
 * @return The new allocated root OObj
 */
O_INLINE
OObj *OObjRoot_new(struct o_allocator_i allocator)
{
    OObj stacked_self;
    OObjRoot_init(&stacked_self, allocator);
    OObj *self = o_new(&stacked_self, OObj, 1);
    *self = stacked_self;
    return self;
}

/**
 * Creates a object root, (so no parent) with the (default) heap allocator.
 * Call its deletor to delete the full managed object tree
 * @return The new allocated root OObj
 */
O_INLINE
OObj *OObjRoot_new_heap(void)
{
    return OObjRoot_new(o_allocator_heap_new());
}

/**
 * Creates a object root, (so no parent) with a pool allocator, which is faster for small allocations
 * Call its deletor to delete the full managed object tree
 * @param block_size maximal size for an allocation to make use of a pool (<=0 for default)
 * @param blocks_in_pool number of blocks in a pool (<=0 for default)
 * @param start_pools number of pools to be created now (<=0 for default)
 * @return The new allocated root OObj
 * @note when deleting this root object, the allocator is >NOT< deleted, see "o_allocator_pool_del".
 */
O_INLINE
OObj *OObjRoot_new_pool_ex(int block_size, int blocks_in_pool, int start_pools)
{
    return OObjRoot_new(o_allocator_pool_new(block_size, blocks_in_pool, start_pools));
}

/**
 * Creates a object root, (so no parent) with a pool allocator, which is faster for small allocations
 * Call its deletor to delete the full managed object tree
 * @return The new allocated root OObj
 * @note uses the pool size defaults.
 *       when deleting this root object, the allocator is >NOT< deleted, see "o_allocator_pool_del".
 */
O_INLINE
OObj *OObjRoot_new_pool(void)
{
    return OObjRoot_new_pool_ex(-1, -1, -1);
}

#endif //O_OOBJ_ROOT_H
