#ifndef O_OOBJ_H
#define O_OOBJ_H

/**
 * @file OObj.h
 *
 * Object
 *
 * The object base class for all objects (like in Java).
 * So each object will get its properties like:
 *  - class name as id, built up like: "OObjFoo" for a class called Foo,
 *          or "OObjFooBar" for the class Bar, that inherits Foo
 *  - allocator, which is inherit|copied by its parent
 *  - mem(ory):
 *    - list of allocates memory on this object
 *    - all free'd on delete
 *    - the ist itself is also part of the list
 *  - children:
 *    - list of OObj children to form the object tree hierarchy, all delete'd on delete
 *  - a parent object (exists so "o_del" can work to del an object created by *_new)
 *  - mutex to synchronize multi threaded code (also locks on resource functions)
 *      only for MIA_OPTION_THREAD
 *  - a virtual deletor function
 *  - optional instance name, may be set by the library (see for example: OJson)
 *  - userdata, which is not used by the library and really for user data, see o_user_ref
 *          (Internal objects may use it btw...)
 *
 *  You can and should allocate memory with an object,
 *      that's why every object needs a parent and lots of functions need an object to allocate stuff.
 *      See o_alloc, o_realloc, o_free, o_new, ...
 *  So its easy to keep track and manage resources with this managed object tree hierarchy.
 *      As an example: A function could create a container object, allocate stuff
 *                     and has only to free|delete the container.
 *                     If the function needs to return an allocated memors, use o_mem_move
 *
 *  Normally, every object function has the object name as prefix, here OObj_*,
 *      but this base object is a special case and has some frequently used functions with just the o_ prefix.
 *      Like o_lock, o_alloc, ...
 *
 *  All fields in an object are "pseudo" private.
 *      Meaning that the user dont needs to access them and should use object functions instead.
 *  Also objects are defined by the "typedef struct { ... } ObjectName;"
 *      in contrast to other data types: (pod: "struct name { ... }" );
 *
 *  Have a look at some other objects like OArray, or its subobject OArray,
 *      if you want to see how to create|inherit objects
 *
 *  Because every object needs a parent, a special object called OObjRoot exists,
 *  which will be the root of the managed hierarchy, see OObjRoot.h
 */

#include "common.h"
#include "allocator.h"

/** Maximal length of an object ID. @sa OObj_ID_BUFFER_SIZE */
#define OObj_ID_MAX 63

/** Buffer size for the null terminated string */
#define OObj_ID_BUFFER_SIZE (OObj_ID_MAX + 1)

/**
 * Each object has a virtual delete function.
 * @param obj: The object to delete
 * @note If you overwrite this virtual function,
 *       call the super deletor at the end of your deletor function
 */
typedef void (*OObj__del_fn)(oobj obj);

/**
 * Some objects can create events. Just a general typedef for this kind of stuff
 * @param obj: The object that called the event.
 */
typedef void (*OObj__event_fn)(oobj obj);

/**
 * Optional virtual num operator function
 * @param obj: OObj object
 * @return for example number of elements, if the object is an array or smth
 */
typedef osize (*OObj__op_num_fn)(oobj obj);

/**
 * Optional virtual at operator function
 * @param obj: OObj object
 * @param idx: index to get an element/object reference
 * @return for example a specific array element
 */
typedef void *(*OObj__op_at_fn)(oobj obj, osize idx);


/**
 * Base ID for all objects.
 * For inheritance, append to the mother ID like:
 *      #define Foo_ID OObj_ID "Foo"   // -> child of OObj so: "OObjFoo"
 *      #define Bar_ID Foo_ID "Bar"    // -> child of Foo which is child of OObj so: "OObjFooBar
 */
#define OObj_ID "OObj"


/**
 * Object base class.
 */
