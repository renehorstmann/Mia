#ifndef O_OJSON_H
#define O_OJSON_H

/**
 * @file OJson.h
 *
 * Object
 *
 * A Json object holds the infos for a single json element.
 * The Json Api makes use of the OObj system,
 * in which a Json objects or arrays children are just allocated by it as parent.
 *
 * The name for object children will be the OObj_name
 *
 * CAUTION: name duplicates can easily occur and are NOT checked!
 *
 * Operators:
 * o_num -> OJson_num
 * o_at -> OJson_at
 */


#include "OObj.h"


/** object id */
#define OJson_ID OObj_ID "OJson"

/** if name==NULL, this will be used */
#define OJson_NAME_DEFAULT "undefined"

enum OJson_type {
    OJson_TYPE_NULL,
    OJson_TYPE_BOOLEAN,
    OJson_TYPE_NUMBER,
    OJson_TYPE_STRING,
    OJson_TYPE_OBJECT,
    OJson_TYPE_ARRAY,
    OJson_NUM_TYPES
};

union OJson_data {
    // null needs no data
    bool boolean;
    double number;
    char *string;
    // object member stored in the resource list
    // array member stored in the resource list
};

typedef struct {
    OObj super;

    enum OJson_type type;
    union OJson_data data;

    // inherit from the parent, set with OJson_packed_set
    bool packed;
} OJson;


/**
 * Initializes the object.
 * If the parent is an OJson object or array, this will get an element in that list (uses the resources list).
 * Also inherits the flag "packed" from the parent.
 * @param obj OJson object
 * @param parent to inherit from (may be a json object or array)
 * @param name if the parent is an object, NULL safe
 * @param type of this OJson element
 * @param data according to type
 *             if type == OJson_TYPE_STRING, data.string will get cloned
 *             if not used it gets cleared
 * @return obj casted as OJson
 */
O_EXTERN
OJson *OJson_init(oobj obj, oobj parent, const char *name, enum OJson_type type, union OJson_data data);

/**
 * Creates a new the OJson object
 * If the parent is an OJson object or array, this will get an element in that list (uses the resources list).
 * Also inherits the flag "packed" from the parent.
 * @param parent to inherit from (may be a json object or array)
 * @param name if the parent is an object, NULL safe
 * @param type of this OJson element
 * @param data according to type
 *             if type == OJson_TYPE_STRING, data.string will get cloned
 *             if not used it gets cleared
 * @return The new object
 */
O_INLINE
OJson *OJson_new(oobj parent, const char *name, enum OJson_type type, union OJson_data data)
{
    OObj_DECL_IMPL_NEW(OJson, parent, name, type, data);
}


/**
 * Creates a new the OJson object
 * If the parent is an OJson object or array, this will get an element in that list (uses the resources list).
 * Also inherits the flag "packed" from the parent.
 * @param parent to inherit from (may be a json object or array)
 * @param name if the parent is an object, NULL safe
 * @param stream OStream object to read a json ascii name from
 * @return The new object, or NULL if parsing failed
 * @note OJson_init_read_stream not available, because we need to create "new" OJson elements for the tree
 * @warning MAY RETURN NULL
 */
O_EXTERN
struct oobj_opt OJson_new_read_stream(oobj parent, const char *name, oobj stream);

/**
 * Creates a new the OJson object
 * If the parent is an OJson object or array, this will get an element in that list (uses the resources list).
 * Also inherits the flag "packed" from the parent.
 * @param parent to inherit from (may be a json object or array)
 * @param name if the parent is an object, NULL safe
 * @param file filepath to an .json name to read in
 * @return The new object, or NULL if parsing failed
 * @note OJson_init_read_file not available, because we need to create "new" OJson elements for the tree-
 *       First reads in the whole file with o_file_read, then uses OJson_new_read_stream with an OStreamArray.
 *       For XXL json files, use OJson_new_read_stream instead with an OStream of o_file_open
 * @warning MAY RETURN NULL
 */
O_EXTERN
struct oobj_opt OJson_new_read_file(oobj parent, const char *name, const char *file);

