#ifndef U_POSE_H
#define U_POSE_H

/**
 * @file pose.h
 *
 * Functions dealing with a mat4 (3D) pose in 2D space.
 * 
 *
 * Pose matrix (printed in column major order...):
 * Xx Xy Xz 0
 * Yx Yy Yz 0
 * Zx Zy Zz 0
 * Tx Ty Tz 1
 *
 * Pose in form of eye will be at position 0, 0, with a radius of 1, 1
 *
 * u_pose_aa_* are axis aligned (angle=0)
 * rx 0  0  0
 * 0  rh 0  0
 * 0  0  1  0
 * x  y  0  1
 *
 * an rect is defined as vec4:
 * center_x
 * center_y
 * width
 * height
 *
 * some functions have *_aa_* in their names
 * aa stand for axis aligned / not rotated
 * these functions are usually faster
 */


#include "o/common.h"
#include "m/types/flt.h"
#include "m/mat/mat4.h"

/**
 * @param x center
 * @param y center
 * @param w width
 * @param h height
 * @return centered axis aligned pose
 */
O_INLINE
mat4 u_pose_new(float x, float y, float w, float h)
{
    // mat4 has column major order
    return mat4_new(
            w / 2, 0, 0, 0,
            0, h / 2, 0, 0,
            0, 0, 1, 0,
            x, y, 0, 1
    );
}

/**
 * @param rect as (x_center, y_center, width, height)
 * @return centered axis aligned pose
 */
O_INLINE
mat4 u_pose_new_rect(vec4 rect)
{
    return u_pose_new(rect.x, rect.y, rect.v2, rect.v3);
}

/**
 * @param x center
 * @param y center
 * @param w width
 * @param h height
 * @param angle_rad
 * @return centered rotated pose
 */
O_INLINE
mat4 u_pose_new_angle(float x, float y, float w, float h, float angle_rad)
{
    // mat4 has column major order
    return mat4_new(
            m_cos(angle_rad) * w / 2, m_sin(angle_rad) * w / 2, 0, 0,
            -m_sin(angle_rad) * h / 2, m_cos(angle_rad) * h / 2, 0, 0,
            0, 0, 1, 0,
            x, y, 0, 1
    );
}

/**
 * @param l left
 * @param b bottom
 * @param w width
 * @param h height
 * @return left bottom relative axis aligned pose
 */
O_INLINE
mat4 u_pose_new_lb(float l, float b, float w, float h)
{
    return u_pose_new(l + w / 2, b + h / 2, w, h);
}

/**
 * @param l left
 * @param t top
 * @param w width
 * @param h height
 * @return left top relative axis aligned pose
 */
O_INLINE
mat4 u_pose_new_lt(float l, float t, float w, float h)
{
    return u_pose_new(l + w / 2, t - h / 2, w, h);
}

/**
 * @param r right
 * @param b bottom
 * @param w width
 * @param h height
 * @return right bottom relative axis aligned pose
 */
O_INLINE
mat4 u_pose_new_rb(float r, float b, float w, float h)
{
    return u_pose_new(r - w / 2, b + h / 2, w, h);
}

/**
 * @param r left
 * @param t top
 * @param w width
 * @param h height
 * @return right top relative axis aligned pose
 */
O_INLINE
mat4 u_pose_new_rt(float r, float t, float w, float h)
{
    return u_pose_new(r - w / 2, t - h / 2, w, h);
}

/**
 * @param l left
 * @param r right
 * @param b bottom
 * @param t top
 * @return top left relative axis aligned pose
 */
O_INLINE
mat4 u_pose_new_bounds(float l, float r, float b, float t)
{
    return u_pose_new_lb(l, b, r - l, t - b);
}

/**
 * @return a hidden pose with valid size
 */
O_INLINE
mat4 u_pose_new_hidden(void)
{
    return u_pose_new(m_MAX, m_MAX, 1, 1);
}

/**
 * @param p pose
 * @return the x center position
 */
O_INLINE
float u_pose_get_x(mat4 p)
{
    return p.m30;
}

/**
 * @param p pose
 * @return the y center position
 */
O_INLINE
float u_pose_get_y(mat4 p)
{
    return p.m31;
}

/**
 * @param p pose
 * @return the xy center position
 */
O_INLINE
vec2 u_pose_get_xy(mat4 p)
{
    return vec2_(p.m30, p.m31);
}

/**
 * @param p pose
 * @return the angle of the rotation in [rad]
 */
O_INLINE
float u_pose_get_angle(mat4 p)
{
    return m_atan2(p.m01, p.m00);
}

