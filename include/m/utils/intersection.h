#ifndef M_UTILS_FLT_INTERSECTION_H
#define M_UTILS_FLT_INTERSECTION_H

/**
 * @file utils/intersection.h
 *
 * geomatrical primitive intersections
 */


#include "m/vec/vec2.h"
#include "m/vec/vec3.h"
#include "m/vec/vec4.h"
#include "m/mat/mat3.h"

#ifndef M_INTERSECTION_EPSILON
#define M_INTERSECTION_EPSILON (float) 0.01
#endif

/** returns false if planes are parallel */
O_INLINE
bool m_intersection_plane_plane(m_vec3 *out_pos, m_vec3 *out_dir,
                                m_vec4 plane_hessian_a, m_vec4 plane_hessian_b)
{
    *out_pos = *out_dir = vec3_set(NAN);

    // parallel check
    if (vec3_dot(plane_hessian_a.xyz, plane_hessian_b.xyz) > ((float) 1 - M_INTERSECTION_EPSILON))
        return false;

    // line dir is orthogonal to both plane orthogonals (nx ny nz)
    m_vec3 dir = vec3_cross(plane_hessian_a.xyz, plane_hessian_b.xyz);
    dir = vec3_normalize(dir);

    // hessian: (nx, ny, nz, d)
    // distance point to plane = x*nx + y*ny + z*nz + 1*d

    m_mat3 A = {{0}};
    A = mat3_set_row(A, plane_hessian_a.xyz, 0);
    A = mat3_set_row(A, plane_hessian_b.xyz, 1);
    A = mat3_set_row(A, dir, 2);

    // distance to planes should be 0, so -1*d
    m_vec3 B = {{-plane_hessian_a.w, -plane_hessian_b.w, 0}};

    // solve A * X = B
    // X = A^-1 * B
    m_vec3 X = mat3_mul_vec(mat3_inv(A), B);

    *out_pos = X;
    *out_dir = dir;

    return true;
}


/** returns false if plane and line are parallel */
O_INLINE
bool m_intersection_plane_line(m_vec3 *out_pos, m_vec4 plane_hessian,
                               m_vec3 line_pos, m_vec3 line_dir)
{
    *out_pos = vec3_set(NAN);

    float plane_line_dot = vec3_dot(plane_hessian.xyz, line_dir);

    // parallel check
    if (o_abs(plane_line_dot) < M_INTERSECTION_EPSILON)
        return false;

    m_vec3 plane_point = vec3_scale(plane_hessian.xyz, -plane_hessian.w);
    m_vec3 w = vec3_sub_v(line_pos, plane_point);
    float si = -vec3_dot(plane_hessian.xyz, w) / plane_line_dot;

    m_vec3 si_dir = vec3_scale(line_dir, si);

    // pos = w + si_dir + plane_point
    *out_pos = vec3_add_v(vec3_add_v(w, si_dir), plane_point);
    return true;
}


/** returns t (line_pos + line_dir * t), or NAN if not intersecting */
O_INLINE
float m_intersection_triangle_line(m_vec3 v0, m_vec3 v1, m_vec3 v2,
                                   m_vec3 line_pos, m_vec3 line_dir,
                                   bool culling)
{
    m_vec3 v0v1 = vec3_sub_v(v1, v0);
    m_vec3 v0v2 = vec3_sub_v(v2, v0);
    m_vec3 pvec = vec3_cross(line_dir, v0v2);
    float det = vec3_dot(v0v1, pvec);

    if (culling) {
        if (det < M_INTERSECTION_EPSILON)
            return NAN;
    } else {
        if (o_abs(det) < M_INTERSECTION_EPSILON)
            return NAN;
    }

    float inv_det = (float) 1 / det;

    m_vec3 tvec = vec3_sub_v(line_pos, v0);
    float u = vec3_dot(tvec, pvec) * inv_det;
    if (u < 0 || u > 1)
        return NAN;

    m_vec3 qvec = vec3_cross(tvec, v0v1);
    float v = vec3_dot(line_dir, qvec) * inv_det;
    if (v < 0 || u + v > 1)
        return NAN;

    return vec3_dot(v0v2, qvec) * inv_det;
}


/** returns false if lines are parallel, searches for the nearest distance points */
O_INLINE
bool m_intersection_line_line(float *out_tau_a, float *out_tau_b,
                              m_vec3 line_a_pos, m_vec3 line_a_dir,
                              m_vec3 line_b_pos, m_vec3 line_b_dir)
{
    *out_tau_a = *out_tau_b = NAN;

    // Book: Real Time Collision Detection: section 5.1.8 Closest Point of Two Lines
    m_vec3 r = vec3_sub_v(line_a_pos, line_b_pos);

    float a = vec3_dot(line_a_dir, line_a_dir);
    float b = vec3_dot(line_a_dir, line_b_dir);
    float c = vec3_dot(line_a_dir, r);
    float e = vec3_dot(line_b_dir, line_b_dir);
    float f = vec3_dot(line_b_dir, r);

    float d = a * e - b * b;

    // parallel check
    if (o_abs(d) < M_INTERSECTION_EPSILON)
        return false;

    *out_tau_a = (b * f - c * e) / d;
    *out_tau_b = (a * f - b * c) / d;
    return true;
}


/** returns the projection point on the line by tau. searches for the nearest distance points */
O_INLINE
float m_intersection_line_point(m_vec3 line_pos, m_vec3 line_dir, m_vec3 point)
{
    return vec3_dot(vec3_sub_v(point, line_pos), line_dir)
           / vec3_dot(line_dir, line_dir);
}


//
// m_vec2 versions:
//


/** returns false if lines are parallel */
O_INLINE
bool intersection2_line_line(float *out_tau_a, float *out_tau_b,
                             m_vec2 line_a_pos, m_vec2 line_a_dir,
                             m_vec2 line_b_pos, m_vec2 line_b_dir)
{
    *out_tau_a = *out_tau_b = NAN;

    // stupid clone of the m_vec2 version above:
    m_vec2 r = vec2_sub_v(line_a_pos, line_b_pos);

    float a = vec2_dot(line_a_dir, line_a_dir);
    float b = vec2_dot(line_a_dir, line_b_dir);
    float c = vec2_dot(line_a_dir, r);
    float e = vec2_dot(line_b_dir, line_b_dir);
    float f = vec2_dot(line_b_dir, r);

    float d = a * e - b * b;

    // parallel check
    if (o_abs(d) < M_INTERSECTION_EPSILON)
        return false;

    *out_tau_a = (b * f - c * e) / d;
    *out_tau_b = (a * f - b * c) / d;
    return true;
}

/** returns the projection point on the line by tau. searches for the nearest distance points */
O_INLINE
float intersection2_line_point(m_vec2 line_pos, m_vec2 line_dir, m_vec2 point)
{
    return vec2_dot(vec2_sub_v(point, line_pos), line_dir)
           / vec2_dot(line_dir, line_dir);
}

#endif //M_UTILS_FLT_INTERSECTION_H
