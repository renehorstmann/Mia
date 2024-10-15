#ifndef M_UTILS_DBL_DROTATION_H
#define M_UTILS_DBL_DROTATION_H

/**
 * @file utils/rotation.h
 *
 * Creates matrices from given rotation vectors, etc.
 */


#include "m/vec/dvec3.h"
#include "m/mat/dmat3.h"
#include "m/mat/dmat4.h"


/** creates a rotation matrix from angle_axis as xyz = axis and w = angle [rad] */
O_INLINE
m_dmat3 dmat3_rotation_from_angle_axis(m_dvec4 angle_axis)
{
    // from cglm/affine/glm_rotate_make
    double c = cosf(angle_axis.w);
    double s = sinf(angle_axis.w);

    m_dvec3 axis = dvec3_normalize(angle_axis.xyz);
    m_dvec3 v = dvec3_scale(axis, (double) 1 - c);
    m_dvec3 vs = dvec3_scale(axis, s);

    m_dmat3 res;
    res.col[0] = dvec3_scale(axis, v.x);
    res.col[1] = dvec3_scale(axis, v.y);
    res.col[2] = dvec3_scale(axis, v.z);

    res.m[0][0] += c;
    res.m[1][0] -= vs.z;
    res.m[2][0] += vs.y;
    res.m[0][1] += vs.z;
    res.m[1][1] += c;
    res.m[2][1] -= vs.x;
    res.m[0][2] -= vs.y;
    res.m[1][2] += vs.x;
    res.m[2][2] += c;
    return res;
}


/** creates a rotation matrix aligned to x (or y if dir_z~=x), so that the z axis is dir_z */
O_INLINE
m_dmat3 dmat3_rotation_from_dir_z(m_dvec3 dir_z)
{
    m_dvec3 z = dvec3_normalize(dir_z);

    // align rotation x to unit x (if unit x ~= dir_z, align to unit y)
    m_dvec3 align_x = dvec3_unit_x();
    if (o_abs(dvec3_dot(z, align_x)) > (double) 0.9)
        align_x = dvec3_unit_y();

    m_dvec3 y = dvec3_normalize(dvec3_cross(z, align_x));

    m_dvec3 x = dvec3_cross(y, z);

    m_dmat3 res;
    res.col[0] = x;
    res.col[1] = y;
    res.col[2] = z;
    return res;
}


/** uses dmat3_rotation_from_dir with ray_dir and sets the pose translation to ray_start */
O_INLINE
m_dmat4 dmat4_pose_from_ray(m_dvec3 ray_start, m_dvec3 ray_dir)
{
    m_dmat4 pose = dmat4_eye();
    pose = dmat4_set_upper_left3(pose, dmat3_rotation_from_dir_z(ray_dir));
    pose.col[3].xyz = ray_start;
    return pose;
}


#endif //M_UTILS_DBL_DROTATION_H
