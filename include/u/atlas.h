#ifndef U_ATLAS_H
#define U_ATLAS_H

/**
 * @file atlas.h
 *
 * an atlas helps in animating sprites (RObjQuad -> r_quad.uv) in setting their uv positions
 */

#include "m/flt.h"

/**
 * Wrap modes for a sprite if its out of bounds.
 * - ASSERT calls assert on the bounds and falls through REPEAT.
 * - REPEAT calls o_mod on the bounds (positive modulo)
 * - CLAMP calls o_clamp on the bounds
 */
enum u_atlas_wrap_mode {
    u_atlas_WRAP_ASSERT,
    u_atlas_WRAP_REPEAT,
    u_atlas_WRAP_CLAMP,
    u_atlas_WRAP_NUM_MODES
};

/**
 * start modes for u_atlas_new_fill (Left, Right, Bottom, Top)
 */
enum u_atlas_start {
    u_atlas_START_LB,
    u_atlas_START_RB,
    u_atlas_START_LT,
    u_atlas_START_RT,
    u_atlas_START_NUM_MODES
};

struct u_atlas {
    // array of center positions and sizes for each sprite in the spritesheet
    // xywh
    vec4 *rects;
    int num;

    // defaults to ASSERT
    enum u_atlas_wrap_mode wrap_mode;
    
    // may be deleted
    oobj parent;
};

/**
 * Creates a new atlas with an pos array allocated on obj (see u_atlas_move, u_atlas_free).
 * pos array is cleared to 0
 * @param obj object to allocate on
 * @param num_sprites number of sprites
 * @return u_atlas object with pos allocated on obj
 * @note wrap_mode defaults to u_atlas_WRAP_ASSERT
 */
O_EXTERN
struct u_atlas u_atlas_new(oobj obj, int num_sprites);

/**
 * Creates a new atlas with an pos array allocated on obj (see u_atlas_move, u_atlas_free)
 * @param obj object to allocate on
 * @param size of each sprite
 * @param num_sprites number of sprites
 * @param pos_0 center position of the first sprite, see u_atlas_pos0_* for helper functions
 * @param offset offset to the next sprite (values may be negative...)
 * @param row_major if true, use offset.x until block size is reached, then offset.y, else other way around
 * @param block if row_major: columns, else rows
 * @return u_atlas object with pos allocated on obj
 * @note wrap_mode defaults to u_atlas_WRAP_ASSERT.
 *       calls u_atlas_set_grid with the allocated pos array
 */
O_EXTERN
struct u_atlas u_atlas_new_grid(oobj obj, vec2 size, int num_sprites,
                                vec2 pos_0, vec2 offset, bool row_major, int block);

/**
 * Creates a new atlas with an pos array allocated on obj (see u_atlas_move, u_atlas_free).
 * "fill" is a special filled out grid for easier setup. bottom left is the first sprite, row_major
 * @param obj object to allocate on
 * @param size of the sprites
 * @param cols number of closed up together sprite columns
 * @param rows number of closed up together sprite rows
 * @param start for example: u_atlas_START_LT for left top
 * @param row_major if true, first traverses through columns (the full row)
 * @return u_atlas object with pos allocated on obj
 * @note wrap_mode defaults to u_atlas_WRAP_ASSERT.
 *       calls u_atlas_new_grid
 */
O_EXTERN
struct u_atlas u_atlas_new_fill(oobj obj, vec2 size, int cols, int rows, enum u_atlas_start start, bool row_major);

/**
 * Creates a new atlas with an pos array allocated on obj (see u_atlas_move, u_atlas_free).
 * "fill" is a special filled out grid for easier setup. bottom left is the first sprite, row_major
 * @param obj object to allocate on
 * @param tex RTex object to get the filled sprite size from (vec2_div_v(RTex_size(tex), vec2_(cols, rows))
 * @param cols number of closed up together sprite columns
 * @param rows number of closed up together sprite rows
 * @param start for example: u_atlas_START_LT for left top
 * @param row_major if true, first traverses through columns (the full row)
 * @return u_atlas object with pos allocated on obj
 * @note wrap_mode defaults to u_atlas_WRAP_ASSERT.
 *       calls u_atlas_new_grid
 */
O_EXTERN
struct u_atlas u_atlas_new_fill_tex(oobj obj, oobj tex, int cols, int rows, enum u_atlas_start start, bool row_major);