/**
 * Creates a new the OJson object
 * If the parent is an OJson object or array, this will get an element in that list (uses the resources list).
 * Also inherits the flag "packed" from the parent.
 * @param parent to inherit from (may be a json object or array)
 * @param name if the parent is an object, NULL safe
 * @param string dumped json string
 * @return The new object, or NULL if parsing failed
 * @note OJson_init_read_string not available, because we need to create "new" OJson elements for the tree
 * @warning MAY RETURN NULL
 */
O_EXTERN
struct oobj_opt OJson_new_read_string(oobj parent, const char *name, const char *string);


//
// type specific initializers
//

/**
 * Initializes the object as OJson_TYPE_BOOLEAN element
 * If the parent is an OJson object or array, this will get an element in that list (uses the resources list).
 * Also inherits the flag "packed" from the parent.
 * @param obj OJson object
 * @param parent to inherit from (may be a json object or array)
 * @param name if the parent is an object, NULL safe
 * @return obj casted as OJson
 */
O_INLINE
OJson *OJson_init_null(oobj obj, oobj parent, const char *name)
{
    union OJson_data data = {0};
    return OJson_init(obj, parent, name, OJson_TYPE_NULL, data);
}

/**
 * Creates a new the OJson object as OJson_TYPE_BOOLEAN element
 * If the parent is an OJson object or array, this will get an element in that list (uses the resources list).
 * Also inherits the flag "packed" from the parent.
 * @param parent to inherit from (may be a json object or array)
 * @param name if the parent is an object, NULL safe
 * @return The new object
 */
O_INLINE
OJson *OJson_new_null(oobj parent, const char *name)
{
    OObj_DECL_IMPL_NEW_SPECIAL(OJson, null, parent, name);
}


/**
 * Initializes the object as OJson_TYPE_BOOLEAN element
 * If the parent is an OJson object or array, this will get an element in that list (uses the resources list).
 * Also inherits the flag "packed" from the parent.
 * @param obj OJson object
 * @param parent to inherit from (may be a json object or array)
 * @param name if the parent is an object, NULL safe
 * @param boolean the bool value
 * @return obj casted as OJson
 */
O_INLINE
OJson *OJson_init_boolean(oobj obj, oobj parent, const char *name, bool boolean)
{
    union OJson_data data = {0};
    data.boolean = boolean;
    return OJson_init(obj, parent, name, OJson_TYPE_BOOLEAN, data);
}

/**
 * Creates a new the OJson object as OJson_TYPE_BOOLEAN element
 * If the parent is an OJson object or array, this will get an element in that list (uses the resources list).
 * Also inherits the flag "packed" from the parent.
 * @param parent to inherit from (may be a json object or array)
 * @param name if the parent is an object, NULL safe
 * @param boolean the bool value
 * @return The new object
 */
O_INLINE
OJson *OJson_new_boolean(oobj parent, const char *name, bool boolean)
{
    OObj_DECL_IMPL_NEW_SPECIAL(OJson, boolean, parent, name, boolean);
}


/**
 * Initializes the object as OJson_TYPE_NUMBER element
 * If the parent is an OJson object or array, this will get an element in that list (uses the resources list).
 * Also inherits the flag "packed" from the parent.
 * @param obj OJson object
 * @param parent to inherit from (may be a json object or array)
 * @param name if the parent is an object, NULL safe
 * @param number the double value
 * @return obj casted as OJson
 */
O_INLINE
OJson *OJson_init_number(oobj obj, oobj parent, const char *name, double number)
{
    union OJson_data data = {0};
    data.number = number;
    return OJson_init(obj, parent, name, OJson_TYPE_NUMBER, data);
}

/**
 * Creates a new the OJson object as OJson_TYPE_NUMBER element
 * If the parent is an OJson object or array, this will get an element in that list (uses the resources list).
 * Also inherits the flag "packed" from the parent.
 * @param parent to inherit from (may be a json object or array)
 * @param name if the parent is an object, NULL safe
 * @param number the double value
 * @return The new object
 */
O_INLINE
OJson *OJson_new_number(oobj parent, const char *name, double number)
{
    OObj_DECL_IMPL_NEW_SPECIAL(OJson, number, parent, name, number);
}


