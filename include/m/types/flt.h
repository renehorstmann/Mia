#ifndef M_TYPES_FLT_H
#define M_TYPES_FLT_H

/**
 * @file types/flt.h
 *
 * typed float matrices and vectors, without the m_* namespace
 */

#include "m/m_types/flt.h"


typedef m_vec2 vec2;
typedef m_vec3 vec3;
typedef m_vec4 vec4;
typedef m_mat2 mat2;
typedef m_mat3 mat3;
typedef m_mat4 mat4;

#define vec2_CAST m_vec2_CAST
#define vec3_CAST m_vec3_CAST
#define vec4_CAST m_vec4_CAST
#define mat2_CAST m_mat2_CAST
#define mat3_CAST m_mat3_CAST
#define mat4_CAST m_mat4_CAST


/**
 * Creates a vec2 from the given values (minimum 1)
 * vec2_(1) -> (vec2) {{1, 1}}
 * vec2_(2, 3) -> (vec2) {{2, 3}}
 * @return a compound literal for vec2.
 */
#define vec2_(...) (vec2) {{ \
(float[]){__VA_ARGS__}[0], \
(float[]){__VA_ARGS__}[o_min(1, O_VA_ARGS_COUNT(__VA_ARGS__)-1)] \
}}


/**
 * Creates a vec3 from the given values (minimum 1)
 * vec3_(1) -> (vec3) {{1, 1, 1}}
 * vec3_(2, 3) -> (vec3) {{2, 3, 3}}
 * vec3_(4, 5, 6) -> (vec3) {{4, 5, 6}}
 * @return a compound literal for vec3.
 */
#define vec3_(...) (vec3) {{ \
(float[]){__VA_ARGS__}[0], \
(float[]){__VA_ARGS__}[o_min(1, O_VA_ARGS_COUNT(__VA_ARGS__)-1)], \
(float[]){__VA_ARGS__}[o_min(2, O_VA_ARGS_COUNT(__VA_ARGS__)-1)] \
}}


/**
 * Creates a vec4 from the given values (minimum 1)
 * vec4_(1) -> (vec4) {{1, 1, 1, 1}}
 * vec4_(2, 3) -> (vec4) {{2, 3, 3, 3}}
 * vec4_(4, 5, 6) -> (vec4) {{4, 5, 6, 6}}
 * vec4_(7, 8, 9, 0) -> (vec4) {{7, 8, 9, 0}}
 * @return a compound literal for vec4.
 */
#define vec4_(...) (vec4) {{ \
(float[]){__VA_ARGS__}[0], \
(float[]){__VA_ARGS__}[o_min(1, O_VA_ARGS_COUNT(__VA_ARGS__)-1)], \
(float[]){__VA_ARGS__}[o_min(2, O_VA_ARGS_COUNT(__VA_ARGS__)-1)], \
(float[]){__VA_ARGS__}[o_min(3, O_VA_ARGS_COUNT(__VA_ARGS__)-1)] \
}}

/**
 * Creates a mat2 from the given vec2 (minimum 1)
 * mat2_(vec2_(1)) -> (mat2) {{1, 1,  1, 1}}
 * mat2_(vec2_(2), vec2_(3)) -> (mat2) {{2, 2,  3, 3}}
 * @return a compound literal for mat2.
 */
#define mat2_(...) (mat2) {{ \
(vec2[]){__VA_ARGS__}[0], \
(vec2[]){__VA_ARGS__}[o_min(1, O_VA_ARGS_COUNT(__VA_ARGS__)-1)] \
}}


/**
 * Creates a mat3 from the given vec3 (minimum 1)
 * mat3_(vec3_(1)) -> (mat3) {{1, 1, 1,  1, 1, 1}}
 * mat3_(vec3_(2), vec3_(3)) -> (mat3) {{2, 2, 2,  3, 3, 3}}
 * mat3_(vec3_(4), vec3_(5), vec3_(6)) -> (mat3) {{4, 4, 4,  5, 5, 5,  6, 6, 6}}
 * @return a compound literal for mat3.
 */
#define mat3_(...) (mat3) {{ \
(vec3[]){__VA_ARGS__}[0], \
(vec2[]){__VA_ARGS__}[o_min(1, O_VA_ARGS_COUNT(__VA_ARGS__)-1)], \
(vec2[]){__VA_ARGS__}[o_min(2, O_VA_ARGS_COUNT(__VA_ARGS__)-1)] \
}}



/**
 * Creates a mat4 from the given vec4 (minimum 1)
 * mat4_(vec4_(1)) -> (mat4) {{1, 1, 1, 1,  1, 1, 1, 1}}
 * mat4_(vec4_(2), vec4_(3)) -> (mat4) {{2, 2, 2, 2,  3, 3, 3, 3}}
 * mat4_(vec4_(4), vec4_(5), vec4_(6)) -> (mat4) {{4, 4, 4, 4,  5, 5, 5, 5,  6, 6, 6, 6}}
 * mat4_(vec4_(7), vec4_(8), vec4_(9), vec4_(0)) -> (mat4) {{7, 7, 7, 7,  8, 8, 8, 8,  9, 9, 9, 9,  0, 0, 0, 0}}
 * @return a compound literal for mat4.
 */
#define mat4_(...) (mat4) {{ \
(vec4[]){__VA_ARGS__}[0], \
(vec2[]){__VA_ARGS__}[o_min(1, O_VA_ARGS_COUNT(__VA_ARGS__)-1)], \
(vec2[]){__VA_ARGS__}[o_min(2, O_VA_ARGS_COUNT(__VA_ARGS__)-1)], \
(vec2[]){__VA_ARGS__}[o_min(3, O_VA_ARGS_COUNT(__VA_ARGS__)-1)] \
}}

#endif //M_TYPES_FLT_H
