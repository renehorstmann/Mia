#ifndef M_TYPES_INT_H
#define M_TYPES_INT_H

/**
 * @file types/int.h
 *
 * typed int vectors, without the m_* namespace
 */

#include "m/m_types/int.h"


typedef m_ivec2 ivec2;
typedef m_ivec3 ivec3;
typedef m_ivec4 ivec4;

#define ivec2_CAST m_ivec2_CAST
#define ivec3_CAST m_ivec3_CAST
#define ivec4_CAST m_ivec4_CAST


/**
 * Creates a ivec2 from the given values (minimum 1)
 * ivec2_(1) -> (ivec2) {{1, 1}}
 * ivec2_(2, 3) -> (ivec2) {{2, 3}}
 * @return a compound literal for ivec2.
 */
#define ivec2_(...) (ivec2) {{ \
(int[]){__VA_ARGS__}[0], \
(int[]){__VA_ARGS__}[o_min(1, O_VA_ARGS_COUNT(__VA_ARGS__)-1)] \
}}


/**
 * Creates a ivec3 from the given values (minimum 1)
 * ivec3_(1) -> (ivec3) {{1, 1, 1}}
 * ivec3_(2, 3) -> (ivec3) {{2, 3, 3}}
 * ivec3_(4, 5, 6) -> (ivec3) {{4, 5, 6}}
 * @return a compound literal for ivec3.
 */
#define ivec3_(...) (ivec3) {{ \
(int[]){__VA_ARGS__}[0], \
(int[]){__VA_ARGS__}[o_min(1, O_VA_ARGS_COUNT(__VA_ARGS__)-1)], \
(int[]){__VA_ARGS__}[o_min(2, O_VA_ARGS_COUNT(__VA_ARGS__)-1)] \
}}


/**
 * Creates a ivec4 from the given values (minimum 1)
 * ivec4_(1) -> (ivec4) {{1, 1, 1, 1}}
 * ivec4_(2, 3) -> (ivec4) {{2, 3, 3, 3}}
 * ivec4_(4, 5, 6) -> (ivec4) {{4, 5, 6, 6}}
 * ivec4_(7, 8, 9, 0) -> (ivec4) {{7, 8, 9, 0}}
 * @return a compound literal for ivec4.
 */
#define ivec4_(...) (ivec4) {{ \
(int[]){__VA_ARGS__}[0], \
(int[]){__VA_ARGS__}[o_min(1, O_VA_ARGS_COUNT(__VA_ARGS__)-1)], \
(int[]){__VA_ARGS__}[o_min(2, O_VA_ARGS_COUNT(__VA_ARGS__)-1)], \
(int[]){__VA_ARGS__}[o_min(3, O_VA_ARGS_COUNT(__VA_ARGS__)-1)] \
}}

#endif //M_TYPES_INT_H