/**
 * @param p pose
 * @return the width (always positive)
 */
O_INLINE
float u_pose_get_w(mat4 p)
{
    return 2 * m_sqrt(m_pow(p.m00, 2) + m_pow(p.m01, 2));
}

/**
 * @param p pose
 * @return the height (always positive)
 */
O_INLINE
float u_pose_get_h(mat4 p)
{
    return 2 * m_sqrt(m_pow(p.m10, 2) + m_pow(p.m11, 2));
}

/**
 * @param p pose
 * @return the size (always positive)
 */
O_INLINE
vec2 u_pose_get_wh(mat4 p)
{
    return vec2_(
            u_pose_get_w(p),
            u_pose_get_h(p)
    );
}

/**
 * @param p axis aligned pose
 * @return the width (may be negative)
 * @note only valid for axis aligned poses
 */
O_INLINE
float u_pose_aa_get_w(mat4 p)
{
    return p.m00*2;
}

/**
 * @param p axis aligned pose
 * @return the height (may be negative)
 * @note only valid for axis aligned poses
 */
O_INLINE
float u_pose_aa_get_h(mat4 p)
{
    return p.m11*2;
}

/**
 * @param p axis aligned pose
 * @return the size (may be negative)
 * @note only valid for axis aligned poses
 */
O_INLINE
vec2 u_pose_aa_get_wh(mat4 p)
{
    return vec2_(p.m00*2, p.m11*2);
}

/**
 * @param p axis aligned pose
 * @return a rect as (x_center, y_center, width, height)
 * @note only valid for axis aligned poses
 */
O_INLINE
vec4 u_pose_aa_get_rect(mat4 p)
{
    return vec4_(p.m30, p.m31, p.m00*2, p.m11*2);
}

/**
 * @param p axis aligned pose
 * @return the left border
 * @note only valid for axis aligned poses
 */
O_INLINE
float u_pose_aa_get_left(mat4 p)
{
    return p.m30 - p.m00;
}

/**
 * @param p axis aligned pose
 * @return the right border
 * @note only valid for axis aligned poses
 */
O_INLINE
float u_pose_aa_get_right(mat4 p)
{
    return p.m30 + p.m00;
}

/**
 * @param p axis aligned pose
 * @return the top border
 * @note only valid for axis aligned poses
 */
O_INLINE
float u_pose_aa_get_top(mat4 p)
{
    return p.m31 + p.m11;
}

/**
 * @param p axis aligned pose
 * @return the bottom border
 * @note only valid for axis aligned poses
 */
O_INLINE
float u_pose_aa_get_bottom(mat4 p)
{
    return p.m31 - p.m11;
}

/**
 * @param p pose reference to set
 * @param x center
 */
O_INLINE
void u_pose_set_x(mat4 *p, float x)
{
    p->m30 = x;
}

/**
 * @param p pose reference to set
 * @param y center
 */
O_INLINE
void u_pose_set_y(mat4 *p, float y)
{
    p->m31 = y;
}

/**
 * @param p pose reference to set
 * @param x center
 * @param y center
 */
O_INLINE
void u_pose_set_xy(mat4 *p, float x, float y)
{
    u_pose_set_x(p, x);
    u_pose_set_y(p, y);
}

/**
 * Sets the position to MAX to hide the rect (only if wh are not MAX...)
 * @param p pose reference to set
 */
O_INLINE
void u_pose_set_hidden(mat4 *p)
{
    u_pose_set_xy(p, m_MAX, m_MAX);
}

/**
 * @param p pose reference to set
 * @param w width
 * @param h height
 * @param angle_rad rotation in [rad]
 */
O_INLINE
void u_pose_set_size_angle(mat4 *p, float w, float h, float angle_rad)
{
    p->m00 = m_cos(angle_rad) * w / 2;
    p->m01 = m_sin(angle_rad) * w / 2;
    p->m10 = -m_sin(angle_rad) * h / 2;
    p->m11 = m_cos(angle_rad) * h / 2;
}

/**
 * @param p pose reference to set
 * @param w width
 * @note width must have been valid before (!=0)
 */
O_INLINE
void u_pose_set_w(mat4 *p, float w)
{
    float f = w / u_pose_get_w(*p);
    p->m00 *= f;
    p->m01 *= f;
}

/**
 * @param p pose reference to set
 * @param h height
 * @note height must have been valid before (!=0)
 */
O_INLINE
void u_pose_set_h(mat4 *p, float h)
{
    float f = h / u_pose_get_h(*p);
    p->m10 *= f;
    p->m11 *= f;
}


