#include "r/RObjGroup.h"
#include "o/OObj_builder.h"

#define O_LOG_LIB "r"
#include "o/log.h"


//
// public
//


RObjGroup *RObjGroup_init(oobj obj, oobj parent)
{
    RObj *super = obj;
    RObjGroup *self = obj;
    o_clear(self, sizeof *self, 1);

    RObj_init(obj, parent, RObjGroup__v_update, RObjGroup__v_render);
    OObj_id_set(self, RObjGroup_ID);

    return self;
}


//
// virtual implementations:
//

void RObjGroup__v_update(oobj obj)
{
    RObj **list = RObjGroup_list(obj, NULL);

    RObj **it = list;
    while (*it) {
        RObj *child = *it++;
        RObj_update(child);
    }

    o_free(obj, list);
}

void RObjGroup__v_render(oobj obj, oobj tex, const struct r_proj *proj)
{
    RObj **list = RObjGroup_list(obj, NULL);

    RObj **it = list;
    while (*it) {
        RObj *child = *it++;
        RObj_render_ex(child, tex, proj, false);
    }

    o_free(obj, list);
}
