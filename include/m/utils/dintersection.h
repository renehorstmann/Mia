#ifndef M_UTILS_DBL_DINTERSECTION_H
#define M_UTILS_DBL_DINTERSECTION_H

/**
 * @file utils/dintersection.h
 *
 * geomatrical primitive intersections
 */


#include "m/sca/dbl.h"
#include "m/vec/dvec2.h"
#include "m/vec/dvec3.h"
#include "m/vec/dvec4.h"
#include "m/mat/dmat3.h"

#ifndef MD_INTERSECTION_EPSILON
#define MD_INTERSECTION_EPSILON (double) 0.01
#endif

/** returns false if planes are parallel */
O_INLINE
bool md_intersection_plane_plane(m_dvec3 *out_pos, m_dvec3 *out_dir,
                                 m_dvec4 plane_hessian_a, m_dvec4 plane_hessian_b)
{
    *out_pos = *out_dir = dvec3_set(NAN);

    // parallel check
    if (dvec3_dot(plane_hessian_a.xyz, plane_hessian_b.xyz) > ((double) 1 - MD_INTERSECTION_EPSILON))
        return false;

    // line dir is orthogonal to both plane orthogonals (nx ny nz)
    m_dvec3 dir = dvec3_cross(plane_hessian_a.xyz, plane_hessian_b.xyz);
    dir = dvec3_normalize(dir);

    // hessian: (nx, ny, nz, d)
    // distance point to plane = x*nx + y*ny + z*nz + 1*d

    m_dmat3 A = {{0}};
    A = dmat3_set_row(A, plane_hessian_a.xyz, 0);
    A = dmat3_set_row(A, plane_hessian_b.xyz, 1);
    A = dmat3_set_row(A, dir, 2);

    // distance to planes should be 0, so -1*d
    m_dvec3 B = {{-plane_hessian_a.w, -plane_hessian_b.w, 0}};

    // solve A * X = B
    // X = A^-1 * B
    m_dvec3 X = dmat3_mul_vec(dmat3_inv(A), B);

    *out_pos = X;
    *out_dir = dir;

    return true;
}


/** returns false if plane and line are parallel */
O_INLINE
bool md_intersection_plane_line(m_dvec3 *out_pos, m_dvec4 plane_hessian,
                                m_dvec3 line_pos, m_dvec3 line_dir)
{
    *out_pos = dvec3_set(NAN);

    double plane_line_dot = dvec3_dot(plane_hessian.xyz, line_dir);

    // parallel check
    if (o_abs(plane_line_dot) < MD_INTERSECTION_EPSILON)
        return false;

    m_dvec3 plane_point = dvec3_scale(plane_hessian.xyz, -plane_hessian.w);
    m_dvec3 w = dvec3_sub_v(line_pos, plane_point);
    double si = -dvec3_dot(plane_hessian.xyz, w) / plane_line_dot;

    m_dvec3 si_dir = dvec3_scale(line_dir, si);

    // pos = w + si_dir + plane_point
    *out_pos = dvec3_add_v(dvec3_add_v(w, si_dir), plane_point);
    return true;
}


/** returns t (line_pos + line_dir * t), or NAN if not intersecting */
O_INLINE
double md_intersection_triangle_line(m_dvec3 v0, m_dvec3 v1, m_dvec3 v2,
                                     m_dvec3 line_pos, m_dvec3 line_dir,
                                     bool culling)
{
    m_dvec3 v0v1 = dvec3_sub_v(v1, v0);
    m_dvec3 v0v2 = dvec3_sub_v(v2, v0);
    m_dvec3 pvec = dvec3_cross(line_dir, v0v2);
    double det = dvec3_dot(v0v1, pvec);

    if (culling) {
        if (det < MD_INTERSECTION_EPSILON)
            return NAN;
    } else {
        if (o_abs(det) < MD_INTERSECTION_EPSILON)
            return NAN;
    }

    double inv_det = (double) 1 / det;

    m_dvec3 tvec = dvec3_sub_v(line_pos, v0);
    double u = dvec3_dot(tvec, pvec) * inv_det;
    if (u < 0 || u > 1)
        return NAN;

    m_dvec3 qvec = dvec3_cross(tvec, v0v1);
    double v = dvec3_dot(line_dir, qvec) * inv_det;
    if (v < 0 || u + v > 1)
        return NAN;

    return dvec3_dot(v0v2, qvec) * inv_det;
}


/** returns false if lines are parallel, searches for the nearest distance points */
O_INLINE
bool md_intersection_line_line(double *out_tau_a, double *out_tau_b,
                               m_dvec3 line_a_pos, m_dvec3 line_a_dir,
                               m_dvec3 line_b_pos, m_dvec3 line_b_dir)
{
    *out_tau_a = *out_tau_b = NAN;

    // Book: Real Time Collision Detection: section 5.1.8 Closest Point of Two Lines
    m_dvec3 r = dvec3_sub_v(line_a_pos, line_b_pos);

    double a = dvec3_dot(line_a_dir, line_a_dir);
    double b = dvec3_dot(line_a_dir, line_b_dir);
    double c = dvec3_dot(line_a_dir, r);
    double e = dvec3_dot(line_b_dir, line_b_dir);
    double f = dvec3_dot(line_b_dir, r);

    double d = a * e - b * b;

    // parallel check
    if (o_abs(d) < MD_INTERSECTION_EPSILON)
        return false;

    *out_tau_a = (b * f - c * e) / d;
    *out_tau_b = (a * f - b * c) / d;
    return true;
}


/** returns the projection point on the line by tau. searches for the nearest distance points */
O_INLINE
double md_intersection_line_point(m_dvec3 line_pos, m_dvec3 line_dir, m_dvec3 point)
{
    return dvec3_dot(dvec3_sub_v(point, line_pos), line_dir)
           / dvec3_dot(line_dir, line_dir);
}


//
// m_dvec2 versions:
//


/** returns false if lines are parallel */
O_INLINE
bool dintersection2_line_line(double *out_tau_a, double *out_tau_b,
                              m_dvec2 line_a_pos, m_dvec2 line_a_dir,
                              m_dvec2 line_b_pos, m_dvec2 line_b_dir)
{
    *out_tau_a = *out_tau_b = NAN;

    // stupid clone of the m_dvec2 version above:
    m_dvec2 r = dvec2_sub_v(line_a_pos, line_b_pos);

    double a = dvec2_dot(line_a_dir, line_a_dir);
    double b = dvec2_dot(line_a_dir, line_b_dir);
    double c = dvec2_dot(line_a_dir, r);
    double e = dvec2_dot(line_b_dir, line_b_dir);
    double f = dvec2_dot(line_b_dir, r);

    double d = a * e - b * b;

    // parallel check
    if (o_abs(d) < MD_INTERSECTION_EPSILON)
        return false;

    *out_tau_a = (b * f - c * e) / d;
    *out_tau_b = (a * f - b * c) / d;
    return true;
}

/** returns the projection point on the line by tau. searches for the nearest distance points */
O_INLINE
double dintersection2_line_point(m_dvec2 line_pos, m_dvec2 line_dir, m_dvec2 point)
{
    return dvec2_dot(dvec2_sub_v(point, line_pos), line_dir)
           / dvec2_dot(line_dir, line_dir);
}

#endif //M_UTILS_DBL_DINTERSECTION_H
