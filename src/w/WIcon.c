#include "w/WIcon.h"
#include "o/OObj_builder.h"
#include "w/WTheme.h"
#include "m/vec/vec2.h"
#include "u/rect.h"

#define O_LOG_LIB "w"
#include "o/log.h"

//
// public
//

WIcon *WIcon_init(oobj obj, oobj parent, enum WTheme_indices icon_idx)
{
    WObj *super = obj;
    WIcon *self = obj;
    o_clear(self, sizeof *self, 1);

    WObj_init(obj, parent);
    OObj_id_set(self, WIcon_ID);

    self->icon_idx = icon_idx;
    self->color = R_WHITE;

    // vfuncs
    super->v_update = WIcon__v_update;

    return self;
}

//
// vfuncs
//

vec2 WIcon__v_update(oobj obj, vec2 lt, vec2 min_size, oobj theme, a_pointer__fn pointer_fn)
{
    OObj_assert(obj, WIcon);
    WIcon *self = obj;

    struct u_atlas atlas = WTheme_atlas(theme);


    vec2 child_size = WObj__v_update(self, lt, min_size, theme, pointer_fn);

    vec2 icon_size = u_atlas_size(atlas, self->icon_idx);

    struct r_rect *rects = WObj__alloc_rects(self, theme, 1);
    rects->uv_rect = u_atlas_rect(atlas, self->icon_idx);
    rects->rect = u_rect_new_lt(m_2(lt), m_2(icon_size));
    rects->s = self->color;
    
    return vec2_max_v(child_size, icon_size);
}
