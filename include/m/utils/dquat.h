#ifndef M_UTILS_DBL_DQUAT_H
#define M_UTILS_DBL_DQUAT_H

/**
 * @file utils/quat.h
 *
 * Quaternion math
 */


#include "m/vec/dvec3.h"
#include "m/vec/dvec4.h"
#include "m/mat/dmat4.h"


/** m_dvec4 = [0, 0, 0, 1] */
O_INLINE
m_dvec4 md_quat_eye(void)
{
    m_dvec4 self;
    self.x = 0;
    self.y = 0;
    self.z = 0;
    self.w = 1;
    return self;
}


/** dst = -x, -y, -z, w */
O_INLINE
m_dvec4 md_quat_conj(m_dvec4 q)
{
    m_dvec4 self;
    self.x = -q.x;
    self.y = -q.y;
    self.z = -q.z;
    self.w = q.w;
    return self;
}

/** dst = inv(m_dvec4) */
O_INLINE
m_dvec4 md_quat_inv(m_dvec4 q)
{
    m_dvec4 conj = md_quat_conj(q);
    return dvec4_scale(conj, 1.0f / dvec4_dot(q, q));
}


/** dst = a @ b Hamilton Product */
O_INLINE
m_dvec4 md_quat_mul(m_dvec4 q_a, m_dvec4 q_b)
{
    // from cglm/m_dvec4/glm_quat_mul
    m_dvec4 res;
    res.v[0] = q_a.v[3] * q_b.v[0] + q_a.v[0] * q_b.v[3] + q_a.v[1] * q_b.v[2] - q_a.v[2] * q_b.v[1];
    res.v[1] = q_a.v[3] * q_b.v[1] - q_a.v[0] * q_b.v[2] + q_a.v[1] * q_b.v[3] + q_a.v[2] * q_b.v[0];
    res.v[2] = q_a.v[3] * q_b.v[2] + q_a.v[0] * q_b.v[1] - q_a.v[1] * q_b.v[0] + q_a.v[2] * q_b.v[3];
    res.v[3] = q_a.v[3] * q_b.v[3] - q_a.v[0] * q_b.v[0] - q_a.v[1] * q_b.v[1] - q_a.v[2] * q_b.v[2];
    return res;
}


/** angle_axis = xyz + w=angle in rad */
O_INLINE
m_dvec4 md_quat_from_angle_axis(m_dvec4 angle_axis)
{
    // from cglm/m_dvec4/glm_quatv
    double a = angle_axis.w * 0.5f;
    double c = md_cos(a);
    double s = md_sin(a);

    m_dvec3 axis = dvec3_normalize(angle_axis.xyz);

    m_dvec4 res;
    res.xyz = dvec3_scale(axis, s);
    res.w = c;
    return res;
}


/** angle_axis = xyz + w=angle in rad */
O_INLINE
m_dvec4 md_quat_to_angle_axis(m_dvec4 q)
{
    // from cglm/m_dvec4/glm_quat_angle
    /*
     sin(theta / 2) = length(x*x + y*y + z*z)
     cos(theta / 2) = w
     theta          = 2 * atan(sin(theta / 2) / cos(theta / 2))
     */
    double imag_len = dvec3_norm(q.xyz);
    double angle = (double) 2 * md_atan2(imag_len, q.w);

    m_dvec4 angle_axis;
    angle_axis.xyz = dvec3_normalize(q.xyz);
    angle_axis.w = angle;
    return angle_axis;
}

/** returns the orientation of m_dvec4 as 3*3 rotation matrix */
O_INLINE
m_dmat3 md_quat_to_rotation_matrix(m_dvec4 q)
{
    // from cglm/m_dvec4/glm_quat_mat3
    double norm = dvec4_norm(q);
    double s = norm > 0 ? (double) 2 / norm : 0;

    double x = q.x;
    double y = q.y;
    double z = q.z;
    double w = q.w;

    double xx, xy, wx;
    double yy, yz, wy;
    double zz, xz, wz;
    xx = s * x * x;
    xy = s * x * y;
    wx = s * w * x;
    yy = s * y * y;
    yz = s * y * z;
    wy = s * w * y;
    zz = s * z * z;
    xz = s * x * z;
    wz = s * w * z;

    m_dmat3 res;
    res.m[0][0] = (double) 1 - yy - zz;
    res.m[1][1] = (double) 1 - xx - zz;
    res.m[2][2] = (double) 1 - xx - yy;

    res.m[0][1] = xy + wz;
    res.m[1][2] = yz + wx;
    res.m[2][0] = xz + wy;

    res.m[1][0] = xy - wz;
    res.m[2][1] = yz - wx;
    res.m[0][2] = xz - wy;
    return res;
}