/**
 * Initializes the object as OJson_TYPE_STRING element
 * If the parent is an OJson object or array, this will get an element in that list (uses the resources list).
 * Also inherits the flag "packed" from the parent.
 * @param obj OJson object
 * @param parent to inherit from (may be a json object or array)
 * @param name if the parent is an object, NULL safe
 * @param string null terminated and NULL safe (results in element OJson_TYPE_NULL)
 * @return obj casted as OJson
 */
O_INLINE
OJson *OJson_init_string(oobj obj, oobj parent, const char *name, const char *string)
{
    union OJson_data data = {0};
    // will get cloned in init
    data.string = (char*) string;
    return OJson_init(obj, parent, name, OJson_TYPE_STRING, data);
}

/**
 * Creates a new the OJson object as OJson_TYPE_STRING element
 * If the parent is an OJson object or array, this will get an element in that list (uses the resources list).
 * Also inherits the flag "packed" from the parent.
 * @param parent to inherit from (may be a json object or array)
 * @param name if the parent is an object, NULL safe
 * @param string null terminated and NULL safe (results in element OJson_TYPE_NULL)
 * @return The new object
 */
O_INLINE
OJson *OJson_new_string(oobj parent, const char *name, const char *string)
{
    OObj_DECL_IMPL_NEW_SPECIAL(OJson, string, parent, name, string);
}


/**
 * Initializes the object as OJson_TYPE_OBJECT element
 * If the parent is an OJson object or array, this will get an element in that list (uses the resources list).
 * Also inherits the flag "packed" from the parent.
 * @param obj OJson object
 * @param parent to inherit from (may be a json object or array)
 * @param name if the parent is an object, NULL safe
 * @return obj casted as OJson
 */
O_INLINE
OJson *OJson_init_object(oobj obj, oobj parent, const char *name)
{
    union OJson_data data = {0};
    return OJson_init(obj, parent, name, OJson_TYPE_OBJECT, data);
}

/**
 * Creates a new the OJson object as OJson_TYPE_NUMBER element
 * If the parent is an OJson object or array, this will get an element in that list (uses the resources list).
 * Also inherits the flag "packed" from the parent.
 * @param parent to inherit from (may be a json object or array)
 * @param name if the parent is an object, NULL safe
 * @return The new object
 */
O_INLINE
OJson *OJson_new_object(oobj parent, const char *name)
{
    OObj_DECL_IMPL_NEW_SPECIAL(OJson, object, parent, name);
}


/**
 * Initializes the object as OJson_TYPE_ARRAY element
 * If the parent is an OJson object or array, this will get an element in that list (uses the resources list).
 * Also inherits the flag "packed" from the parent.
 * @param obj OJson object
 * @param parent to inherit from (may be a json object or array)
 * @param name if the parent is an object, NULL safe
 * @return obj casted as OJson
 */
O_INLINE
OJson *OJson_init_array(oobj obj, oobj parent, const char *name)
{
    union OJson_data data = {0};
    return OJson_init(obj, parent, name, OJson_TYPE_ARRAY, data);
}

/**
 * Creates a new the OJson object as OJson_TYPE_ARRAY element
 * If the parent is an OJson object or array, this will get an element in that list (uses the resources list).
 * Also inherits the flag "packed" from the parent.
 * @param parent to inherit from (may be a json object or array)
 * @param name if the parent is an object, NULL safe
 * @return The new object
 */
O_INLINE
OJson *OJson_new_array(oobj parent, const char *name)
{
    OObj_DECL_IMPL_NEW_SPECIAL(OJson, array, parent, name);
}

//
// virtual implementations:
//

/**
 * virtual operator function
 * @param obj OJson object
 * @return the number of children if obj type is OJson_TYPE_OBJECT or OJson_TYPE_ARRAY
 * @note calls OJson_num
 */
O_EXTERN
osize OJson__v_op_num(oobj obj);

/**
 * virtual operator function
 * @param obj OJson object
 * @param idx element index
 * @return void * (oobj) of the OJson_TYPE_OBJECT or OJson_TYPE_ARRAY indexed child (or NULL if failed)
 * @note calls OJson_at
 */
O_EXTERN
void *OJson__v_op_at(oobj obj, osize idx);

//
// object functions:
//

/**
 * @param obj OJson object
 * @return the type of this json element
 */