typedef struct {
    /**
     * Object type id, always null terminated
     * Each object must inherit from (this) base object OObj.
     * An object like Bar : Foo : OObj would get the id "OObjFooBar"
     * @note must not be changed after init
     *       When printing an oobj with %s it will print this id
     * @sa OBJ_ID_MAX for the maximal length
     */
    char id[OObj_ID_BUFFER_SIZE];

    /**
     * Each object may have a name that can be set with OObj_name_set.
     * It has no meaningful purpose, but objects can be found this way
     * and better printed for debugging.
     * Some objects may set this name automatically,
     *      like OJson with the given name, or OThread, name Stream name, ...
     */
    char *opt_name;

    /**
     * Each object may reference some userdata.
     * Not used by the library itself. (Internal objects may use it btw...)
     * Could be set for example to an allocated (on this object) struct of data.
     * @sa o_user_ref
     */
    void *userdata;

    /**
     * The allocator to create memory resources
     * Copied from the parent
     */
    struct o_allocator_i allocator;

    /** parent in the resource hierarchy, needed (among others) for o_del */
    oobj parent;

#ifdef MIA_OPTION_THREAD
    /** each object holds its own mutex, see o_lock, o_unlock, o_lock_block */
    void *mutex;
#endif

    /** managed memory, will all be free'd in the deletor */
    void **mem;
    osize mem_capacity;
    osize mem_num;
    
    /** managed OObj children, will be delete'd in the deletor */
    oobj *children;
    osize children_capacity;
    osize children_num;
    
    //
    // vfuncs
    //

    /**
     * virtual delete function
     * to delete OObj`s, call "o_del" which will delete itself with OObj_child_del through the parent
     */
    OObj__del_fn v_del;

    /**
     * virtual optional operator function (may be NULL).
     * returns the number of elements for something like an array.
     */
     OObj__op_num_fn v_op_num;

    /**
     * virtual optional operator function (may be NULL).
     * returns an element from something like an array.
     */
     OObj__op_at_fn v_op_at;
} OObj;




//
// id functions
//

/**
 * @return true if the given objects id begins with the given id
 *         So an object of type OObjFooBar would be true for id "OObjFoo", but NOT vice versa
 * @threadsafe because ids must not be changed
 */
O_INLINE
bool OObj_id_check(oobj obj, const char *id)
{
    assert(id && "id must not be NULL");
    OObj *self = (OObj *) obj;
    // uses strlen instead of o_strlen, because we are in a static header function
    return self != NULL && strncmp(self->id, id, strlen(id)) == 0;
}

/**
 * @param obj The object
 * @param type an Object type like OObjData (without "")
 * @return true if the obj type matches
 * @note concatenates _ID to the type, so OObjData would be OObjData_ID which will be "OObjData"
 * @threadsafe because ids must not be changed
 */
#define OObj_check(obj, type) OObj_id_check((obj), type ##_ID)

/**
 * Logs a wtf message, containing obj info and the wanted id
 */
O_EXTERN
void OObj_id_mismatch_log_wtf(oobj obj, const char *id, const char *func);

/**
 * Same as OObj_id_check, but logs a wtf message, if failed
 * @return true if the given objects id begins with the given id
 *         So an object of type OObjFooBar would be true for id "OObjFoo", but NOT vice versa
 * @threadsafe because ids must not be changed
 */
O_INLINE
bool OObj_id_check_wtf(oobj obj, const char *id, const char *func)
{
    bool check = OObj_id_check(obj, id);
    if(!check) {
        OObj_id_mismatch_log_wtf(obj, id, func);
    }
    return check;
}

/**
 * Same as OObj_id_check, but logs a wtf message, if failed
 * @param obj The object
 * @param type an Object type like OObjData (without "")
 * @return true if the obj type matches
 * @note concatenates _ID to the type, so OObjData would be OObjData_ID which will be "OObjData"
 * @threadsafe because ids must not be changed
 */
#define OObj_check_wtf(obj, type) OObj_id_check_wtf((obj), type ##_ID)

/**
 * Asserts the given object id
 * @param obj The object
 * @param id a string of the object id like "OObjData"
 * @threadsafe because ids must not be changed
 */
#define OObj_assert_id(obj, id) assert(OObj_id_check_wtf((obj), (id), __func__) && "invalid obj")

/**
 * Asserts the given object type
 * @param obj The object
 * @param type an Object type like OObjData (without "")
 * @note concatenates _ID to the type, so OObjData would be OObjData_ID which will be "OObjData"
 * @threadsafe because ids must not be changed
 */
#define OObj_assert(obj, type) assert(OObj_id_check_wtf((obj), type ## _ID, __func__) && "invalid obj, expected:" #type)


/**
 * @param obj OObj object
 * @return the objects id
 * @threadsafe
 */
O_INLINE
const char *OObj_id(oobj obj) {
    OObj_assert(obj, OObj);
    OObj *self = obj;
    return self->id;
}

//
// name functions
//

/**
 * @param obj OObj object
 * @return the objects name, or NULL if not given
 * @note an objects name is optional, can be used to find objects, for debugging, ...
 *       some objects may already define a name, like OJson, OThread, OStream from name
 * @sa OObj_name_set
 * @threadsafe
 */
