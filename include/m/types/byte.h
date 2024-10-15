#ifndef M_TYPES_BYTE_H
#define M_TYPES_BYTE_H

/**
 * @file types/byte.h
 *
 * typed obyte vectors, without the m_* namespace
 */

#include "m/m_types/byte.h"


typedef m_bvec2 bvec2;
typedef m_bvec3 bvec3;
typedef m_bvec4 bvec4;

#define bvec2_CAST m_bvec2_CAST
#define bvec3_CAST m_bvec3_CAST
#define bvec4_CAST m_bvec4_CAST


/**
 * Creates a bvec2 from the given values (minimum 1)
 * bvec2_(1) -> (bvec2) {{1, 1}}
 * bvec2_(2, 3) -> (bvec2) {{2, 3}}
 * @return a compound literal for bvec2.
 */
#define bvec2_(...) (bvec2) {{ \
(obyte[]){__VA_ARGS__}[0], \
(obyte[]){__VA_ARGS__}[o_min(1, O_VA_ARGS_COUNT(__VA_ARGS__)-1)] \
}}


/**
 * Creates a bvec3 from the given values (minimum 1)
 * bvec3_(1) -> (bvec3) {{1, 1, 1}}
 * bvec3_(2, 3) -> (bvec3) {{2, 3, 3}}
 * bvec3_(4, 5, 6) -> (bvec3) {{4, 5, 6}}
 * @return a compound literal for bvec3.
 */
#define bvec3_(...) (bvec3) {{ \
(obyte[]){__VA_ARGS__}[0], \
(obyte[]){__VA_ARGS__}[o_min(1, O_VA_ARGS_COUNT(__VA_ARGS__)-1)], \
(obyte[]){__VA_ARGS__}[o_min(2, O_VA_ARGS_COUNT(__VA_ARGS__)-1)] \
}}


/**
 * Creates a bvec4 from the given values (minimum 1)
 * bvec4_(1) -> (bvec4) {{1, 1, 1, 1}}
 * bvec4_(2, 3) -> (bvec4) {{2, 3, 3, 3}}
 * bvec4_(4, 5, 6) -> (bvec4) {{4, 5, 6, 6}}
 * bvec4_(7, 8, 9, 0) -> (bvec4) {{7, 8, 9, 0}}
 * @return a compound literal for bvec4.
 */
#define bvec4_(...) (bvec4) {{ \
(obyte[]){__VA_ARGS__}[0], \
(obyte[]){__VA_ARGS__}[o_min(1, O_VA_ARGS_COUNT(__VA_ARGS__)-1)], \
(obyte[]){__VA_ARGS__}[o_min(2, O_VA_ARGS_COUNT(__VA_ARGS__)-1)], \
(obyte[]){__VA_ARGS__}[o_min(3, O_VA_ARGS_COUNT(__VA_ARGS__)-1)] \
}}

#endif //M_TYPES_BYTE_H
