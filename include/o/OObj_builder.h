#ifndef O_OOBJ_BUILDER_H
#define O_OOBJ_BUILDER_H

/**
 * @file OObj_builder.h
 *
 * Object extension
 *
 * OObj functions that are necessary to create new object types
 */


#include "OObj.h"

/**
 * Set the id of an object, should only be done in *_init constructors!
 * @param obj The object to set the id on
 * @param id The new id for the object
 */
O_INLINE
void OObj_id_set(oobj obj, const char *id)
{
    assert(id && strlen(id) <= OObj_ID_MAX && "type id NULL or too long ");
    OObj *self = (OObj *) obj;
    strcpy(self->id, id);
    OObj_assert(self, OObj);
}

/**
 * Default virtual destructor for the vfunc v_del.
 * When overwriting the v_del deletor, direct childs must call this function at the end of their v_del function.
 *      like *_init, but in opposite direction...
 * OObj´s are typically deleted with the "o_del" macro, with OObj_resource_del on the parent
 * @param obj The object to delete
 */
O_EXTERN
void OObj__v_del(oobj obj);


#endif //O_OOBJ_BUILDER_H