/**
 * moves the atlas pos array into another object
 * @param self to move
 * @param into the new object to move the list into
 * @note just calls o_move_res(obj, into, self.pos)
 */
O_EXTERN
void u_atlas_move(struct u_atlas *self, oobj into);

/**
 * frees the atlas pos array
 * @param self to free
 * @note just calls o_free(obj, self.pos)
 */
O_EXTERN
void u_atlas_free(struct u_atlas *self);


//
// struct functions
//

/**
 * @param self the sprite atlas
 * @param size for each sprite
 */
O_EXTERN
void u_atlas_set_size(struct u_atlas self, vec2 size);

/**
 * @param self the sprite atlas
 * @param pos_0 center position of the first sprite, see u_atlas_pos0_* for helper functions
 * @param offset offset to the next sprite (values may be negative...)
 * @param row_major if true, use offset.x until block size is reached, then offset.y, else other way around
 * @param block if row_major: columns, else rows
 */
O_EXTERN
void u_atlas_set_grid(struct u_atlas self, vec2 pos_0, vec2 offset, bool row_major, int block);

/**
 * @param self the sprite atlas
 * @param sprite the sprite index
 * @return sprite wrapped with the wrap mode if out of bounds
 */
O_INLINE
int u_atlas_wrap_sprite(struct u_atlas self, int sprite)
{
    switch (self.wrap_mode) {
        default:
        case u_atlas_WRAP_ASSERT:
            assert(sprite>=0 && sprite<self.num && "invalid sprite");
            /* fall-through */
        case u_atlas_WRAP_REPEAT:
            sprite = o_mod(sprite, self.num);
            break;
        case u_atlas_WRAP_CLAMP:
            sprite = o_clamp(sprite, 0, self.num-1);
            break;
    }
    return sprite;
}

/**
 * @param self the sprite atlas
 * @param sprite sprite which may be wrapped if out of bounds
 * @return center position and size as xywh
 */
O_INLINE
vec4 u_atlas_rect(struct u_atlas self, int sprite)
{
    return self.rects[u_atlas_wrap_sprite(self, sprite)];
}

/**
 * @param self the sprite atlas
 * @param sprite sprite which may be wrapped if out of bounds
 * @return center position of the sprite
 */
O_INLINE
vec2 u_atlas_pos(struct u_atlas self, int sprite)
{
    return u_atlas_rect(self, sprite).xy;
}

/**
 * @param self the sprite atlas
 * @param sprite sprite which may be wrapped if out of bounds
 * @return size of the sprite
 */
O_INLINE
vec2 u_atlas_size(struct u_atlas self, int sprite)
{
    return u_atlas_rect(self, sprite).zw;
}

/**
 * @param self the sprite atlas
 * @param sprite sprite which may be wrapped if out of bounds
 * @return a full uv matrix for an axis aligned sprite
 */
O_INLINE
mat4 u_atlas_uv(struct u_atlas self, int sprite)
{
    vec4 item = u_atlas_rect(self, sprite);
    // as u_pose matrices...
    return mat4_new(
            item.v2 / 2, 0, 0, 0,
            0, item.v3 / 2, 0, 0,
            0, 0, 1, 0,
            item.x, item.y, 0, 1
    );
}


/**
 * @param self the sprite atlas
 * @param in_out_uv reference to an existing uv matrix, to just update the new center position
 * @param sprite sprite which may be wrapped if out of bounds
 */
O_INLINE
void u_atlas_uv_update_pos(struct u_atlas self, mat4 *in_out_uv, int sprite)
{
    vec2 pos = u_atlas_pos(self, sprite);
    // as u_pose matrices...
    in_out_uv->m30 = pos.x;
    in_out_uv->m31 = pos.y;
}

/**
 * Shifts all center positions with offset
 * @param self the sprite atlas to shift
 * @param offset to be added on each center pos
 */
O_INLINE
void u_atlas_offset(struct u_atlas self, vec2 offset)
{
    for(int i=0; i<self.num; i++) {
        self.rects[i].xy = vec2_add_v(self.rects[i].xy, offset);
    }
}


//
// some helper to create pos_0
//

/**
 * @param tex_size size of the tex for the atlas
 * @param offset_l from tex's left border to the left border of the first sprite
 * @param offset_b from tex's bottom border to the bottom border of the first sprite
 * @param w sprite width
 * @param h sprite height
 * @return center position for pos0
 */