/** returns the orientation of a 3*3 rotation matrix as m_dvec4 */
O_INLINE
m_dvec4 md_quat_from_rotation_matrix(m_dmat3 mat)
{
    // from cglm/m_dmat3/glm_mat3_quat
    m_dvec4 res;
    double trace, r, rinv;
    trace = mat.m[0][0] + mat.m[1][1] + mat.m[2][2];
    if (trace >= 0) {
        r = md_sqrt((double) 1 + trace);
        rinv = (double) 0.5 / r;

        res.v[0] = rinv * (mat.m[1][2] - mat.m[2][1]);
        res.v[1] = rinv * (mat.m[2][0] - mat.m[0][2]);
        res.v[2] = rinv * (mat.m[0][1] - mat.m[1][0]);
        res.v[3] = r * (double) 0.5;
    } else if (mat.m[0][0] >= mat.m[1][1] && mat.m[0][0] >= mat.m[2][2]) {
        r = md_sqrt((double) 1 - mat.m[1][1] - mat.m[2][2] + mat.m[0][0]);
        rinv = (double) 0.5 / r;

        res.v[0] = r * (double) 0.5;
        res.v[1] = rinv * (mat.m[0][1] + mat.m[1][0]);
        res.v[2] = rinv * (mat.m[0][2] + mat.m[2][0]);
        res.v[3] = rinv * (mat.m[1][2] - mat.m[2][1]);
    } else if (mat.m[1][1] >= mat.m[2][2]) {
        r = md_sqrt((double) 1 - mat.m[0][0] - mat.m[2][2] + mat.m[1][1]);
        rinv = (double) 0.5 / r;

        res.v[0] = rinv * (mat.m[0][1] + mat.m[1][0]);
        res.v[1] = r * (double) 0.5;
        res.v[2] = rinv * (mat.m[1][2] + mat.m[2][1]);
        res.v[3] = rinv * (mat.m[2][0] - mat.m[0][2]);
    } else {
        r = md_sqrt((double) 1 - mat.m[0][0] - mat.m[1][1] + mat.m[2][2]);
        rinv = (double) 0.5 / r;

        res.v[0] = rinv * (mat.m[0][2] + mat.m[2][0]);
        res.v[1] = rinv * (mat.m[1][2] + mat.m[2][1]);
        res.v[2] = r * (double) 0.5;
        res.v[3] = rinv * (mat.m[0][1] - mat.m[1][0]);
    }

    return res;
}

/** returns the orientation of a 3*3 rotation matrix as m_dvec4 */
O_INLINE
void md_quat_pose_from_pose_matrix(m_dvec3 *out_pose_pos, m_dvec4 *out_pose_quat, m_dmat4 mat_pose)
{
    *out_pose_pos = mat_pose.col[3].xyz;
    *out_pose_quat = md_quat_from_rotation_matrix(dmat4_get_upper_left3(mat_pose));
}

/** returns the orientation of a 3*3 rotation matrix as m_dvec4 */
O_INLINE
m_dmat4 md_quat_pose_to_pose_matrix(m_dvec3 pose_pos, m_dvec4 pose_quat)
{
    m_dmat4 pose = dmat4_eye();
    pose.col[3].xyz = pose_pos;
    dmat4_set_this_upper_left3(&pose, md_quat_to_rotation_matrix(pose_quat));
    return pose;
}


/** returns the rotated position pos with the rotation of q */
O_INLINE
m_dvec3 md_quat_rotate_pos(m_dvec4 q, m_dvec3 pos)
{
    m_dvec4 p;
    p.xyz = pos;
    p.w = 0;
    // res = q @ pos(w=0) @ conj(q)
    return md_quat_mul(q, md_quat_mul(p, md_quat_conj(q))).xyz;
}


/**
 * Inverts the given pose (position + orientation)
 * Same as inverting a m_dmat4 pose
 */
O_INLINE
void md_quat_pose_inv(m_dvec3 *out_pose_inv_pos, m_dvec4 *out_pose_inv_quat, m_dvec3 pose_pos, m_dvec4 pose_quat)
{
    *out_pose_inv_quat = md_quat_inv(pose_quat);
    *out_pose_inv_pos = dvec3_neg(md_quat_rotate_pos(*out_pose_inv_quat, pose_pos));
}


/** returns pos transform by pose a @ b */
O_INLINE
m_dvec3 md_quat_pose_transform_pos(m_dvec3 a_pos, m_dvec4 a_quat, m_dvec3 pos_b)
{
    return dvec3_add_v(a_pos, md_quat_rotate_pos(a_quat, pos_b));
}


/** returns pose transform of a @ b */
O_INLINE
void md_quat_pose_transform_pose(m_dvec3 *out_pos, m_dvec4 *out_quat,
                                 m_dvec3 a_pos, m_dvec4 a_quat,
                                 m_dvec3 b_pos, m_dvec4 b_quat)
{
    *out_pos = md_quat_pose_transform_pos(a_pos, a_quat, b_pos);
    *out_quat = md_quat_mul(a_quat, b_quat);
}


/** spherical linear interpolation between a and b, t in [0 : 1] */
O_INLINE
m_dvec4 md_quat_slerp(m_dvec4 q_a, m_dvec4 q_b, double t)
{
    // from cglm/m_dvec4/glm_quat_slerp
    double cos_theta = dvec4_dot(q_a, q_b);

    if (o_abs(cos_theta) >= 1)
        return q_a;

    if (cos_theta < 0) {
        q_a = dvec4_neg(q_a);
        cos_theta = -cos_theta;
    }

    double sin_theta = md_sqrt((double) 1 - cos_theta * cos_theta);

    /* LERP q_b avoid zero division */
    if (o_abs(sin_theta) < (double) 0.001)
        return dvec4_mix(q_a, q_b, t);

    /* SLERP */
    double angle = md_acos(cos_theta);
    m_dvec4 q1 = dvec4_scale(q_a, md_sin(((double) 1 - t) * angle));
    m_dvec4 q2 = dvec4_scale(q_b, md_sin(t * angle));

    q1 = dvec4_add_v(q1, q2);
    return dvec4_scale(q1, (double) 1 / sin_theta);
}


#endif //M_UTILS_DBL_DQUAT_H
