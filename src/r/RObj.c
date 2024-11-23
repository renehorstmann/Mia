#include "r/RObj.h"
#include "r/RTex.h"
#include "o/OObj_builder.h"

RObj *RObj_init(oobj obj, oobj parent, OObj__event_fn update_fn, RObj__render_fn render_fn)
{
    RObj *self = obj;
    o_clear(self, sizeof *self, 1);

    OObj_init(self, parent);
    OObj_id_set(self, RObj_ID);

    // vfuncs
    self->v_update = update_fn;
    self->v_render = render_fn;

    return self;
}

void RObj_render_ex(oobj obj, oobj tex, const struct r_proj *opt_proj, bool update)
{
    OObj_assert(obj, RObj);
    RObj *self = obj;
    if(update) {
        self->v_update(self);
    }

    // v funcs proj are NOT optional
    opt_proj = o_or(opt_proj, RTex_proj(tex));

    self->v_render(self, tex, opt_proj);
}
