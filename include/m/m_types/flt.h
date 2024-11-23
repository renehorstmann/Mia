#ifndef M_PUBLICTYPES_FLT_H
#define M_PUBLICTYPES_FLT_H

/**
 * @file m_types/flt.h
 *
 * typed float matrices and vectors
 */

#include "m/common.h"

typedef union {
    float v[2];
    struct {
        float v0, v1;
    };
    struct {
        float r, g;
    };
    struct {
        float x, y;
    };
} m_vec2;
static_assert(sizeof(m_vec2) == sizeof(float) * 2, "wrong expected size");

/** casts a float * to a dereferenced vec2 */
#define m_vec2_CAST(vec) (*(m_vec2 *) (vec))


typedef union {
    float v[3];
    struct {
        float v0, v1, v2;
    };
    struct {
        float r, g, b;
    };
    m_vec2 xy;
    struct {
        float x;
        union {
            struct {
                float y, z;
            };
            m_vec2 yz;
        };
    };
} m_vec3;
static_assert(sizeof(m_vec3) == sizeof(float) * 3, "wrong expected size");

/** casts a float * to a dereferenced vec3 */
#define m_vec3_CAST(vec) (*(m_vec3 *) (vec))


typedef union {
    float v[4];
    struct {
        float v0, v1, v2, v3;
    };
    struct {
        float r, g, b, a;
    };
    m_vec2 xy;
    m_vec3 xyz;
    struct {
        float x;
        union {
            struct {
                float y;
                union {
                    struct {
                        float z, w;
                    };
                    m_vec2 zw;
                };
            };
            m_vec2 yz;
            m_vec3 yzw;
        };
    };
} m_vec4;
static_assert(sizeof(m_vec4) == sizeof(float) * 4, "wrong expected size");

/** casts a float * to a dereferenced vec4 */
#define m_vec4_CAST(vec) (*(m_vec4 *) (vec))


typedef union {
    float v[2 * 2];
    float m[2][2];
    m_vec2 col[2];
    struct {
        float v0, v1;
        float v2, v3;
    };
    struct {
        float m00, m01;
        float m10, m11;
    };
} m_mat2;
static_assert(sizeof(m_mat2) == sizeof(float) * 2 * 2, "wrong expected size");

/** casts a float * to a dereferenced mat2 */
#define m_mat2_CAST(mat) (*(m_mat2 *) (mat))


typedef union {
    float v[3 * 3];
    float m[3][3];
    m_vec3 col[3];
    struct {
        float v0, v1, v2;
        float v3, v4, v5;
        float v6, v7, v8;
    };
    struct {
        float m00, m01, m02;
        float m10, m11, m12;
        float m20, m21, m22;
    };
} m_mat3;
static_assert(sizeof(m_mat3) == sizeof(float) * 3 * 3, "wrong expected size");

/** casts a float * to a dereferenced mat3 */
#define m_mat3_CAST(mat) (*(m_mat3 *) (mat))


typedef union {
    float v[4 * 4];
    float m[4][4];
    m_vec4 col[4];
    struct {
        float v0, v1, v2, v3;
        float v4, v5, v6, v7;
        float v8, v9, v10, v11;
        float v12, v13, v14, v15;
    };
    struct {
        float m00, m01, m02, m03;
        float m10, m11, m12, m13;
        float m20, m21, m22, m23;
        float m30, m31, m32, m33;
    };
} m_mat4;
static_assert(sizeof(m_mat4) == sizeof(float) * 4 * 4, "wrong expected size");

/** casts a float * to a dereferenced mat4 */
#define m_mat4_CAST(mat) (*(m_mat4 *) (mat))


#endif //M_PUBLICTYPES_FLT_H
