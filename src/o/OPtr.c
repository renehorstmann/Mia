#include "o/OPtr.h"
#include "o/OObj_builder.h"
#include "o/ODelcallback.h"


O_STATIC
void delcallback(oobj delcallback_obj)
{
    oobj obj = o_user(delcallback_obj);
    // if OPtr valid and not NULL, set the reference to the ptr to NULL
    if(OObj_check(obj, OPtr)) {
        OPtr *self = obj;
        self->ptr = NULL;
        self->ptr_delcallback = NULL;
    }
}

O_STATIC
void delete_delcallback(OPtr *self)
{
    if(self->ptr_delcallback) {
        // set reference of this OPtr to NULL in the delcallback
        o_user_set(self->ptr_delcallback, NULL);

        // also sets self->ptr_delcallback to NULL
        o_del(self->ptr_delcallback);
    }
}

//
// public
//

OPtr *OPtr_init(oobj obj, oobj parent, oobj ptr)
{
    OPtr *self = obj;
    o_clear(self, sizeof *self, 1);

    OObj_init(self, parent);
    OObj_id_set(self, OPtr_ID);

    OPtr_set(self, ptr);

    // v del override
    self->super.v_del = OPtr__v_del;

    return self;
}

//
// virtual implementations:
//

void OPtr__v_del(oobj obj)
{
    OObj_assert(obj, OPtr);
    OPtr *self = obj;
    delete_delcallback(self);
    OObj__v_del(self);
}

//
// object functions:
//


void OPtr_set(oobj obj, oobj ptr)
{
    OObj_assert(obj, OPtr);
    OPtr *self = obj;

    delete_delcallback(self);

    self->ptr = ptr;

    if(ptr) {
        // install the ODelcallback
        self->ptr_delcallback = ODelcallback_new(ptr, delcallback);
        OObj_name_set(self->ptr_delcallback, "OPtr_Installed");
        o_user_set(self->ptr_delcallback, self);
    }
}
