#ifndef M_PUBLICTYPES_INT_H
#define M_PUBLICTYPES_INT_H

/**
 * @file m_types/int.h
 *
 * typed int vectors
 */

#include "m/common.h"

typedef union {
    int v[2];
    struct {
        int v0, v1;
    };
    struct {
        int r, g;
    };
    struct {
        int x, y;
    };
} m_ivec2;
static_assert(sizeof(m_ivec2) == sizeof(int) * 2, "wrong expected size");

/** casts a int * to a dereferenced vec2 */
#define m_ivec2_CAST(vec) (*(m_ivec2 *) (vec))


typedef union {
    int v[3];
    struct {
        int v0, v1, v2;
    };
    struct {
        int r, g, b;
    };
    m_ivec2 xy;
    struct {
        int x;
        union {
            struct {
                int y, z;
            };
            m_ivec2 yz;
        };
    };
} m_ivec3;
static_assert(sizeof(m_ivec3) == sizeof(int) * 3, "wrong expected size");

/** casts a int * to a dereferenced vec3 */
#define m_ivec3_CAST(vec) (*(m_ivec3 *) (vec))


typedef union {
    int v[4];
    struct {
        int v0, v1, v2, v3;
    };
    struct {
        int r, g, b, a;
    };
    m_ivec2 xy;
    m_ivec3 xyz;
    struct {
        int x;
        union {
            struct {
                int y;
                union {
                    struct {
                        int z, w;
                    };
                    m_ivec2 zw;
                };
            };
            m_ivec2 yz;
            m_ivec3 yzw;
        };
    };
} m_ivec4;
static_assert(sizeof(m_ivec4) == sizeof(int) * 4, "wrong expected size");

/** casts a int * to a dereferenced vec4 */
#define m_ivec4_CAST(vec) (*(m_ivec4 *) (vec))


#endif //M_PUBLICTYPES_INT_H
