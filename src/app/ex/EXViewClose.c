#include "app/ex/EXViewClose.h"
#include "o/OObj_builder.h"
#include "r/RTex.h"
#include "w/WIcon.h"
#include "w/WBtn.h"
#include "w/WTheme.h"
#include "o/log.h"


//
// public
//

EXViewClose *EXViewClose_init(oobj obj, oobj parent, OObj__event_fn event)
{

    AView *super = obj;
    EXViewClose *self = obj;
    o_clear(self, sizeof *self, 1);

    AView_init(obj, parent, EXViewClose__v_setup, EXViewClose__v_update, EXViewClose__v_render);
    OObj_id_set(self, EXViewClose_ID);

    self->event = event;

    self->theme = WTheme_new_tiny(self);
    self->button = WBtn_new(self);
    WObj_padding_set(self->button, vec4_(2));
    WBtn_style_set(self->button, WBtn_BIG_ROUND);
    oobj icon = WIcon_new(self->button, WTheme_ICON_CROSS);
    WIcon_color_set(icon, vec4_(0.8, 0.1, 0.1, 1.0));
    WObj_padding_set(icon, vec4_(4, 0, 4, 0));

    return self;

}


//
// virtual implementations
//

void EXViewClose__v_setup(oobj view)
{
    // noop
}

void EXViewClose__v_update(oobj view, oobj tex, float dt)
{
    OObj_assert(view, EXViewClose);
    EXViewClose *self = view;

    struct r_proj *proj = RTex_proj(tex);
    vec2 lt = vec2_(r_proj_left_int(proj), r_proj_top_int(proj));
    WTheme_update(self->theme, self->button, lt, vec2_(0), a_pointer);
    if(WBtn_clicked(self->button)) {
        o_log_s(__func__, "button clicked");
        if(self->event) {
            self->event(self);
        }
    }
}

void EXViewClose__v_render(oobj view, oobj tex, float dt)
{
    OObj_assert(view, EXViewClose);
    EXViewClose *self = view;
    WTheme_render(self->theme, tex);
}

//
// object functions:
//