OObj_DECL_GET(OJson, enum OJson_type, type)

/**
 * @param obj OJson object
 * @return the data union of this json element
 */
OObj_DECL_GET(OJson, union OJson_data, data)

/**
 * @param obj OJson object
 * @return a pointer to the OJson boolean data, or NULL if type!=OJson_TYPE_BOOLEAN
 */
O_INLINE
bool *OJson_boolean(oobj obj)
{
    OObj_assert(obj, OJson);
    OJson *self = obj;
    if(self->type != OJson_TYPE_BOOLEAN) {
        return NULL;
    }
    return &self->data.boolean;
}

/**
 * @param obj OJson object
 * @return a pointer to the OJson number data, or NULL if type!=OJson_TYPE_NUMBER
 */
O_INLINE
double *OJson_number(oobj obj)
{
    OObj_assert(obj, OJson);
    OJson *self = obj;
    if(self->type != OJson_TYPE_NUMBER) {
        return NULL;
    }
    return &self->data.number;
}

/**
 * @param obj OJson object
 * @return the string of the OJson, or NULL if type!=OJson_TYPE_STRING
 */
O_INLINE
const char *OJson_string(oobj obj)
{
    OObj_assert(obj, OJson);
    OJson *self = obj;
    if(self->type != OJson_TYPE_STRING) {
        return NULL;
    }
    return self->data.string;
}

/**
 * @param obj OJson object
 * @string the string for the OJson element
 * @note does an o_log_warn if type!=OJson_TYPE_STRING
 */
O_EXTERN
void OJson_string_set(oobj obj, const char *string);


/**
 * @param obj OJson object
 * @return true if the element should be written in packed mode (no newlines)
 */
OObj_DECL_GETSET(OJson, bool, packed)


/**
 * Get the children of a OJson_TYPE_OBJECT or OJson_TYPE_ARRAY
 * @param obj OJson object
 * @param opt_out_num if not NULL, returns the list size/num
 * @return A null terminated list of the OJson children, call o_free(obj, list) to free it manually
 * @note the list is allocated on obj.
 *       Create an OJson **it=list to traverse through the list
 */
O_INLINE
OJson **OJson_list(oobj obj, osize *opt_out_num)
{
    return (OJson **) OObj_list(obj, opt_out_num, OJson);
}

/**
 * Gets the array size of an OJson_TYPE_OBJECT or OJson_TYPE_ARRAY
 * @param obj OJson object
 * @return number of elements in the object or array (0 on type missmatch)
 */
O_EXTERN
osize OJson_num(oobj obj);

/**
 * Get an indexed child of an OJson_TYPE_OBJECT or OJson_TYPE_ARRAY
 * @param obj OJson object
 * @param idx in the OJson_list
 * @return the OJson object with that index or NULL if not in range or wrong type
 */
O_EXTERN
struct oobj_opt OJson_at(oobj obj, osize idx);

/**
 * Get a specific child of an OJson_TYPE_OBJECT
 * @param obj OJson object
 * @param name the name to search
 * @return the OJson object with that name or NULL if not found
 * @note asserts name!=NULL
 */
O_EXTERN
struct oobj_opt OJson_get(oobj obj, const char *name);

/**
 * writes the json element tree into the given stream
 * @param obj OJson object
 * @param stream OStream object
 * @return the number of written characters
 */
O_EXTERN
osize OJson_write_stream(oobj obj, oobj stream);

/**
 * writes the json element tree into the given name
 * @param obj OJson object
 * @param file filepath
 * @return true if succeeded
 * @note first uses OJson_dump and then writes the whole string to file (o_file_write).
 *       For XXL json files, use OJson_write_stream with an file OStream instead (o_file_open)
 */
O_EXTERN
bool OJson_write_file(oobj obj, const char *file);

/**
 * Writes the json element tree into the resulting allocated string
 * @param obj OJson object
 * @param opt_out_len if not NULL, will be set to the string length
 * @return json ascii string null terminated, or NULL if failed
 * @note uses OJson_write with an OStreamArray internally
 */
O_EXTERN
char *OJson_dump(oobj obj, osize *opt_out_len);

#endif //O_OJSON_H
