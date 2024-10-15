#ifndef U_BTN_H
#define U_BTN_H

/**
 * @file btn.h
 *
 * button logics. Uses an atlas to set the uv map of an r_quad
 */

#include "o/OObj.h"
#include "r/quad.h"
#include "r/rect.h"
#include "a/pointer.h"
#include "atlas.h"
#include "pose.h"
#include "rect.h"

struct u_btn_state {
    union {
        mat4 stuv;
        struct {
            vec4 s, t, u, v;
        };
    };
    int atlas;
};

O_INLINE
struct u_btn_state u_btn_state_new(void)
{
    struct u_btn_state self = {0};
    self.s = vec4_(1);
    return self;
}

struct u_btn {
    struct r_quad *opt_quad_ref;
    struct r_rect *opt_rect_ref;
    struct u_atlas *opt_atlas_ref;

    // 0 for unpressed, 1 for pressed, >1 for custom
    int mode;

    union {
        struct u_btn_state states[2];
        struct {
            struct u_btn_state state_unpressed, state_pressed;
        };
    };
};

O_INLINE
struct u_btn u_btn_new(struct r_quad *quad_ref, struct u_atlas *opt_atlas_ref, int atlas_unpressed, int atlas_pressed)
{
    struct u_btn self = {0};
    self.opt_quad_ref = quad_ref;
    self.opt_atlas_ref = opt_atlas_ref;
    self.state_unpressed = self.state_pressed = u_btn_state_new();
    self.state_unpressed.atlas = atlas_unpressed;
    self.state_pressed.atlas = atlas_pressed;
    if(self.opt_quad_ref && self.opt_atlas_ref) {
        self.opt_quad_ref->uv = u_atlas_uv(*self.opt_atlas_ref, self.state_unpressed.atlas);
        u_pose_set_size(&self.opt_quad_ref->pose, m_2(u_atlas_size(*self.opt_atlas_ref, 0)));
    }
    return self;
}

O_INLINE
struct u_btn u_btn_new_rect(struct r_rect *rect_ref, struct u_atlas *opt_atlas_ref, int atlas_unpressed, int atlas_pressed)
{
    struct u_btn self = {0};
    self.opt_rect_ref = rect_ref;
    self.opt_atlas_ref = opt_atlas_ref;
    self.state_unpressed = self.state_pressed = u_btn_state_new();
    self.state_unpressed.atlas = atlas_unpressed;
    self.state_pressed.atlas = atlas_pressed;
    if(self.opt_rect_ref && self.opt_atlas_ref) {
        self.opt_rect_ref->uv_rect = u_atlas_rect(*self.opt_atlas_ref, self.state_unpressed.atlas);
        self.opt_rect_ref->rect.zw = u_atlas_size(*self.opt_atlas_ref, 0);
    }
    return self;
}

O_INLINE
struct u_btn u_btn_new_single(oobj parent, struct r_quad *quad_ref) {
    struct u_atlas *atlas = o_new(parent, *atlas, 1);
    vec2 full_size = u_pose_aa_get_wh(quad_ref->uv);
    *atlas = u_atlas_new_fill(parent, vec2_(full_size.x/2, full_size.y), 2, 1, u_atlas_START_LT, true);
    return u_btn_new(quad_ref, atlas, 0, 1);
}

O_INLINE
struct u_btn u_btn_new_color(oobj parent, struct r_quad *quad_ref, vec4 unpressed, vec4 pressed) {
    struct u_btn self = u_btn_new(quad_ref, NULL, 0, 0);
    self.state_unpressed.s = unpressed;
    self.state_pressed.s = pressed;
    return self;
}

O_INLINE
bool u_btn_is_pressed(const struct u_btn *self) {
    return self->mode == 1;
}

O_INLINE
void u_btn_set(struct u_btn *self, bool pressed) {
    self->mode = pressed? 1 : 0;
    if(self->opt_quad_ref) {
        if(self->opt_atlas_ref) {
            self->opt_quad_ref->uv = u_atlas_uv(*self->opt_atlas_ref, self->states[self->mode].atlas);
        }
        self->opt_quad_ref->stuv = self->states[self->mode].stuv;
    }
    if(self->opt_rect_ref) {
        if(self->opt_atlas_ref) {
            self->opt_rect_ref->uv_rect = u_atlas_rect(*self->opt_atlas_ref, self->states[self->mode].atlas);
        }
        self->opt_rect_ref->s = self->states[self->mode].s;
        self->opt_rect_ref->t = self->states[self->mode].t;
    }
}

O_INLINE
bool u_btn_contains(struct u_btn *self, vec4 pos)
{
    bool contains = false;
    if(self->opt_quad_ref) {
        contains = u_pose_contains(self->opt_quad_ref->pose, pos);
    }
    if(self->opt_rect_ref) {
        contains = u_rect_contains(self->opt_rect_ref->rect, pos.xy);
    }
    return contains;
}

O_INLINE
bool u_btn_clicked(struct u_btn *self, struct a_pointer pointer, struct a_pointer prev)
{
    bool contains = pointer.active && u_btn_contains(self, pointer.pos);
    bool press = a_pointer_down(pointer, prev) && contains;
    bool clicked = a_pointer_up(pointer, prev) && u_btn_is_pressed(self); // was pressed before

    if(press) {
        u_btn_set(self, true);
    }
    if(!contains || clicked) {
        u_btn_set(self, false);
    }
    return clicked;
}

O_INLINE
bool u_btn_pressed(struct u_btn *self, struct a_pointer pointer, struct a_pointer prev)
{
    bool contains = u_btn_contains(self, pointer.pos);
    bool press = a_pointer_down(pointer, prev) && contains;
    if(press) {
        u_btn_set(self, true);
    }
    return press;
}

O_INLINE
bool u_btn_toggled(struct u_btn *self, struct a_pointer pointer, struct a_pointer prev)
{
    bool contains = u_btn_contains(self, pointer.pos);
    bool toggle = a_pointer_up(pointer, prev) && contains;
    if(toggle) {
        u_btn_set(self, !u_btn_is_pressed(self));
    }
    return toggle;
}


#endif //U_BTN_H
