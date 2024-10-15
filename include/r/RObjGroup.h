#ifndef R_ROBJGROUP_H
#define R_ROBJGROUP_H

/**
 * @file RObjGroup.h
 *
 * Object
 *
 * Renders all (direct) child RObj's 
 */
 
#include "RObj.h"
#include "RTex.h"
#include "quad.h"

/** object id */
#define RObjGroup_ID RObj_ID "Group"

typedef struct {
    RObj super;
} RObjGroup;


/**
 * Initializes the object
 * @param obj RObjGroup object
 * @param parent to inherit from
 * @return obj casted as RObjGroup
 */
O_EXTERN
RObjGroup *RObjGroup_init(oobj obj, oobj parent);


/**
 * Creates a new RObjGroup object
 * @param parent to inherit from
 * @return The new object
 */
O_INLINE
RObjGroup *RObjGroup_new(oobj parent)
{
    OObj_DECL_IMPL_NEW(RObjGroup, parent);
}




//
// virtual implementations:
//

O_EXTERN
void RObjGroup__v_update(oobj obj);

O_EXTERN
void RObjGroup__v_render(oobj obj, oobj tex, const struct r_proj *proj);

//
// object functions:
//

/**
 * Get the children allocated on this group
 * @param obj RObjGroup object
 * @param opt_out_num if not NULL, returns the list size/num
 * @return A null terminated list of the RObj children, call o_free(obj, list) to free it manually
 * @note the list is allocated on obj.
 *       Create an RObj **it=list to traverse through the list
 */
O_INLINE
RObj **RObjGroup_list(oobj obj, osize *opt_out_size) {
    return (RObj **) OObj_list(obj, opt_out_size, RObj);
}

#endif //R_ROBJGROUP_H