O_INLINE
vec2 u_atlas_pos0_lb(vec2 tex_size, float offset_l, float offset_b, float w, float h)
{
    return vec2_(+offset_l +w/2.0f -tex_size.x/2.0f,
                 +offset_b +h/2.0f -tex_size.y/2.0f);
}

/**
 * @param tex_size size of the tex for the atlas
 * @param offset_l from tex's left border to the left border of the first sprite
 * @param offset_t from tex's top border to the top border of the first sprite
 * @param w sprite width
 * @param h sprite height
 * @return center position for pos0
 */
O_INLINE
vec2 u_atlas_pos0_lt(vec2 tex_size, float offset_l, float offset_t, float w, float h)
{
    return vec2_(+offset_l +w/2.0f -tex_size.x/2.0f,
                 -offset_t -h/2.0f +tex_size.y/2.0f);
}

/**
 * @param tex_size size of the tex for the atlas
 * @param offset_r from tex's right border to the right border of the first sprite
 * @param offset_b from tex's bottom border to the bottom border of the first sprite
 * @param w sprite width
 * @param h sprite height
 * @return center position for pos0
 */
O_INLINE
vec2 u_atlas_pos0_rb(vec2 tex_size, float offset_r, float offset_b, float w, float h)
{
    return vec2_(-offset_r -w/2.0f +tex_size.x/2.0f,
                 +offset_b +h/2.0f -tex_size.y/2.0f);
}

/**
 * @param tex_size size of the tex for the atlas
 * @param offset_r from tex's right border to the right border of the first sprite
 * @param offset_t from tex's top border to the top border of the first sprite
 * @param w sprite width
 * @param h sprite height
 * @return center position for pos0
 */
O_INLINE
vec2 u_atlas_pos0_rt(vec2 tex_size, float offset_r, float offset_t, float w, float h)
{
    return vec2_(-offset_r -w/2.0f +tex_size.x/2.0f,
                 -offset_t -h/2.0f +tex_size.y/2.0f);
}

/**
 * @param uv Unchanged axis aligned uv that covers the full tex (so has the tex size internally)
 * @param offset_l from tex's left border to the left border of the first sprite
 * @param offset_b from tex's bottom border to the bottom border of the first sprite
 * @param w sprite width
 * @param h sprite height
 * @return center position for pos0
 */
O_INLINE
vec2 u_atlas_pos0_lb_uv(mat4 uv, float offset_l, float offset_b, float w, float h)
{
    return u_atlas_pos0_lb(vec2_(uv.m00*2.0f, uv.m11*2.0f), offset_l, offset_b, w, h);
}

/**
 * @param uv Unchanged axis aligned uv that covers the full tex (so has the tex size internally)
 * @param offset_l from tex's left border to the left border of the first sprite
 * @param offset_t from tex's top border to the top border of the first sprite
 * @param w sprite width
 * @param h sprite height
 * @return center position for pos0
 */
O_INLINE
vec2 u_atlas_pos0_lt_uv(mat4 uv, float offset_l, float offset_t, float w, float h)
{
    return u_atlas_pos0_lt(vec2_(uv.m00*2.0f, uv.m11*2.0f), offset_l, offset_t, w, h);
}

/**
 * @param uv Unchanged axis aligned uv that covers the full tex (so has the tex size internally)
 * @param offset_r from tex's right border to the right border of the first sprite
 * @param offset_b from tex's bottom border to the bottom border of the first sprite
 * @param w sprite width
 * @param h sprite height
 * @return center position for pos0
 */
O_INLINE
vec2 u_atlas_pos0_rb_uv(mat4 uv, float offset_r, float offset_b, float w, float h)
{
    return u_atlas_pos0_rb(vec2_(uv.m00*2.0f, uv.m11*2.0f), offset_r, offset_b, w, h);
}

/**
 * @param uv Unchanged axis aligned uv that covers the full tex (so has the tex size internally)
 * @param offset_r from tex's right border to the right border of the first sprite
 * @param offset_t from tex's top border to the top border of the first sprite
 * @param w sprite width
 * @param h sprite height
 * @return center position for pos0
 */
O_INLINE
vec2 u_atlas_pos0_rt_uv(mat4 uv, float offset_r, float offset_t, float w, float h)
{
    return u_atlas_pos0_rt(vec2_(uv.m00*2.0f, uv.m11*2.0f), offset_r, offset_t, w, h);
}

#endif //U_ATLAS_H
