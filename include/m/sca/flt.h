#ifndef M_SCA_FLT_H
#define M_SCA_FLT_H

/**
 * @file sca/flt.h
 *
 * scalar functions working on floats in the m_ namespace
 */

#include "m/common.h"

/** maximum possible value */
#define m_MAX FLT_MAX

/** minimum possible value */
#define m_MIN (-FLT_MAX)

/** smallest positive value */
#define m_SMALL FLT_MIN

/** smallest positive value change */
#define m_EPS FLT_EPSILON

/* clone of some math.h constants */
#define m_E 2.718281828459045235360287471352662498f /* e */
#define m_LOG2E 1.442695040888963407359924681001892137f /* log_2 e */
#define m_LOG10E 0.434294481903251827651128918916605082f /* log_10 e */
#define m_LN2 0.693147180559945309417232121458176568f /* log_e 2 */
#define m_LN10 2.302585092994045684017991454684364208f /* log_e 10 */
#define m_PI 3.141592653589793238462643383279502884f /* pi */
#define m_PI_2 1.570796326794896619231321691639751442f /* pi/2 */
#define m_PI_4 0.785398163397448309615660845819875721f /* pi/4 */
#define m_1_PI 0.318309886183790671537767526745028724f /* 1/pi */
#define m_2_PI 0.636619772367581343075535053490057448f /* 2/pi */
#define m_2_SQRTPI 1.128379167095512573896158903121545172f /* 2/sqrt(pi) */
#define m_SQRT2 1.414213562373095048801688724209698079f /* sqrt(2) */
#define m_SQRT1_2 0.707106781186547524400844362104849039f /* 1/sqrt(2) */

/** for printing in IO */
#define m_PRI "g"

//
// macro copies of o/common
//

/** @return the absolute value */
#define m_abs(x) ((x) < 0 ? -(x) : (x))

/** @return the smaller value */
#define m_min(a, b) ((a)<(b)?(a):(b))

/** @return the bigger value */
#define m_max(a, b) ((a)>(b)?(a):(b))

/** @return x clamped in the range [min : max] (so can be exactly min and exactly max) */
#define m_clamp(x, min, max) ((x) < (min) ? (min) : ((x) > (max) ? (max) : (x)))

/** @return x>0: +1; x==0: 0; x<0: -1 (the sign of the value, or 0 on 0) */
#define m_sign(x) ((x) > 0 ? 1 : ((x) < 0 ? -1 : 0))

/** @return x>=edge? 1:0 (true or 1 if x has reached the step) */
#define m_step(edge, x) (((x) < (edge)) ? 0 : 1)

//
// integer based functions
//

/** dst = a * (1-t) + b * t */
O_INLINE
float m_mix(float a, float b, float t)
{
    return a * (1.0f - t) + b * t;
}

//
// floating point functions
//

/** dst = isnan(x) */
O_INLINE
bool m_isnan(float x)
{
    return isnan(x);
}

/** dst = isinf(x) */
O_INLINE
bool m_isinf(float x)
{
    return isinf(x);
}

/** dst = a * M_PI / 180 */
O_INLINE
float m_radians(float deg)
{
    return deg * m_PI / 180.0f;
}

/** dst = a * 180 / M_PI */
O_INLINE
float m_degrees(float rad)
{
    return rad * 180.0f / m_PI;
}

/** dst = sin(angle_rad) */
O_INLINE
float m_sin(float angle_rad)
{
    return sinf(angle_rad);
}

/** dst = cos(angle_rad) */
O_INLINE
float m_cos(float angle_rad)
{
    return cosf(angle_rad);
}

/** dst = tan(angle_rad) */
O_INLINE
float m_tan(float angle_rad)
{
    return tanf(angle_rad);
}

/** dst = asin(x) */
O_INLINE
float m_asin(float x)
{
    return asinf(x);
}

/** dst = acos(x) */
O_INLINE
float m_acos(float x)
{
    return acosf(x);
}

/** dst = atan(x) */
O_INLINE
float m_atan(float x)
{
    return atanf(x);
}

/** dst = atan2(y, x) */
O_INLINE
float m_atan2(float y, float x)
{
    return atan2f(y, x);
}

/** dst = sinh(angle_rad) */
O_INLINE
float m_sinh(float angle_rad)
{
    return sinhf(angle_rad);
}

/** dst = cosh(angle_rad) */
O_INLINE
float m_cosh(float angle_rad)
{
    return coshf(angle_rad);
}

/** dst = tanh(angle_rad) */
O_INLINE
float m_tanh(float angle_rad)
{
    return tanhf(angle_rad);
}

/** dst = asinh(x) */
O_INLINE
float m_asinh(float x)
{
    return asinhf(x);
}

/** dst = acosh(x) */
O_INLINE
float m_acosh(float x)
{
    return acoshf(x);
}

/** dst = atanh(x) */
O_INLINE
float m_atanh(float x)
{
    return atanhf(x);
}

/** dst = pow(x, y) */
O_INLINE
float m_pow(float x, float y)
{
    return powf(x, y);
}

/** dst = exp(x) */
O_INLINE
float m_exp(float x)
{
    return expf(x);
}

/** dst = log(x) */
O_INLINE
float m_log(float x)
{
    return logf(x);
}

/** dst = exp2(x) */
O_INLINE
float m_exp2(float x)
{
    return exp2f(x);
}

/** dst = log2(x) */
O_INLINE
float m_log2(float x)
{
    return log2f(x);
}

/** dst = sqrt(x) */
O_INLINE
float m_sqrt(float x)
{
    return sqrtf(x);
}

