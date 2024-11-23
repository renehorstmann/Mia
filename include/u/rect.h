#ifndef U_RECT_H
#define U_RECT_H

/**
 * @file rect.h
 *
 * Functions dealing with a vec4 rect (xywh) in 2D space.
 * 
 * an rect is defined as vec4:
 * center_x
 * center_y
 * width
 * height
 */


#include "o/common.h"
#include "m/types/flt.h"
#include "m/types/int.h"
#include "m/sca/flt.h"

/**
 * @param x center
 * @param y center
 * @param w width
 * @param h height
 * @return centered rect
 */
O_INLINE
vec4 u_rect_new(float x, float y, float w, float h)
{
    return vec4_(x, y, w, h);
}

/**
 * @param l left
 * @param b bottom
 * @param w width
 * @param h height
 * @return left bottom relative rect
 */
O_INLINE
vec4 u_rect_new_lb(float l, float b, float w, float h)
{
    return u_rect_new(l + w / 2, b + h / 2, w, h);
}

/**
 * @param l left
 * @param t top
 * @param w width
 * @param h height
 * @return left top relative rect
 */
O_INLINE
vec4 u_rect_new_lt(float l, float t, float w, float h)
{
    return u_rect_new(l + w / 2, t - h / 2, w, h);
}

/**
 * @param r right
 * @param b bottom
 * @param w width
 * @param h height
 * @return right bottom relative rect
 */
O_INLINE
vec4 u_rect_new_rb(float r, float b, float w, float h)
{
    return u_rect_new(r - w / 2, b + h / 2, w, h);
}

/**
 * @param r left
 * @param t top
 * @param w width
 * @param h height
 * @return right top relative rect
 */
O_INLINE
vec4 u_rect_new_rt(float r, float t, float w, float h)
{
    return u_rect_new(r - w / 2, t - h / 2, w, h);
}

/**
 * @param l left
 * @param r right
 * @param b bottom
 * @param t top
 * @return top left relative rect
 */
O_INLINE
vec4 u_rect_new_bounds(float l, float r, float b, float t)
{
    return u_rect_new_lb(l, b, r - l, t - b);
}

/**
 * @return a hidden rect with valid size
 */
O_INLINE
vec4 u_rect_new_hidden(void)
{
    return u_rect_new(m_MAX, m_MAX, 1, 1);
}

/**
 * @param r rect
 * @return the left border
 */
O_INLINE
float u_rect_get_left(vec4 r)
{
    return r.x - r.v2 / 2;
}

/**
 * @param r rect
 * @return the right border
 */
O_INLINE
float u_rect_get_right(vec4 r)
{
    return r.x + r.v2 / 2;
}

/**
 * @param r rect
 * @return the top border
 */
O_INLINE
float u_rect_get_top(vec4 r)
{
    return r.y + r.v3 / 2;
}

/**
 * @param r rect
 * @return the bottom border
 */
O_INLINE
float u_rect_get_bottom(vec4 r)
{
    return r.y - r.v3 / 2;
}

/**
 * Sets the position to MAX to hide the rect (only if wh are not MAX...)
 * @param r rect reference to set
 */
O_INLINE
void u_rect_set_hidden(vec4 *r)
{
    r->xy = vec2_(m_MAX, m_MAX);
}


/**
 * @param r rect reference to set
 * @param l left border
 */
O_INLINE
void u_rect_set_left(vec4 *r, float l)
{
    r->x = l + r->v2 / 2;
}

/**
 * @param r rect reference to set
 * @param right right border
 */
O_INLINE
void u_rect_set_right(vec4 *r, float right)
{
    r->x = right - r->v2 / 2;
}

/**
 * @param r rect reference to set
 * @param t top border
 */
O_INLINE
void u_rect_set_top(vec4 *r, float t)
{
    r->y = t - r->v3 / 2;
}

/**
 * @param r rect reference to set
 * @param b bottom border
 */
O_INLINE
void u_rect_set_bottom(vec4 *r, float b)
{
    r->y = b + r->v3 / 2;
}

/**
 * @param r rect reference to set
 * @param l left
 * @param b bottom
 */
O_INLINE
void u_rect_set_lb(vec4 *r, float l, float b)
{
    u_rect_set_left(r, l);
    u_rect_set_bottom(r, b);
}

/**
 * @param r rect reference to set
 * @param l left
 * @param t top
 */
O_INLINE
void u_rect_set_lt(vec4 *r, float l, float t)
{
    u_rect_set_left(r, l);
    u_rect_set_top(r, t);
}

/**
 * @param r rect reference to set
 * @param right right
 * @param b bottom
 */
O_INLINE
void u_rect_set_rb(vec4 *r, float right, float b)
{
    u_rect_set_right(r, right);
    u_rect_set_bottom(r, b);
}

/**
 * @param r rect reference to set
 * @param right right
 * @param t top
 */
O_INLINE
void u_rect_set_rt(vec4 *r, float right, float t)
{
    u_rect_set_right(r, right);
    u_rect_set_top(r, t);
}

/**
 * @param r rect
 * @return left bottom width height
 */
O_INLINE
vec4 u_rect_lbwh(vec4 r)
{
    return vec4_(u_rect_get_left(r), u_rect_get_bottom(r), r.v2, r.v3);
}

/**
 * @param r rect
 * @return 4x4 pose matrix as with u/pose.h
 */
O_INLINE
mat4 u_rect_as_pose(vec4 r)
{
    return (mat4) {
            {
                    r.v2 / 2.0f, 0, 0, 0,
                    0, r.v3 / 2.0f, 0, 0,
                    0, 0, 1, 0,
                    r.x, r.y, 0, 1
            }
    };
}

/**
 * @param r rect
 * @param pos xy position to check if its in the rect rect
 * @return true if pos is in p
 */
O_INLINE
bool u_rect_contains(vec4 r, vec2 pos)
{
    float l = u_rect_get_left(r);
    float right = u_rect_get_right(r);
    float t = u_rect_get_top(r);
    float b = u_rect_get_bottom(r);

    return pos.x >= l && pos.x <= right && pos.y <= t && pos.y >= b;
}


#endif //U_RECT_H