O_INLINE
const char *OObj_name(oobj obj) {
    OObj_assert(obj, OObj);
    OObj *self = obj;
    return self->opt_name;
}

/**
 * @param obj OObj object
 * @param opt_name new name for the object, or NULL to clear it
 * @note an objects name is optional, can be used to find objects, for debugging, ...
 *       some objects may already define a name, like OJson, OThread, OStream from name
 * @threadsafe
 */
O_EXTERN
void OObj_name_set(oobj obj, const char *opt_name);

//
// userdata
//

/**
 * Returns the userdata of this object
 * Each object may reference some userdata.
 * Not used by the library itself. (Internal objects may use it btw...)
 * Could be set for example to an allocated (on this object) struct of data.
 * @param obj OObj object
 * @return the userdata pointer
 * @note see o_user_set
 * @example
 * struct data {int i;};
 * // ...
 * struct data *d = *o_user_get(obj);
 * d->i = 12345;
 */
O_INLINE
void *o_user(oobj obj)
{
    OObj_assert(obj, OObj);
    OObj *self = obj;
    return self->userdata;
}

/**
 * Sets the userdata of this object
 * Each object may reference some userdata.
 * Not used by the library itself. (Internal objects may use it btw...)
 * Could be set for example to an allocated (on this object) struct of data.
 * @param obj OObj object
 * @param userdata to set into the object
 * @return the given userdata
 * @example
 * struct data {int i;};
 * struct data *d = *o_user_set(obj, o_new(obj, *d, 1));
 * d->i = 12345;
 */
O_INLINE
void *o_user_set(oobj obj, void *userdata)
{
    OObj_assert(obj, OObj);
    OObj *self = obj;
    self->userdata = userdata;
    return self->userdata;
}


//
// operators
//

/**
 * Optional virtual num operator function
 * @param obj: OObj object
 * @return for example number of elements, if the object is an array or smth, or NULL if not defined
 * @note asserts that the operator is available
 */
O_INLINE
osize o_num(oobj obj)
{
    OObj_assert(obj, OObj);
    OObj *self = obj;
    assert(self->v_op_num && "operator not available");
    return self->v_op_num(obj);
}

/**
 * Optional virtual at operator function
 * @param obj: OObj object
 * @param idx: index to get an element/object reference
 * @return for example a specific array element
 * @note asserts that the operator is available
 */
O_INLINE
void *o_at(oobj obj, osize idx)
{
    OObj_assert(obj, OObj);
    OObj *self = obj;
    assert(self->v_op_at && "operator not available");
    return self->v_op_at(obj, idx);
}

/**
 * Optional virtual at operator function.
 * Returns a type * of the element at the given index
 * @param obj OObj object
 * @param idx index to get an element/object reference
 * @param type to cast into (not checked for type equality)
 * @return type * of the element data, or NULL if not defined
 * @note asserts that the operator is available
 *       type not checked for equality, just a convenient wrapper to use the "->" operator
 */
#define o_at_type(obj, idx, type)                        \
((type*) o_at((obj), (idx)))


//
// mutex stuff
//

#ifdef MIA_OPTION_THREAD

/**
 * Locks an objects mutex (each object has its own)
 * Waits until other threads lock em
 * @param obj The object to synchronize
 * @threadsafe
 */
O_EXTERN
void o_lock(oobj obj);

/**
 * Tries to locks an objects mutex (each object has its own)
 * @param obj The object to synchronize
 * @returns false if failed
 * @threadsafe
 */
O_EXTERN
bool o_lock_try(oobj obj);


/**
 * Unlocks an objects mutex (each object has its own)
 * @param obj The object to synchronize
 * @threadsafe
 */
O_EXTERN
void o_unlock(oobj obj);

#else // MIA_OPTION_THREAD

// threading not enabled, just replace the calls with nothing
#define o_lock(obj) ((void)0)
#define o_lock_try(obj) true
#define o_unlock(obj) ((void)0)

#endif // MIA_OPTION_THREAD


/**
 * Create a block in which the mutex is locked and unlocked.
 * Use continue to leave the block.
 * @param obj The object to synchronize
 * @threadsafe
 * @note May be nested, but needs to be in another line (uses __LINE__ internally).
 *       DO NOT:
 *          return in the block!
 *          break in the block! (this block in a for loop...)
 */
