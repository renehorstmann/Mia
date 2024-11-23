#ifndef M_UTILS_FLT_QUAT_H
#define M_UTILS_FLT_QUAT_H

/**
 * @file utils/quat.h
 *
 * Quaternion math
 */


#include "m/vec/vec3.h"
#include "m/vec/vec4.h"
#include "m/mat/mat4.h"


/** m_vec4 = [0, 0, 0, 1] */
O_INLINE
m_vec4 m_quat_eye(void)
{
    m_vec4 self;
    self.x = 0;
    self.y = 0;
    self.z = 0;
    self.w = 1;
    return self;
}


/** dst = -x, -y, -z, w */
O_INLINE
m_vec4 m_quat_conj(m_vec4 q)
{
    m_vec4 self;
    self.x = -q.x;
    self.y = -q.y;
    self.z = -q.z;
    self.w = q.w;
    return self;
}

/** dst = inv(m_vec4) */
O_INLINE
m_vec4 m_quat_inv(m_vec4 q)
{
    m_vec4 conj = m_quat_conj(q);
    return vec4_scale(conj, 1.0f / vec4_dot(q, q));
}


/** dst = a @ b Hamilton Product */
O_INLINE
m_vec4 m_quat_mul(m_vec4 q_a, m_vec4 q_b)
{
    // from cglm/m_vec4/glm_m_quat_mul
    m_vec4 res;
    res.v[0] = q_a.v[3] * q_b.v[0] + q_a.v[0] * q_b.v[3] + q_a.v[1] * q_b.v[2] - q_a.v[2] * q_b.v[1];
    res.v[1] = q_a.v[3] * q_b.v[1] - q_a.v[0] * q_b.v[2] + q_a.v[1] * q_b.v[3] + q_a.v[2] * q_b.v[0];
    res.v[2] = q_a.v[3] * q_b.v[2] + q_a.v[0] * q_b.v[1] - q_a.v[1] * q_b.v[0] + q_a.v[2] * q_b.v[3];
    res.v[3] = q_a.v[3] * q_b.v[3] - q_a.v[0] * q_b.v[0] - q_a.v[1] * q_b.v[1] - q_a.v[2] * q_b.v[2];
    return res;
}


/** angle_axis = xyz + w=angle in rad */
O_INLINE
m_vec4 m_quat_from_angle_axis(m_vec4 angle_axis)
{
    // from cglm/m_vec4/glm_quatv
    float a = angle_axis.w * 0.5f;
    float c = m_cos(a);
    float s = m_sin(a);

    m_vec3 axis = vec3_normalize(angle_axis.xyz);

    m_vec4 res;
    res.xyz = vec3_scale(axis, s);
    res.w = c;
    return res;
}


/** angle_axis = xyz + w=angle in rad */
O_INLINE
m_vec4 m_quat_to_angle_axis(m_vec4 q)
{
    // from cglm/m_vec4/glm_m_quat_angle
    /*
     sin(theta / 2) = length(x*x + y*y + z*z)
     cos(theta / 2) = w
     theta          = 2 * atan(sin(theta / 2) / cos(theta / 2))
     */
    float imag_len = vec3_norm(q.xyz);
    float angle = (float) 2 * m_atan2(imag_len, q.w);

    m_vec4 angle_axis;
    angle_axis.xyz = vec3_normalize(q.xyz);
    angle_axis.w = angle;
    return angle_axis;
}

/** returns the orientation of m_vec4 as 3*3 rotation matrix */
O_INLINE
m_mat3 m_quat_to_rotation_matrix(m_vec4 q)
{
    // from cglm/m_vec4/glm_m_quat_mat3
    float norm = vec4_norm(q);
    float s = norm > 0 ? (float) 2 / norm : 0;

    float x = q.x;
    float y = q.y;
    float z = q.z;
    float w = q.w;

    float xx, xy, wx;
    float yy, yz, wy;
    float zz, xz, wz;
    xx = s * x * x;
    xy = s * x * y;
    wx = s * w * x;
    yy = s * y * y;
    yz = s * y * z;
    wy = s * w * y;
    zz = s * z * z;
    xz = s * x * z;
    wz = s * w * z;

    m_mat3 res;
    res.m[0][0] = (float) 1 - yy - zz;
    res.m[1][1] = (float) 1 - xx - zz;
    res.m[2][2] = (float) 1 - xx - yy;

    res.m[0][1] = xy + wz;
    res.m[1][2] = yz + wx;
    res.m[2][0] = xz + wy;

    res.m[1][0] = xy - wz;
    res.m[2][1] = yz - wx;
    res.m[0][2] = xz - wy;
    return res;
}


