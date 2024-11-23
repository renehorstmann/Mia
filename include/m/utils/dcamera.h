#ifndef M_UTILS_DBL_DCAMERA_H
#define M_UTILS_DBL_DCAMERA_H

/**
 * @file utils/dcamera.h
 *
 * 3d camera matrix functions
 */


#include "m/vec/dvec3.h"
#include "m/mat/dmat4.h"

/**
 * Creates an orthogonal camera matrix
 * @param near_z: near clipping plane
 * @param far_z: far flipping plane
 */
O_INLINE
m_dmat4 dmat4_camera_ortho(double left, double right,
                           double bottom, double top,
                           double near_z, double far_z)
{
    // from cglm/cam.h/glm_ortho
    double rl = (double) 1 / (right - left);
    double tb = (double) 1 / (top - bottom);
    double fn = (double) -1 / (far_z - near_z);

    m_dmat4 res = {{0}};
    res.m[0][0] = (double) 2 * rl;
    res.m[1][1] = (double) 2 * tb;
    res.m[2][2] = (double) 2 * fn;
    res.m[3][0] = -(right + left) * rl;
    res.m[3][1] = -(top + bottom) * tb;
    res.m[3][2] = (far_z + near_z) * fn;
    res.m[3][3] = (double) 1;
    return res;
}

/** 
 * Creates a perspective (frustum) camera matrix
 * @param near_z: near clipping plane
 * @param far_z: far flipping plane
 */
O_INLINE
m_dmat4 dmat4_camera_frustum(double left, double right,
                             double bottom, double top,
                             double near_z, double far_z)
{
    // from cglm/cam.h/glm_frustum
    double rl = (double) 1 / (right - left);
    double tb = (double) 1 / (top - bottom);
    double fn = (double) -1 / (far_z - near_z);
    double nv = (double) 2 * near_z;

    m_dmat4 res = {{0}};
    res.m[0][0] = nv * rl;
    res.m[1][1] = nv * tb;
    res.m[2][0] = (right + left) * rl;
    res.m[2][1] = (top + bottom) * tb;
    res.m[2][2] = (far_z + near_z) * fn;
    res.m[2][3] = (double) -1;
    res.m[3][2] = far_z * nv * fn;
    return res;
}

/**
 * Creates a perspective camera matrix.
 * @param fovy: field of view angle [rad]
 * @param aspect: width/height ratio
 * @param near_z: near clipping plane
 * @param far_z: far flipping plane
 */
O_INLINE
m_dmat4 dmat4_camera_perspective(double fovy, double aspect, double near_z, double far_z)
{
    // from cglm/cam.h/glm_perspective
    double f = (double) 1 / md_tan(fovy * (double) 0.5);
    double fn = (double) 1 / (near_z - far_z);

    m_dmat4 res = {{0}};
    res.m[0][0] = f / aspect;
    res.m[1][1] = f;
    res.m[2][2] = (near_z + far_z) * fn;
    res.m[2][3] = (double) -1;
    res.m[3][2] = (double) 2 * near_z * far_z * fn;
    return res;
}

/**
 * Creates the view matrix for a 3d camera.
 * up vector must not be parallel with eye to center vector
 */
O_INLINE
m_dmat4 dmat4_camera_lookat(m_dvec3 eye, m_dvec3 center, m_dvec3 up)
{
    // from cglm/cam.h/glm_lookar

    m_dvec3 f = dvec3_sub_v(center, eye);
    f = dvec3_normalize(f);

    m_dvec3 s = dvec3_cross(f, up);
    s = dvec3_normalize(s);

    m_dvec3 u = dvec3_cross(s, f);

    m_dmat4 res = dmat4_eye();
    res.col[0].xyz = s;
    res.col[1].xyz = u;
    res.col[2].xyz = dvec3_neg(f);
    res.col[3].x = -dvec3_dot(s, eye);
    res.col[3].y = -dvec3_dot(u, eye);
    res.col[3].z = dvec3_dot(f, eye);
    return res;
}

#endif //M_UTILS_DBL_DCAMERA_H