#define o_lock_block(obj) \
for(bool O_NAME_CONCAT(o_lock_block__run_, __LINE__) = (o_lock(obj), true); \
O_NAME_CONCAT(o_lock_block__run_, __LINE__); \
O_NAME_CONCAT(o_lock_block__run_, __LINE__) = (o_unlock(obj), false))


//
// resource stuff
//

/**
 * @param obj OObj object
 * @return the objects parent/
 * @note each OObj has a parent object, besides OObjRoot and OJoin!, so may return >NULL<
 * @threadsafe
 */
O_INLINE
oobj OObj_parent(oobj obj) {
    OObj_assert(obj, OObj);
    OObj *self = obj;
    return self->parent;
}

/**
 * @param obj OObj object
 * @return the internal memory list size
 * @threadsafe
 */
O_INLINE
osize OObj_mem_num(oobj obj)
{
    OObj_assert(obj, OObj);
    OObj *self = obj;
    return self->mem_num;
}


/**
 * @param obj OObj object
 * @return the internal children list size
 * @threadsafe
 */
O_INLINE
osize OObj_children_num(oobj obj)
{
    OObj_assert(obj, OObj);
    OObj *self = obj;
    return self->children_num;
}


/**
 * Searches for the parent of the memory in recursion.
 * @param obj The object to find the memory in its object hierarchy
 * @param mem The memory to search
 * @param r_level: <=0 to only search in the given objects memory list, without recursion. 
                   else maximal recursion level, use oi32_MAX for all
 * @return the object which holds the memory directly, or NULL if not found
 * @threadsafe
 */
O_EXTERN
struct oobj_opt OObj_mem_search_parent(oobj obj, void *mem, oi32 r_level);

/**
 * Moves a memory pointer from the object hierarchy of "obj" into object "into".
 * Noop if mem==NULL.
 * Asserts that both objects share the same allocator.
 * Asserts that mem exists in the object hierarchy of obj.
 * @param obj The object hierarchy to search and move the memory from
 * @param into The object to move a memory into
 * @param mem The memory to move
 * @threadsafe
 */
O_EXTERN
void o_mem_move(oobj obj, oobj into, void *mem);


/**
 * Searches for the parent of the object in recursion.
 * @param obj The object to find the child in its object hierarchy
 * @param child OObj to search
 * @param r_level: <=0 to only search in the given objects children list, without recursion. 
                   else maximal recursion level, use oi32_MAX for all
 * @return the object which holds the child directly, or NULL if not found
 * @threadsafe
 */
O_EXTERN
struct oobj_opt OObj_child_search_parent(oobj obj, oobj child, oi32 r_level);

/**
 * Moves object "obj" into object "into".
 * Noop if obj==NULL.
 * Asserts that both objects share the same allocator.
 * @param obj OObj object to move
 * @param into OObj object as new parent
 * @threadsafe
 */
O_EXTERN
void o_move(oobj obj, oobj into);




/**
 * @param obj obj OObj object
 * @param opt_out_num if not NULL, set to the returned list size/num
 * @param id to find in the objects resources
 * @return a null terminated list of objects (allocated on obj)
 *         of the objects with a matching id in the resources list of obj (not recursive!)
 * @threadsafe
 */
O_EXTERN
oobj *OObj_list_id(oobj obj, osize *opt_out_num, const char *id);

/**
 * @param obj The object
 * @param opt_out_num if not NULL, set to the returned list size/num
 * @param type an Object type like OObjData (without "")
 * @return a null terminated list of objects (allocated on obj)
 *         of the objects with a matching id in the resources list of obj (not recursive!).
 *         Call o_free(obj, list) to free it.
 * @note Create an oobj *it=list to traverse through the list
 * @threadsafe
 */
#define OObj_list(obj, opt_out_num, type) OObj_list_id((obj), (opt_out_num), type ##_ID)

/**
 * Searches for the first object with a matching name and id.
 * In recursion mode, first looks for a matching name in the object, than traverses through the objects in recursion
 * @param obj The object to search in, NULL safe
 * @param opt_name To find, must match exactly, or pass NULL to ignore
 * @param id Must also match with OObj_id_check (so true if id="OObjOArray" and the object's id is "OObjOArray")
 * @param r_level: <=0 to only search in the objects resources, else maximal recursion level, use oi32_MAX for all
 * @return the first found object, or NULL if not found
 * @note may return obj directly if it matches
 * @threadsafe
 */
O_EXTERN
struct oobj_opt OObj_find_id(oobj obj, const char *id, const char *opt_name, oi32 r_level);