/**
 * @param p pose reference to set
 * @param w width
 * @param h height
 * @note size must have been valid before (!=0)
 */
O_INLINE
void u_pose_set_size(mat4 *p, float w, float h)
{
    u_pose_set_w(p, w);
    u_pose_set_h(p, h);
}

/**
 * @param p pose reference to set
 * @param angle_rad rotation in [rad]
 */
O_INLINE
void u_pose_set_angle(mat4 *p, float angle_rad)
{
    u_pose_set_size_angle(p, u_pose_get_w(*p), u_pose_get_h(*p), angle_rad);
}

/**
 * @param p pose reference to set
 * @param x center
 * @param y center
 * @param w width
 * @param h height
 * @param angle_rad rotation in [rad]
 */
O_INLINE
void u_pose_set(mat4 *p, float x, float y, float w, float h, float angle_rad)
{
    u_pose_set_xy(p, x, y);
    u_pose_set_size_angle(p, w, h, angle_rad);
}

/**
 * @param p pose reference to set
 * @param x offset
 */
O_INLINE
void u_pose_shift_x(mat4 *p, float x)
{
    p->m30 += x;
}

/**
 * @param p pose reference to set
 * @param y offset
 */
O_INLINE
void u_pose_shift_y(mat4 *p, float y)
{
    p->m31 += y;
}

/**
 * @param p pose reference to set
 * @param x offset
 * @param y offset
 */
O_INLINE
void u_pose_shift_xy(mat4 *p, float x, float y)
{
    u_pose_shift_x(p, x);
    u_pose_shift_y(p, y);
}

/**
 * @param p pose reference to set
 * @param angle_rad rotation offset in [rad]
 */
O_INLINE
void u_pose_shift_angle(mat4 *p, float angle_rad)
{
    u_pose_set_angle(p, u_pose_get_angle(*p) + angle_rad);
}


/**
 * @param p pose reference to set
 * @param x offset
 * @param y offset
 * @param angle_rad rotation offset in [rad]
 */
O_INLINE
void u_pose_shift(mat4 *p, float x, float y, float angle_rad)
{
    u_pose_shift_xy(p, x, y);
    u_pose_shift_angle(p, angle_rad);
}

/**
 * @param p axis aligned pose reference to set
 * @param l left border
 * @note only valid for axis aligned poses
 */
O_INLINE
void u_pose_aa_set_left(mat4 *p, float l)
{
    p->m30 = l + p->m00;
}

/**
 * @param p axis aligned pose reference to set
 * @param r right border
 * @note only valid for axis aligned poses
 */
O_INLINE
void u_pose_aa_set_right(mat4 *p, float r)
{
    p->m30 = r - p->m00;
}

/**
 * @param p axis aligned pose reference to set
 * @param t top border
 * @note only valid for axis aligned poses
 */
O_INLINE
void u_pose_aa_set_top(mat4 *p, float t)
{
    p->m31 = t - p->m11;
}

/**
 * @param p axis aligned pose reference to set
 * @param b bottom border
 * @note only valid for axis aligned poses
 */
O_INLINE
void u_pose_aa_set_bottom(mat4 *p, float b)
{
    p->m31 = b + p->m11;
}

/**
 * @param p axis aligned pose reference to set
 * @param w width
 * @note only valid for axis aligned poses
 */
O_INLINE
void u_pose_aa_set_w(mat4 *p, float w)
{
    p->m00 = w / 2;
}

/**
 * @param p axis aligned pose reference to set
 * @param h height
 * @note only valid for axis aligned poses
 */
O_INLINE
void u_pose_aa_set_h(mat4 *p, float h)
{
    p->m11 = h / 2;
}

/**
 * @param p axis aligned pose reference to set
 * @param w width
 * @note only valid for axis aligned poses
 */
O_INLINE
void u_pose_aa_set_size(mat4 *p, float w, float h)
{
    u_pose_aa_set_w(p, w);
    u_pose_aa_set_h(p, h);
}


/**
 * @param p axis aligned pose reference to set
 * @param x center x
 * @param r center y
 * @param w width
 * @param h height
 * @note only valid for axis aligned poses
 */
O_INLINE
void u_pose_aa_set(mat4 *p, float x, float y, float w, float h)
{
    u_pose_set_xy(p, x, y);
    u_pose_aa_set_size(p, w, h);
}

/**
 * @param p axis aligned pose reference to set
 * @param l left
 * @param b bottom
 * @note only valid for axis aligned poses
 */