/** returns the orientation of a 3*3 rotation matrix as m_vec4 */
O_INLINE
m_vec4 m_quat_from_rotation_matrix(m_mat3 mat)
{
    // from cglm/m_mat3/glm_mat3_quat
    m_vec4 res;
    float trace, r, rinv;
    trace = mat.m[0][0] + mat.m[1][1] + mat.m[2][2];
    if (trace >= 0) {
        r = m_sqrt((float) 1 + trace);
        rinv = (float) 0.5 / r;

        res.v[0] = rinv * (mat.m[1][2] - mat.m[2][1]);
        res.v[1] = rinv * (mat.m[2][0] - mat.m[0][2]);
        res.v[2] = rinv * (mat.m[0][1] - mat.m[1][0]);
        res.v[3] = r * (float) 0.5;
    } else if (mat.m[0][0] >= mat.m[1][1] && mat.m[0][0] >= mat.m[2][2]) {
        r = m_sqrt((float) 1 - mat.m[1][1] - mat.m[2][2] + mat.m[0][0]);
        rinv = (float) 0.5 / r;

        res.v[0] = r * (float) 0.5;
        res.v[1] = rinv * (mat.m[0][1] + mat.m[1][0]);
        res.v[2] = rinv * (mat.m[0][2] + mat.m[2][0]);
        res.v[3] = rinv * (mat.m[1][2] - mat.m[2][1]);
    } else if (mat.m[1][1] >= mat.m[2][2]) {
        r = m_sqrt((float) 1 - mat.m[0][0] - mat.m[2][2] + mat.m[1][1]);
        rinv = (float) 0.5 / r;

        res.v[0] = rinv * (mat.m[0][1] + mat.m[1][0]);
        res.v[1] = r * (float) 0.5;
        res.v[2] = rinv * (mat.m[1][2] + mat.m[2][1]);
        res.v[3] = rinv * (mat.m[2][0] - mat.m[0][2]);
    } else {
        r = m_sqrt((float) 1 - mat.m[0][0] - mat.m[1][1] + mat.m[2][2]);
        rinv = (float) 0.5 / r;

        res.v[0] = rinv * (mat.m[0][2] + mat.m[2][0]);
        res.v[1] = rinv * (mat.m[1][2] + mat.m[2][1]);
        res.v[2] = r * (float) 0.5;
        res.v[3] = rinv * (mat.m[0][1] - mat.m[1][0]);
    }

    return res;
}

/** returns the orientation of a 3*3 rotation matrix as m_vec4 */
O_INLINE
void m_quat_pose_from_pose_matrix(m_vec3 *out_pose_pos, m_vec4 *out_pose_quat, m_mat4 mat_pose)
{
    *out_pose_pos = mat_pose.col[3].xyz;
    *out_pose_quat = m_quat_from_rotation_matrix(mat4_get_upper_left3(mat_pose));
}

/** returns the orientation of a 3*3 rotation matrix as m_vec4 */
O_INLINE
m_mat4 m_quat_pose_to_pose_matrix(m_vec3 pose_pos, m_vec4 pose_quat)
{
    m_mat4 pose = mat4_eye();
    pose.col[3].xyz = pose_pos;
    mat4_set_this_upper_left3(&pose, m_quat_to_rotation_matrix(pose_quat));
    return pose;
}


/** returns the rotated position pos with the rotation of q */
O_INLINE
m_vec3 m_quat_rotate_pos(m_vec4 q, m_vec3 pos)
{
    m_vec4 p;
    p.xyz = pos;
    p.w = 0;
    // res = q @ pos(w=0) @ conj(q)
    return m_quat_mul(q, m_quat_mul(p, m_quat_conj(q))).xyz;
}


/**
 * Inverts the given pose (position + orientation)
 * Same as inverting a m_mat4 pose
 */
O_INLINE
void m_quat_pose_inv(m_vec3 *out_pose_inv_pos, m_vec4 *out_pose_inv_quat, m_vec3 pose_pos, m_vec4 pose_quat)
{
    *out_pose_inv_quat = m_quat_inv(pose_quat);
    *out_pose_inv_pos = vec3_neg(m_quat_rotate_pos(*out_pose_inv_quat, pose_pos));
}


/** returns pos transform by pose a @ b */
O_INLINE
m_vec3 m_quat_pose_transform_pos(m_vec3 a_pos, m_vec4 a_quat, m_vec3 pos_b)
{
    return vec3_add_v(a_pos, m_quat_rotate_pos(a_quat, pos_b));
}


/** returns pose transform of a @ b */
O_INLINE
void m_quat_pose_transform_pose(m_vec3 *out_pos, m_vec4 *out_quat,
                                m_vec3 a_pos, m_vec4 a_quat,
                                m_vec3 b_pos, m_vec4 b_quat)
{
    *out_pos = m_quat_pose_transform_pos(a_pos, a_quat, b_pos);
    *out_quat = m_quat_mul(a_quat, b_quat);
}


/** spherical linear interpolation between a and b, t in [0 : 1] */
O_INLINE
m_vec4 m_quat_slerp(m_vec4 q_a, m_vec4 q_b, float t)
{
    // from cglm/m_vec4/glm_m_quat_slerp
    float cos_theta = vec4_dot(q_a, q_b);

    if (o_abs(cos_theta) >= 1)
        return q_a;

    if (cos_theta < 0) {
        q_a = vec4_neg(q_a);
        cos_theta = -cos_theta;
    }

    float sin_theta = m_sqrt((float) 1 - cos_theta * cos_theta);

    /* LERP q_b avoid zero division */
    if (o_abs(sin_theta) < (float) 0.001)
        return vec4_mix(q_a, q_b, t);

    /* SLERP */
    float angle = m_acos(cos_theta);
    m_vec4 q1 = vec4_scale(q_a, m_sin(((float) 1 - t) * angle));
    m_vec4 q2 = vec4_scale(q_b, m_sin(t * angle));

    q1 = vec4_add_v(q1, q2);
    return vec4_scale(q1, (float) 1 / sin_theta);
}


#endif //M_UTILS_FLT_QUAT_H
