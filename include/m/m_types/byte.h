#ifndef M_PUBLICTYPES_BYTE_H
#define M_PUBLICTYPES_BYTE_H

/**
 * @file m_types/byte.h
 *
 * typed obyte vectors
 */

#include "m/common.h"

typedef union {
    obyte v[2];
    struct {
        obyte v0, v1;
    };
    struct {
        obyte r, g;
    };
    struct {
        obyte x, y;
    };
} m_bvec2;
static_assert(sizeof(m_bvec2) == sizeof(obyte) * 2, "wrong expected size");

/** casts a obyte * to a dereferenced vec2 */
#define m_bvec2_CAST(vec) (*(m_bvec2 *) (vec))


typedef union {
    obyte v[3];
    struct {
        obyte v0, v1, v2;
    };
    struct {
        obyte r, g, b;
    };
    m_bvec2 xy;
    struct {
        obyte x;
        union {
            struct {
                obyte y, z;
            };
            m_bvec2 yz;
        };
    };
} m_bvec3;
static_assert(sizeof(m_bvec3) == sizeof(obyte) * 3, "wrong expected size");

/** casts a obyte * to a dereferenced vec3 */
#define m_bvec3_CAST(vec) (*(m_bvec3 *) (vec))


typedef union {
    obyte v[4];
    struct {
        obyte v0, v1, v2, v3;
    };
    struct {
        obyte r, g, b, a;
    };
    m_bvec2 xy;
    m_bvec3 xyz;
    struct {
        obyte x;
        union {
            struct {
                obyte y;
                union {
                    struct {
                        obyte z, w;
                    };
                    m_bvec2 zw;
                };
            };
            m_bvec2 yz;
            m_bvec3 yzw;
        };
    };
} m_bvec4;
static_assert(sizeof(m_bvec4) == sizeof(obyte) * 4, "wrong expected size");

/** casts a obyte * to a dereferenced vec4 */
#define m_bvec4_CAST(vec) (*(m_bvec4 *) (vec))


#endif //M_PUBLICTYPES_BYTE_H