/** dst = 1 / sqrt(x) */
O_INLINE
float m_isqrt(float x)
{
    return 1.0f / sqrtf(x);
}

/** dst = godot like ease(x, y) */
O_INLINE
float m_ease(float x, float y)
{
    x = m_clamp(x, 0.0f, 1.0f);
    if (y > 0.0f) {
        return y<1.0f?
                1.0f - m_pow(1.0f - x, 1.0f / y)
                : m_pow(x, y);
    } else if (y < 0.0f) {
        return x<0.5f?
                m_pow(x * 2.0f, -y) * 0.5f
                : (1.0f - m_pow(1.0f - (x - 0.5f) * 2.0f, -y)) * 0.5f + 0.5f;
    }
    return 0.0f;
}

/** dst = round(x) */
O_INLINE
float m_round(float x)
{
    return roundf(x);
}

/** dst = floor(x) */
O_INLINE
float m_floor(float x)
{
    return floorf(x);
}

/** dst = ceil(x) */
O_INLINE
float m_ceil(float x)
{
    return ceilf(x);
}

/** dst = x - floor(x) */
O_INLINE
float m_fract(float x)
{
    return x - floorf(x);
}

/** dst = x - y * floor(x/y) */
O_INLINE
float m_mod(float x, float y)
{
    return x - y * floorf(x / y);
}

/** dst = x < edge1 ? 0 : (x > edge2 ? 1 : x * x * (3 - 2 * x)) Hermite polynomials */
O_INLINE
float m_smoothstep(float edge1, float edge2, float x)
{
    x = o_clamp((x - edge1) / (edge2 - edge1), 0.0f, 1.0f);
    return x * x * (3.0f - 2.0f * x);
}

/**
 * dst = sin(x*2pi)
 *  X   ->   Y
 * 0.00 ->  0.0
 * 0.25 -> +1.0
 * 0.50 ->  0.0
 * 0.75 -> -1.0
 */
O_INLINE
float m_signal_wave(float x)
{
    return m_sin(x * 2.0f * m_PI);
}

/**
 * dst = -1 or 1, x: [0:1] (0-0.5 is -1)
 *  X   ->   Y
 * 0.25 -> -1.0
 * 0.75 -> +1.0
 */
O_INLINE
float m_signal_block(float x)
{
    return 2.0f * o_step(0.5, m_mod(x, 1.0f)) - 1.0f;
}

/**
 * dst = saw like signal, linear up, step down, x: [0:1] -> dst [-1:1]
 *  X   ->   Y
 * 0.00 -> -1.0
 * 0.50 ->  0.0
 */
O_INLINE
float m_signal_saw(float x)
{
    return 2.0f * m_mod(x, 1.0f) - 1.0f;
}

/**
 * dst = linear up and down signal, x: [0:1] -> dst [-1:1]
 *  X   ->   Y
 * 0.00 -> -1.0
 * 0.25 ->  0.0
 * 0.50 -> +1.0
 * 0.75 ->  0.0
 */
O_INLINE
float m_signal_ramp(float x)
{
    x = m_mod(x + 0.5f, 1.0f);
    return 4.0f * o_abs(x - 0.5f) - 1.0f;
}

/**
 * dst = saw like signal, linear up, step down, x: [0:1] -> dst [-1:1]
 * smoothed edge at [1-edge:1]
 *  X   ->   Y
 * 0.00 -> -1.0
 * 1.00 -> +1.0
 */
O_INLINE
float m_signal_smoothsaw_single(float x, float edge)
{
    x = m_mod(x, 1.0f);

    // linear: y = a*x
    // smooth: y = 1-b*(x-1)^2
    // equations solved, so that f==(1-edge) -> y and derivate are equal
    float f = 1.0f - edge;
    float b = -1.0f / (2.0f * (f - 1.0f));
    float a = (1.0f - b * m_pow(f - 1.0f, 2.0f)) / f;

    float signal;
    // linear up
    if (x < f) {
        signal = a * x;
    } else {
        // smooth end
        signal = 1.0f - b * m_pow(x - 1.0f, 2.0f);
    }
    return -1.0f + 2.0f * signal;
}

/*
 * dst = saw like signal, linear up, step down, x: [0:1] -> dst [-1:1]
 * smoothed edges between [0:edge] , [1-edge:1]
 *  X   ->   Y
 * 0.00 -> -1.0
 * 0.50 ->  0.0
 */
O_INLINE
float m_signal_smoothsaw(float x, float edge)
{
    x = m_mod(x, 1.0f);
    if (x < 0.5f) {
        // 0.5*2 is exactly 1.0f, which will mod to 0, which will result in 1.0 instead of -1.0
        return -0.5f - m_signal_smoothsaw_single((0.5f - x) * 1.99999f, edge * 2.0f) / 2.0f;
    }
    return 0.5f + m_signal_smoothsaw_single((x - 0.5f) * 2.0f, edge * 2.0f) / 2.0f;
}

/**
 * dst = ramp like signal, linear up, linear down, x: [0:1] -> dst [-1:1]
 * smoothed edges between [0:edge] , [0.5-edge:0.5+edge] , [1-edge:1]
 *  X   ->   Y
 * 0.00 -> -1.0
 * 0.25 ->  0.0
 * 0.50 -> +1.0
 * 0.75 ->  0.0
 */
O_INLINE
float m_signal_smoothramp(float x, float edge)
{
    x = m_mod(x, 1.0f);
    if (x < 0.5f) {
        return m_signal_smoothsaw(x * 2.0f, edge * 2.0f);
    }
    return -m_signal_smoothsaw((x - 0.5f) * 2.0f, edge * 2.0f);
}


#endif //M_SCA_FLT_H
