#ifndef M_COMMON_H
#define M_COMMON_H

/**
 * @file common.h
 *
 * Common used | Basic includes, macros, types and functions
 */


#include "o/common.h"
#include <math.h>

// maximal size for some vec and mat functions
#ifndef M_MAX_SIZE
#define M_MAX_SIZE 16
#endif


/** Unpacks the first 2 values of an m type */
#define m_2(mt) (mt).v[0], (mt).v[1]

/** Unpacks the first 3 values of an m type */
#define m_3(mt) (mt).v[0], (mt).v[1], (mt).v[2]

/** Unpacks the first 4 values of an m type */
#define m_4(mt) (mt).v[0], (mt).v[1], (mt).v[2], (mt).v[3]

/** Unpacks the first 9 values of an m type (3x3 mat) */
#define m_9(mt) (mt).v[0], (mt).v[1], (mt).v[2], (mt).v[3], (mt).v[4], (mt).v[5], (mt).v[6], (mt).v[7], (mt).v[8]

/** Unpacks the first 16 values of an m type (4x4 mat) */
#define m_16(mt) (mt).v[0], (mt).v[1], (mt).v[2], (mt).v[3], (mt).v[4], (mt).v[5], (mt).v[6], (mt).v[7], \
(mt).v[8], (mt).v[9], (mt).v[10], (mt).v[11], (mt).v[12], (mt).v[13], (mt).v[14], (mt).v[15]

#endif //M_COMMON_H