/**
 * Searches for the first object with a matching name and id.
 * In recursion mode, first looks for a matching name in the object, than traverses through the objects in recursion
 * @param obj The object to search in, NULL safe
 * @param opt_name To find, must match exactly, or pass NULL to ignore
 * @param id Must also match with OObj_id_check (so true if id="OObjOArray" and the object's id is "OObjOArray")
 * @param r_level: <=0 to only search in the objects resources, else maximal recursion level, use oi32_MAX for all
 * @return the first found object, or NULL if not found
 * @note may return obj directly if it matches
 * @threadsafe
 */
#define OObj_find(obj, type, opt_name, r_level) OObj_find_id((obj), type ##_ID, (opt_name), (r_level))

/**
 * Searches for the first parent object with a matching name and id.
 * @param obj The object to search from, NULL safe
 * @param opt_name To find, must match exactly, or pass NULL to ignore
 * @param id Must also match with OObj_id_check (so true if id="OObjOArray" and the object's id is "OObjOArray")
 * @param r_level: <=0 to only search for the objects parent, else maximal recursion level, use oi32_MAX for all
 * @return the first found parent object, or NULL if not found
 * @threadsafe
 */
O_EXTERN
struct oobj_opt OObj_find_parent_id(oobj obj, const char *id, const char *opt_name, oi32 r_level);

/**
 * Searches for the first parent object with a matching name and id.
 * @param obj The object to search from, NULL safe
 * @param opt_name To find, must match exactly, or pass NULL to ignore
 * @param id Must also match with OObj_id_check (so true if id="OObjOArray" and the object's id is "OObjOArray")
 * @param r_level: <=0 to only search in the objects parent, else maximal recursion level, use oi32_MAX for all
 * @return the first found parent object, or NULL if not found
 * @threadsafe
 */
#define OObj_parent_find(obj, type, opt_name, r_level) OObj_find_id((obj), type ##_ID, (opt_name), (r_level))


/**
 * Prints all children in a recursion
 * @param obj OObj object to print from
 * @param r_level: <=0 to only print the direct children, else maximal recursion level, use oi32_MAX for all
 * @threadsafe
 */
O_EXTERN
void OObj_children_debug_print(oobj obj, oi32 r_level);



//
// allocator functions of an object
//

/**
 * @param obj OObj object
 * @return the allocator of this object hierarchy
 */
O_INLINE
struct o_allocator_i OObj_allocator(oobj obj) 
{
    OObj_assert(obj, OObj);
    OObj *self = obj;
    return self->allocator;
}

/**
 * Tries to realloc with the resource list of the given object.
 * On a reallocation or free. The objects hierarchy is traversed to find the memory
 *      using OObj_resource_search_parent.
 * @param obj The object to allocate from (uses its resource list)
 * @param mem mem == NULL -> alloc.
 *            mem != NULL -> realloc | free.
 * @param element_size Size of a single element (sizeof(...)).
 * @param num  num <= 0 -> free (if mem != NULL).
 *
 * @return the (re)allocated memory or NULL, if it failed.
 * @threadsafe
 */
O_EXTERN
void *o_realloc_try(oobj obj, void *mem, osize element_size, osize num);

/**
 * Frees the memory allocated by the given object.
 * @param obj The object to free from (uses its resource list)
 * @param mem the memory to free
 * @threadsafe
 */
O_INLINE
void o_free(oobj obj, void *mem)
{
    o_realloc_try(obj, mem, 0, 0);
}


/**
 * Realloc with the resource list of the given object.
 * Will exit the program if the allocation fails.
 * @param obj The object to allocate from (uses its resource list)
 * @param mem mem == NULL -> alloc.
 *            mem != NULL -> realloc | free.
 * @param element_size Size of a single element (sizeof(...)).
 * @param num  num <= 0 -> free (if mem != NULL).
 *
 * @return the (re)allocated memory
 * @threadsafe
 */
O_INLINE
void *o_realloc(oobj obj, void *mem, osize element_size, osize num)
{
    mem = o_realloc_try(obj, mem, element_size, num);
    o_assume(num <= 0 || mem, "allocation failed");
    return mem;
}

/**
 * Tries to allocate with the resource list of the given object.
 * @param obj The object to allocate from (uses its resource list)
 * @param element_size Size of a single element (sizeof(...)).
 * @param num Numbers of elements to allocate.
 * @return the allocated memory or NULL, if it failed, or (element_size*num)==0.
 * @threadsafe
 */
