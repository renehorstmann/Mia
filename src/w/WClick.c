#include "w/WClick.h"
#include "o/OObj_builder.h"
#include "w/WTheme.h"
#include "m/vec/vec2.h"
#include "u/rect.h"

#define O_LOG_LIB "w"
#include "o/log.h"

//
// public
//

WClick *WClick_init(oobj obj, oobj parent)
{
    WObj *super = obj;
    WClick *self = obj;
    o_clear(self, sizeof *self, 1);

    WObj_init(obj, parent);
    OObj_id_set(self, WClick_ID);

    // vfuncs
    super->v_update = WClick__v_update;

    return self;
}

//
// vfuncs
//

vec2 WClick__v_update(oobj obj, vec2 lt, vec2 min_size, oobj theme, a_pointer__fn pointer_fn)
{
    OObj_assert(obj, WClick);
    WClick *self = obj;


    vec2 child_size = WObj__v_update(self, lt, min_size, theme, pointer_fn);
    vec2 size = vec2_max_v(child_size, min_size);

    self->pointer = pointer_fn(0, 0);

    self->pressed = self->clicked = false;


    vec2 pp = self->pointer.pos.xy;
    pp.x = pp.x - lt.x;
    pp.y = pp.y - lt.y;
    self->pos = pp;
    if (0 <= pp.x && pp.x <= size.x
        && 0 >= pp.y && pp.y >= -size.y) {
        if (a_pointer_down(self->pointer, self->prev)) {
            self->down = true;
            self->pressed = true;
            if(self->pressed_event) {
                self->pressed_event(self);
            }
        } else if(self->down && a_pointer_up(self->pointer, self->prev)) {
            self->clicked = true;
            self->down = false;
            if(self->clicked_event) {
                self->clicked_event(self);
            }
        }
    } else {
        self->down = false;
    }
    if (!self->pointer.active) {
        self->down = false;
    }

    self->prev = self->pointer;
    
    return size;
}
