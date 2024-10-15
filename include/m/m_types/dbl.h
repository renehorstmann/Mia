#ifndef M_PUBLICTYPES_DBL_H
#define M_PUBLICTYPES_DBL_H

/**
 * @file m_types/dbl.h
 *
 * typed double matrices and vectors
 */

#include "m/common.h"

typedef union {
    double v[2];
    struct {
        double v0, v1;
    };
    struct {
        double r, g;
    };
    struct {
        double x, y;
    };
} m_dvec2;
static_assert(sizeof(m_dvec2) == sizeof(double) * 2, "wrong expected size");

/** casts a double * to a dereferenced vec2 */
#define m_dvec2_CAST(vec) (*(m_dvec2 *) (vec))


typedef union {
    double v[3];
    struct {
        double v0, v1, v2;
    };
    struct {
        double r, g, b;
    };
    m_dvec2 xy;
    struct {
        double x;
        union {
            struct {
                double y, z;
            };
            m_dvec2 yz;
        };
    };
} m_dvec3;
static_assert(sizeof(m_dvec3) == sizeof(double) * 3, "wrong expected size");

/** casts a double * to a dereferenced vec3 */
#define m_dvec3_CAST(vec) (*(m_dvec3 *) (vec))


typedef union {
    double v[4];
    struct {
        double v0, v1, v2, v3;
    };
    struct {
        double r, g, b, a;
    };
    m_dvec2 xy;
    m_dvec3 xyz;
    struct {
        double x;
        union {
            struct {
                double y;
                union {
                    struct {
                        double z, w;
                    };
                    m_dvec2 zw;
                };
            };
            m_dvec2 yz;
            m_dvec3 yzw;
        };
    };
} m_dvec4;
static_assert(sizeof(m_dvec4) == sizeof(double) * 4, "wrong expected size");

/** casts a double * to a dereferenced vec4 */
#define m_dvec4_CAST(vec) (*(m_dvec4 *) (vec))


typedef union {
    double v[2 * 2];
    double m[2][2];
    m_dvec2 col[2];
    struct {
        double v0, v1;
        double v2, v3;
    };
    struct {
        double m00, m01;
        double m10, m11;
    };
} m_dmat2;
static_assert(sizeof(m_dmat2) == sizeof(double) * 2 * 2, "wrong expected size");

/** casts a double * to a dereferenced mat2 */
#define m_dmat2_CAST(mat) (*(m_dmat2 *) (mat))


typedef union {
    double v[3 * 3];
    double m[3][3];
    m_dvec3 col[3];
    struct {
        double v0, v1, v2;
        double v3, v4, v5;
        double v6, v7, v8;
    };
    struct {
        double m00, m01, m02;
        double m10, m11, m12;
        double m20, m21, m22;
    };
} m_dmat3;
static_assert(sizeof(m_dmat3) == sizeof(double) * 3 * 3, "wrong expected size");

/** casts a double * to a dereferenced mat3 */
#define m_dmat3_CAST(mat) (*(m_dmat3 *) (mat))


typedef union {
    double v[4 * 4];
    double m[4][4];
    m_dvec4 col[4];
    struct {
        double v0, v1, v2, v3;
        double v4, v5, v6, v7;
        double v8, v9, v10, v11;
        double v12, v13, v14, v15;
    };
    struct {
        double m00, m01, m02, m03;
        double m10, m11, m12, m13;
        double m20, m21, m22, m23;
        double m30, m31, m32, m33;
    };
} m_dmat4;
static_assert(sizeof(m_dmat4) == sizeof(double) * 4 * 4, "wrong expected size");

/** casts a double * to a dereferenced mat4 */
#define m_dmat4_CAST(mat) (*(m_dmat4 *) (mat))


#endif //M_PUBLICTYPES_DBL_H