O_INLINE
void *o_alloc_try(oobj obj, osize element_size, osize num)
{
    return o_realloc_try(obj, NULL, element_size, num);
}

/**
 * Tries to allocate with the resource list of the given object.
 * Also sets the memory to 0 with o_clea.
 * @param obj The object to allocate from (uses its resource list)
 * @param element_size Size of a single element (sizeof(...)).
 * @param num Numbers of elements to allocate.
 *
 * @return the allocated memory or NULL, if it failed, or (element_size*num)==0
 * @threadsafe
 */
O_INLINE
void *o_alloc0_try(oobj obj, osize element_size, osize num)
{
    void *mem = o_realloc_try(obj, NULL, element_size, num);
    if (!mem)
        return NULL;
    o_clear(mem, element_size, num);
    return mem;
}

/**
 * Tries to allocate with the resource list of the given object.
 * Copies the data from to_clone into the allocated memory.
 * @param obj The object to allocate from (uses its resource list)
 * @param to_clone to copy the data from
 * @param element_size Size of a single element (sizeof(...)).
 * @param num Numbers of elements to allocate.
 *
 * @return the allocated memory or NULL, if it failed, or (element_size*num)==0
 * @threadsafe
 */
O_INLINE
void *o_alloc_clone_try(oobj obj, const void *to_clone, osize element_size, osize num)
{
    void *mem = o_realloc_try(obj, NULL, element_size, num);
    if (!mem)
        return NULL;
    o_memcpy(mem, to_clone, element_size, num);
    return mem;
}

/**
 * Allocation with the resource list of the given object.
 * Will exit the program if the allocation fails.
 * @param obj The object to allocate from (uses its resource list)
 * @param element_size Size of a single element (sizeof(...)).
 * @param num Numbers of elements to allocate.
 *
 * @return the allocated memory, or NULL if (element_size*num)==0
 * @threadsafe
 */
O_INLINE
void *o_alloc(oobj obj, osize element_size, osize num)
{
    return o_realloc(obj, NULL, element_size, num);
}

/**
 * Allocation with the resource list of the given object.
 * Will exit the program if the allocation fails.
 * Also sets the memory to 0 with o_clea.
 * @param obj The object to allocate from (uses its resource list)
 * @param element_size Size of a single element (sizeof(...)).
 * @param num Numbers of elements to allocate.
 *
 * @return the allocated memory, or NULL if (element_size*num)==0
 * @threadsafe
 */
O_INLINE
void *o_alloc0(oobj obj, osize element_size, osize num)
{
    void *mem = o_realloc(obj, NULL, element_size, num);
    if(mem) {
        // returns NULL if element_size*num is NULL
        o_clear(mem, element_size, num);
    }
    return mem;
}

/**
 * Allocation with the resource list of the given object.
 * Will exit the program if the allocation fails.
 * Also sets the memory to 0 with o_clea.
 * Copies the data from to_clone into the allocated memory.
 * @param obj The object to allocate from (uses its resource list)
 * @param to_clone to copy the data from
 * @param element_size Size of a single element (sizeof(...)).
 * @param num Numbers of elements to allocate.
 *
 * @return the allocated memory, or NULL if (element_size*num)==0
 * @threadsafe
 */
O_INLINE
void *o_alloc_clone(oobj obj, const void *to_clone, osize element_size, osize num)
{
    void *mem = o_realloc(obj, NULL, element_size, num);
    if(mem) {
        // returns NULL if element_size*num is NULL
        o_memcpy(mem, to_clone, element_size, num);
    }
    return mem;
}

/**
 * Tries to realloc with the resource list of the given object.
 * @param obj The object to allocate from (uses its resource list)
 * @param mem mem == NULL -> alloc.
 *            mem != NULL -> realloc | free.
 * @param type_or_instance type or instance to calculate the sizeof(...)
 * @param num  num <= 0 -> free (if mem != NULL).
 *
 * @return the (re)allocated memory or NULL, if it failed.
 * @threadsafe
 */
#define o_renew_try(obj, mem, type_or_instance, num) \
    o_realloc_try((obj), (mem), sizeof (type_or_instance), (num))


/**
 * Allocation with the resource list of the given object.
 * Will exit the program if the allocation fails.
 * @param obj The object to allocate from (uses its resource list)
 * @param type_or_instance type or instance to calculate the sizeof(...)
 * @param num Numbers of elements to allocate.
 *
 * @return the allocated memory
 * @threadsafe
 */
