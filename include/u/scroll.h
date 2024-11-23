#ifndef U_SCROLL_H
#define U_SCROLL_H

/**
 * @file scroll.h
 *
 * RCam scrolling with a_pointer.
 */

#include "rect.h"
#include "a/pointer.h"

/**
 * Different scroll modes to update the RCam
 */
enum u_scroll_mode {
    u_scroll_OFF,
    u_scroll_X,
    u_scroll_Y,
    u_scroll_XY
};

/**
 * Align modes for x and y
 */
enum u_scroll_align {
    u_scroll_BEGIN,
    u_scroll_CENTER,
    u_scroll_END,
    u_scroll_NUM_ALIGN_MODES
};


struct u_scroll {
    enum u_scroll_mode mode;

    // alignments, inits to CENTER
    // added as offset from raw_pos -> pos
    enum u_scroll_align align_x;
    enum u_scroll_align align_y;

    // resulting cam position
    vec2 pos;

    // opt RCam, pos will also be set to RCam_pos_set, if not NULL
    oobj cam;

    // if true (default), pos will be rounded to an integer
    bool on_units;

    // rect's (u_rect, etc.), init as inf
    // view_rect should be set to the applied view's rect as with "RCam_rect(AView_cam(view), 0, 0)"
    // cam_limits_rect is the rect in which the camera is able to scroll, so should be bigger view_rect to allow movement
    vec4 view_rect;
    vec4 cam_limits_rect;

    // de acceleration, init as vec2_(1000.0)
    vec2 deacc;

    // if ||speed||_1 is greater then that, pointer will be disables, inits to 20
    float handle_speed;

    // scrolling stuff
    bool scrolling;
    vec2 raw_pos;
    vec2 speed;
    vec2 start_pointer, start_pos, prev_pos;
    struct a_pointer prev;
};

/**
 * Creates a new u_scroll member
 * @param mode either none, X, Y or both
 * @param opt_cam RCam object reference or NULL
 * @return u_scroll member
 */
O_EXTERN
struct u_scroll u_scroll_new(enum u_scroll_mode mode, oobj opt_cam);


/**
 * Updates the scrolling, uses a_pointer_p to get a pointer
 * @param self u_scroll member
 * @param dt delta time to apply the scroll speed
 * @return current generated camera pos; self->pos
 */
O_EXTERN
vec2 u_scroll_update(struct u_scroll *self, float dt);


/**
 * Sets pos and all internal associated positions (raw_pos, ...)
 */
O_STATIC
vec2 u_scroll_pos_set(struct u_scroll *self, vec2 pos)
{
    return self->pos = self->prev_pos = self->raw_pos = pos;
}

#endif //U_SCROLL_H
