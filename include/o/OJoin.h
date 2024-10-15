#ifndef O_OJOIN_H
#define O_OJOIN_H

/**
 * @file OJoin.h
 *
 * Object
 *
 * A Join combines multiple object tree branches together.
 * So the only object to have multiple parents.
 * If all parents are deleted (or removed manually) the OJoin deletes itself.
 * To work with parents that got different allocators,
 *      the OJoin object itselfs acts as a root object with its own allocator
 * So the OObj field .parent is NULL!
 * Do NOT call o_del on OJoin objects!
 *      Call OJoin_del instead to delete the object, if you really want to.
 *      Normal way would be deleting all parents.
 *
 * Can be used for shared resources like a traditional shared ptr with ref counting
 *
 * @sa OWeakjoin for special weak parents, that may get a temporary parent of the OJoin
 */


#include "OObj.h"


/** object id */
#define OJoin_ID OObj_ID "OJoin"


typedef struct {
    OObj super;

    // list of parents (internal struct), if dropping to 0, the OJoin deletes itself
    oobj parents;

    // list of weak parents (internal struct) (must be OWeak)
    // an OWeakjoin can check if the OJoin is valid, and get a temporary parent to the OJoin
    oobj weaks;
} OJoin;


/**
 * Creates a new the OJoin object.
 * This function can be used to init an derived OJoin (for thread safe stuff as an example)
 * @param object_size size to allocate (asserts >= sizeof(OJoin)
 * @param parents a list of parents to inherit and join
 * @param parents_size must be at least one parent (asserts), or <=0 for a null terminated parents list
 * @param allocator the allocator to allocate from in the whole tree
 * @return The new object
 */
O_EXTERN
OJoin *OJoin_new_super(osize object_size, oobj *parents, osize parents_size, struct o_allocator_i allocator);

/**
 * Creates a new the OJoin object
 * @param parents a list of parents to inherit and join
 * @param parents_size must be at least one parent (asserts), or <=0 for a null terminated parents list
 * @param allocator the allocator to allocate from in the whole tree
 * @return The new object
 */
O_INLINE
OJoin *OJoin_new(oobj *parents, osize parents_size, struct o_allocator_i allocator)
{
    return OJoin_new_super(sizeof(OJoin), parents, parents_size, allocator);
}


/**
 * Creates a new the OJoin object, using o_allocator_heap_new as allocator
 * @param ... a va list of parents to inherit and join
 * @return The new object
 */
#define OJoin_new_heap(...) \
OJoin_new((oobj[]) { __VA_ARGS__, NULL }, -1, o_allocator_heap_new())


//
// virtual implementations:
//

/**
 * Deletes only if parents_size == 0
 * @param obj OJoin object
 * @threadsafe
 */
O_EXTERN
void OJoin__v_del(oobj obj);

//
// object functions:
//

/**
 * Deletor that will delete the OJoin object manually.
 * Removes all parents and calls the deletor
 * @param obj OJoin object to delete
 * @threadsafe
 * @note BEWARE even if threadsafe, other threads may suggest that they're parent of this dead OJoin!
 */
O_EXTERN
void OJoin_del(oobj obj);

/**
 * @param obj OJoin object
 * @return current number of parents, if not locked, could change immediately...
 */
O_EXTERN
osize OJoin_num_parents(oobj obj);

/**
 * Adds a parent to the join.
 * @param obj OJoin object
 * @param parent new parent to join.
 * @threadsafe
 */
O_EXTERN
void OJoin_add(oobj obj, oobj parent);

/**
 * @param obj OJoin object
 * @param parent to check in the parents list of OJoin
 * @return true if the given parent is also in the parent list
 * @threadsafe
 */
O_EXTERN
bool OJoin_is_parent(oobj obj, oobj parent);

/**
 * Manually removes that parent from the list of parents.
 * If parents_size == 0, the OJoin will get deleted
 * @param obj OJoin object
 * @param parent parent to remove, logs on error
 * @return true is OJoin got deleted
 * @threadsafe
 */
O_EXTERN
bool OJoin_remove(oobj obj, oobj parent);



#endif //O_OJOIN_H
