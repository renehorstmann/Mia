#ifndef M_TYPES_DBL_H
#define M_TYPES_DBL_H

/**
 * @file types/dbl.h
 *
 * typed double matrices and vectors, without the m_* namespace
 */

#include "m/m_types/dbl.h"


typedef m_dvec2 dvec2;
typedef m_dvec3 dvec3;
typedef m_dvec4 dvec4;
typedef m_dmat2 dmat2;
typedef m_dmat3 dmat3;
typedef m_dmat4 dmat4;

#define dvec2_CAST m_dvec2_CAST
#define dvec3_CAST m_dvec3_CAST
#define dvec4_CAST m_dvec4_CAST
#define dmat2_CAST m_dmat2_CAST
#define dmat3_CAST m_dmat3_CAST
#define dmat4_CAST m_dmat4_CAST


/**
 * Creates a dvec2 from the given values (minimum 1)
 * dvec2_(1) -> (dvec2) {{1, 1}}
 * dvec2_(2, 3) -> (dvec2) {{2, 3}}
 * @return a compound literal for dvec2.
 */
#define dvec2_(...) (dvec2) {{ \
(double[]){__VA_ARGS__}[0], \
(double[]){__VA_ARGS__}[o_min(1, O_VA_ARGS_COUNT(__VA_ARGS__)-1)] \
}}


/**
 * Creates a dvec3 from the given values (minimum 1)
 * dvec3_(1) -> (dvec3) {{1, 1, 1}}
 * dvec3_(2, 3) -> (dvec3) {{2, 3, 3}}
 * dvec3_(4, 5, 6) -> (dvec3) {{4, 5, 6}}
 * @return a compound literal for dvec3.
 */
#define dvec3_(...) (dvec3) {{ \
(double[]){__VA_ARGS__}[0], \
(double[]){__VA_ARGS__}[o_min(1, O_VA_ARGS_COUNT(__VA_ARGS__)-1)], \
(double[]){__VA_ARGS__}[o_min(2, O_VA_ARGS_COUNT(__VA_ARGS__)-1)] \
}}


/**
 * Creates a dvec4 from the given values (minimum 1)
 * dvec4_(1) -> (dvec4) {{1, 1, 1, 1}}
 * dvec4_(2, 3) -> (dvec4) {{2, 3, 3, 3}}
 * dvec4_(4, 5, 6) -> (dvec4) {{4, 5, 6, 6}}
 * dvec4_(7, 8, 9, 0) -> (dvec4) {{7, 8, 9, 0}}
 * @return a compound literal for dvec4.
 */
#define dvec4_(...) (dvec4) {{ \
(double[]){__VA_ARGS__}[0], \
(double[]){__VA_ARGS__}[o_min(1, O_VA_ARGS_COUNT(__VA_ARGS__)-1)], \
(double[]){__VA_ARGS__}[o_min(2, O_VA_ARGS_COUNT(__VA_ARGS__)-1)], \
(double[]){__VA_ARGS__}[o_min(3, O_VA_ARGS_COUNT(__VA_ARGS__)-1)] \
}}

/**
 * Creates a dmat2 from the given dvec2 (minimum 1)
 * dmat2_(dvec2_(1)) -> (dmat2) {{1, 1,  1, 1}}
 * dmat2_(dvec2_(2), dvec2_(3)) -> (dmat2) {{2, 2,  3, 3}}
 * @return a compound literal for dmat2.
 */
#define dmat2_(...) (dmat2) (mat2) {{ \
(dvec2[]){__VA_ARGS__}[0], \
(dvec2[]){__VA_ARGS__}[o_min(1, O_VA_ARGS_COUNT(__VA_ARGS__)-1)] \
}}


/**
 * Creates a dmat3 from the given dvec3 (minimum 1)
 * dmat3_(dvec3_(1)) -> (dmat3) {{1, 1, 1,  1, 1, 1}}
 * dmat3_(dvec3_(2), dvec3_(3)) -> (dmat3) {{2, 2, 2,  3, 3, 3}}
 * dmat3_(dvec3_(4), dvec3_(5), dvec3_(6)) -> (dmat3) {{4, 4, 4,  5, 5, 5,  6, 6, 6}}
 * @return a compound literal for dmat3.
 */
#define dmat3_(...) (dmat3) {{ \
(dvec3[]){__VA_ARGS__}[0], \
(dvec2[]){__VA_ARGS__}[o_min(1, O_VA_ARGS_COUNT(__VA_ARGS__)-1)], \
(dvec2[]){__VA_ARGS__}[o_min(2, O_VA_ARGS_COUNT(__VA_ARGS__)-1)] \
}}



/**
 * Creates a dmat4 from the given dvec4 (minimum 1)
 * dmat4_(dvec4_(1)) -> (dmat4) {{1, 1, 1, 1,  1, 1, 1, 1}}
 * dmat4_(dvec4_(2), dvec4_(3)) -> (dmat4) {{2, 2, 2, 2,  3, 3, 3, 3}}
 * dmat4_(dvec4_(4), dvec4_(5), dvec4_(6)) -> (dmat4) {{4, 4, 4, 4,  5, 5, 5, 5,  6, 6, 6, 6}}
 * dmat4_(dvec4_(7), dvec4_(8), dvec4_(9), dvec4_(0)) -> (dmat4) {{7, 7, 7, 7,  8, 8, 8, 8,  9, 9, 9, 9,  0, 0, 0, 0}}
 * @return a compound literal for dmat4.
 */
#define dmat4_(...) (dmat4) {{ \
(dvec4[]){__VA_ARGS__}[0], \
(dvec2[]){__VA_ARGS__}[o_min(1, O_VA_ARGS_COUNT(__VA_ARGS__)-1)], \
(dvec2[]){__VA_ARGS__}[o_min(2, O_VA_ARGS_COUNT(__VA_ARGS__)-1)], \
(dvec2[]){__VA_ARGS__}[o_min(3, O_VA_ARGS_COUNT(__VA_ARGS__)-1)] \
}}

#endif //M_TYPES_DBL_H