O_INLINE
void u_pose_aa_set_lb(mat4 *p, float l, float b)
{
    u_pose_aa_set_left(p, l);
    u_pose_aa_set_bottom(p, b);
}

/**
 * @param p axis aligned pose reference to set
 * @param l left
 * @param t top
 * @note only valid for axis aligned poses
 */
O_INLINE
void u_pose_aa_set_lt(mat4 *p, float l, float t)
{
    u_pose_aa_set_left(p, l);
    u_pose_aa_set_top(p, t);
}

/**
 * @param p axis aligned pose reference to set
 * @param r right
 * @param b bottom
 * @note only valid for axis aligned poses
 */
O_INLINE
void u_pose_aa_set_rb(mat4 *p, float r, float b)
{
    u_pose_aa_set_right(p, r);
    u_pose_aa_set_bottom(p, b);
}

/**
 * @param p axis aligned pose reference to set
 * @param r right
 * @param t top
 * @note only valid for axis aligned poses
 */
O_INLINE
void u_pose_aa_set_rt(mat4 *p, float r, float t)
{
    u_pose_aa_set_right(p, r);
    u_pose_aa_set_top(p, t);
}

/**
 * @param p axis aligned pose reference to set
 * @param l left
 * @param b bottom
 * @param w width
 * @param h height
 * @note only valid for axis aligned poses
 */
O_INLINE
void u_pose_aa_set_lbwh(mat4 *p, float l, float b, float w, float h)
{
    u_pose_aa_set(p, l + w / 2, b + h / 2, w, h);
}

/**
 * @param p axis aligned pose reference to set
 * @param l left
 * @param t top
 * @param w width
 * @param h height
 * @note only valid for axis aligned poses
 */
O_INLINE
void u_pose_aa_set_ltwh(mat4 *p, float l, float t, float w, float h)
{
    u_pose_aa_set(p, l + w / 2, t - h / 2, w, h);
}

/**
 * @param p axis aligned pose reference to set
 * @param r right
 * @param b bottom
 * @param w width
 * @param h height
 * @note only valid for axis aligned poses
 */
O_INLINE
void u_pose_aa_set_rbwh(mat4 *p, float r, float b, float w, float h)
{
    u_pose_aa_set(p, r - w / 2, b + h / 2, w, h);
}

/**
 * @param p axis aligned pose reference to set
 * @param r left
 * @param t top
 * @param w width
 * @param h height
 * @note only valid for axis aligned poses
 */
O_INLINE
void u_pose_aa_set_rtwh(mat4 *p, float r, float t, float w, float h)
{
    u_pose_aa_set(p, r - w / 2, t - h / 2, w, h);
}

/**
 * @param p axis aligned pose reference to set
 * @param l left border
 * @param r right border
 * @param t top border
 * @param b bottom border
 * @note only valid for axis aligned poses
 */
O_INLINE
void u_pose_aa_set_bounds(mat4 *p, float l, float r, float t, float b)
{
    u_pose_aa_set(p, l, t, r - l, t - b);
}

/**
 * @param pos already transformed with pos = mat4_mul_vec(mat4_inv(pose), pos);
 * @return true if transformed pos is within the pose boundaries
 * @sa u_pose_contains
 */
O_INLINE
bool u_pose_transformed_contains(vec4 pos)
{
    return pos.x >= -1.0f && pos.x <= 1.0f && pos.y >= -1.0f && pos.y <= 1.0f;
}


/**
 * @param p pose
 * @param pos position to check if its in the pose rect
 * @return true if pos is in p
 */
O_INLINE
bool u_pose_contains(mat4 p, vec4 pos)
{
    pos = mat4_mul_vec(mat4_inv(p), pos);
    return u_pose_transformed_contains(pos);
}

/**
 * @param p axis aligned pose
 * @param pos position to check if its in the pose rect
 * @return true if pos is in p
 * @note only valid for axis aligned poses
 */
O_INLINE
bool u_pose_aa_contains(mat4 p, vec2 pos)
{
    float l = u_pose_aa_get_left(p);
    float r = u_pose_aa_get_right(p);
    float t = u_pose_aa_get_top(p);
    float b = u_pose_aa_get_bottom(p);

    return pos.x >= l && pos.x <= r && pos.y <= t && pos.y >= b;
}

/**
 * @param p axis aligned pose
 * @param a start position of the line
 * @param b end position of the line
 * @return true if line ab intersects with p
 * @note only valid for axis aligned poses
 */
O_EXTERN
bool u_pose_aa_intersects_line(mat4 p, vec2 a, vec2 b);


#endif //U_POSE_H