#define o_new_try(obj, type_or_instance, num) \
    o_alloc_try((obj), sizeof (type_or_instance), (num))


/**
 * Allocation with the resource list of the given object.
 * Will exit the program if the allocation fails.
 * Also sets the memory to 0 with o_clea
 * @param obj The object to allocate from (uses its resource list)
 * @param type_or_instance type or instance to calculate the sizeof(...)
 * @param num Numbers of elements to allocate.
 *
 * @return the allocated memory
 * @threadsafe
 */
#define o_new0_try(obj, type_or_instance, num) \
    o_alloc0_try((obj), sizeof (type_or_instance), (num))


/**
 * Tries to allocate with the resource list of the given object.
 * Copies the data from to_clone into the allocated memory.
 * @param obj The object to allocate from (uses its resource list)
 * @param instance_ptr to copy the data from
 * @param type_or_instance type or instance to calculate the sizeof(...)
 * @param num Numbers of elements to allocate.
 *
 * @return the allocated memory or NULL, if it failed.
 * @threadsafe
 */
#define o_new_clone_try(obj, instance_ptr, type_or_instance, num) \
    o_alloc_clone_try((obj), instance_ptr, sizeof (type_or_instance), (count))


/**
 * Realloc with the resource list of the given object.
 * Will exit the program if the allocation fails.
 * @param obj The object to allocate from (uses its resource list)
 * @param mem mem == NULL -> alloc.
 *            mem != NULL -> realloc | free.
 * @param type_or_instance type or instance to calculate the sizeof(...)
 * @param num  num <= 0 -> free (if mem != NULL).
 *
 * @return the (re)allocated memory
 * @threadsafe
 */
#define o_renew(obj, mem, type_or_instance, count) \
    o_realloc((obj), (mem), sizeof (type_or_instance), (count))


/**
 * Allocation with the resource list of the given object.
 * Will exit the program if the allocation fails.
 * @param obj The object to allocate from (uses its resource list)
 * @param type_or_instance type or instance to calculate the sizeof(...)
 * @param num Numbers of elements to allocate.
 *
 * @return the allocated memory
 * @threadsafe
 */
#define o_new(obj, type_or_instance, count) \
    o_alloc((obj), sizeof (type_or_instance), (count))


/**
 * Allocation with the resource list of the given object.
 * Will exit the program if the allocation fails.
 * Also sets the memory to 0 with o_clea.
 * @param obj The object to allocate from (uses its resource list)
 * @param type_or_instance type or instance to calculate the sizeof(...)
 * @param num Numbers of elements to allocate.
 *
 * @return the allocated memory
 * @threadsafe
 */
#define o_new0(obj, type_or_instance, count) \
    o_alloc0((obj), sizeof (type_or_instance), (count))


/**
 * Allocation with the resource list of the given object.
 * Will exit the program if the allocation fails.
 * Also sets the memory to 0 with o_clea.
 * Copies the data from to_clone into the allocated memory.
 * @param obj The object to allocate from (uses its resource list)
 * @param to_clone to copy the data from
 * @param type_or_instance type or instance to calculate the sizeof(...)
 * @param num Numbers of elements to allocate.
 *
 * @return the allocated memory
 * @threadsafe
 */
#define o_new_clone(obj, instance_ptr, type_or_instance, count) \
    o_alloc_clone((obj), instance_ptr, sizeof (type_or_instance), (count))


//
// initializer
//


/**
 * Initializes an object.
 * May be used as a container or a thread synchronisation mutex.
 *      Container can create (many) resources, which gets deleted by deleting the container...
 * Or called in a inheriting childs init function.
 * Direct child objects must call
 *      their mother / super *_init function first in their init functions
 * @param obj The object to init
 * @param parent The parent to inherit the allocator from,
 *               also this object is added as resource in the parent
 * @return obj casted as OObj*
 * @note object *_init functions NEVER return NULL
 */
O_EXTERN
OObj *OObj_init(oobj obj, oobj parent);


/**
 * Building blocks to create the internal *_new constructor.
 * Allocating the size via its own allocator.
 * see OObj_new example below
 * @param objtype The type of the object like OObjData
 * @param ... parent, <parameter list of init>
 * @note some object *_new function may return NULL (in contrast to *_init, which NEVER return NULL!)
 */
#define OObj_DECL_IMPL_NEW(objtype, ...)                            \
objtype *self = o_new(O_VA_ARGS_FIRST(__VA_ARGS__), objtype, 1);    \
objtype ## _init(self, __VA_ARGS__);\
o_mem_move(O_VA_ARGS_FIRST(__VA_ARGS__), self, self);\
return self



