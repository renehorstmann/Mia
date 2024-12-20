#ifndef M_UTILS_FLT_ROTATION_H
#define M_UTILS_FLT_ROTATION_H

/**
 * @file utils/rotation.h
 *
 * Creates matrices from given rotation vectors, etc.
 */


#include "m/vec/vec3.h"
#include "m/mat/mat3.h"
#include "m/mat/mat4.h"


/** creates a rotation matrix from angle_axis as xyz = axis and w = angle [rad] */
O_INLINE
m_mat3 mat3_rotation_from_angle_axis(m_vec4 angle_axis)
{
    // from cglm/affine/glm_rotate_make
    float c = cosf(angle_axis.w);
    float s = sinf(angle_axis.w);

    m_vec3 axis = vec3_normalize(angle_axis.xyz);
    m_vec3 v = vec3_scale(axis, (float) 1 - c);
    m_vec3 vs = vec3_scale(axis, s);

    m_mat3 res;
    res.col[0] = vec3_scale(axis, v.x);
    res.col[1] = vec3_scale(axis, v.y);
    res.col[2] = vec3_scale(axis, v.z);

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
m_mat3 mat3_rotation_from_dir_z(m_vec3 dir_z)
{
    m_vec3 z = vec3_normalize(dir_z);

    // align rotation x to unit x (if unit x ~= dir_z, align to unit y)
    m_vec3 align_x = vec3_unit_x();
    if (o_abs(vec3_dot(z, align_x)) > (float) 0.9)
        align_x = vec3_unit_y();

    m_vec3 y = vec3_normalize(vec3_cross(z, align_x));

    m_vec3 x = vec3_cross(y, z);

    m_mat3 res;
    res.col[0] = x;
    res.col[1] = y;
    res.col[2] = z;
    return res;
}


/** uses mat3_rotation_from_dir with ray_dir and sets the pose translation to ray_start */
O_INLINE
m_mat4 mat4_pose_from_ray(m_vec3 ray_start, m_vec3 ray_dir)
{
    m_mat4 pose = mat4_eye();
    pose = mat4_set_upper_left3(pose, mat3_rotation_from_dir_z(ray_dir));
    pose.col[3].xyz = ray_start;
    return pose;
}


#endif //M_UTILS_FLT_ROTATION_H