/**
 * Building blocks to create a special internal *_new_* constructor.
 * Allocating the size via its own allocator.
 * @param objtype The type of the object like OObjData
 * @param special The name of the special init constructor (string for _init_string)
 * @param ... parent, <parameter list of init>
 * @note some object *_new function may return NULL (in contrast to *_init, which NEVER return NULL!)
 */
#define OObj_DECL_IMPL_NEW_SPECIAL(objtype, special, ...)           \
objtype *self = o_new(O_VA_ARGS_FIRST(__VA_ARGS__), objtype, 1);    \
objtype ## _init_ ## special(self, __VA_ARGS__);\
o_mem_move(O_VA_ARGS_FIRST(__VA_ARGS__), self, self);\
return self


/**
 * Creates a new OObj
 * May be used as a container or a thread synchronisation mutex.
 *      Container can create (many) resources, which gets deleted by deleting the container...
 * Or called in a inheriting childs init function.
 * @param parent The parent to inherit the allocator from,
 *               also this object is added as resource in the parent
 * @return The new allocated OObj
 * @note some object *_new function may return NULL (in contrast to *_init, which NEVER return NULL!)
 */
O_INLINE
OObj *OObj_new(oobj parent)
{
    OObj_DECL_IMPL_NEW(OObj, parent);
}


//
// deletor
//

/**
 * @param obj OObj object to delete, NULL safe
 * @sa o_del() which also sets obj to NULL
 */
O_EXTERN
void OObj_del(oobj obj);

/**
 * @param self OObj object to delete, NULL safe, will be set to NULL
 */
#define o_del(obj) do { \
    oobj O_NAME_CONCAT(o_del__obj_tmp_, __LINE__) = obj;\
    (obj) = NULL;\
    OObj_del(O_NAME_CONCAT(o_del__obj_tmp_, __LINE__));\
} while(0)



//
// default getter and setter
//

/**
 * Creates a O_INLINE function to get a member of an objects field.
 * @param obj_type OObj name like OArray
 * @param member_type type of the member like int
 * @param member_name name of the member
 */
#define OObj_DECL_GET(obj_type, member_type, member_name)   \
O_INLINE                                                    \
member_type obj_type ## _ ## member_name(oobj obj)          \
{                                                           \
    OObj_assert(obj, obj_type);                             \
    obj_type *self = obj;                                   \
    return self->member_name;                               \
}

/**
 * Creates a O_INLINE function to set (and get) a member of an objects field.
 * @param obj_type OObj name like OArray
 * @param member_type type of the member like int
 * @param member_name name of the member
 */
#define OObj_DECL_SET(obj_type, member_type, member_name)                       \
O_INLINE                                                                        \
member_type obj_type ## _ ## member_name ## _set(oobj obj, member_type set)     \
{                                                                               \
    OObj_assert(obj, obj_type);                                                 \
    obj_type *self = obj;                                                       \
    self->member_name = set;                                                    \
    return self->member_name;                                                   \
}

/**
 * Creates a O_INLINE function to return a reference to a member of an objects field.
 * @param obj_type OObj name like OArray
 * @param member_type type of the member like int
 * @param member_name name of the member
 */
#define OObj_DECL_REF(obj_type, member_type, member_name)       \
O_INLINE                                                        \
member_type * obj_type ## _ ## member_name ## _ref(oobj obj)    \
{                                                               \
    OObj_assert(obj, obj_type);                                 \
    obj_type *self = obj;                                       \
    return &self->member_name;                                  \
}


/**
 * Creates both a getter and setter O_INLINE function to access a member of an objects field.
 * @param obj_type OObj name like OArray
 * @param member_type type of the member like int
 * @param member_name name of the member
 */
#define OObj_DECL_GETSET(obj_type, member_type, member_name) \
OObj_DECL_GET(obj_type, member_type, member_name)            \
OObj_DECL_SET(obj_type, member_type, member_name)


/**
 * Creates both a getter, setter and reference O_INLINE function to access a member of an objects field.
 * @param obj_type OObj name like OArray
 * @param member_type type of the member like int
 * @param member_name name of the member
 */
#define OObj_DECL_GETSETREF(obj_type, member_type, member_name) \
OObj_DECL_GETSET(obj_type, member_type, member_name)            \
OObj_DECL_REF(obj_type, member_type, member_name)

#endif //O_OOBJ_H
