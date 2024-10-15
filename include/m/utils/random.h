#ifndef M_UTILS_FLT_RANDOM
#define M_UTILS_FLT_RANDOM

/**
 * @file utils/random.h
 *
 * Random math for the types
 */


#ifndef M_RANDOM_FUNCTION
#define M_RANDOM_FUNCTION o_rand
#endif

#ifndef M_RANDOM_FUNCTION_MAX
#define M_RANDOM_FUNCTION_MAX o_rand_MAX
#endif

#include "m/m_types/flt.h"
#include "m/sca/flt.h"

/** dst = rand() [0:1] */
O_INLINE
float m_random(void)
{
    return (float) 1 * M_RANDOM_FUNCTION() / (float) M_RANDOM_FUNCTION_MAX;
}

/** dst = rand() [a:b] */
O_INLINE
float m_random_range(float a, float b)
{
    return a + (b - a) * m_random();
}

/** dst = mean +- amplitude * rand() */
O_INLINE
float m_random_noise(float mean, float amplitude)
{
    return mean - amplitude + 2 * amplitude * m_random();
}

/** dst = mean +- sigma * normalized_rand() */
O_INLINE
float m_random_normal(float mean, float sigma)
{
    float u1, u2;
    do {
        u1 = m_random();
    } while (u1 <= (float) 0.00000001);
    u2 = m_random();

    float mag = sigma * m_sqrt((float) -2 * log(u1));
    return mean + mag * m_cos(2 * m_PI * u2);
}

//
// vecn
//

/** dst = rand() [0:1] */
O_INLINE
void vecn_random(float *dst, int n)
{
    for (int i = 0; i < n; i++) {
        dst[i] = m_random();
    }
}

/** dst = rand() [a:b] */
O_INLINE
void vecn_random_range(float *dst, float a, float b, int n)
{
    for (int i = 0; i < n; i++) {
        dst[i] = m_random_range(a, b);
    }
}

/** dst = rand() [a:b] */
O_INLINE
void vecn_random_range_v(float *dst, const float *a, const float *b, int n)
{
    for (int i = 0; i < n; i++) {
        dst[i] = m_random_range(a[i], b[i]);
    }
}

/** dst = mean +- amplitude * rand() */
O_INLINE
void vecn_random_noise(float *dst, float mean, float amplitude, int n)
{
    for (int i = 0; i < n; i++) {
        dst[i] = m_random_noise(mean, amplitude);
    }
}

/** dst = mean +- amplitude * rand() */
O_INLINE
void vecn_random_noise_v(float *dst, const float *mean, const float *amplitude, int n)
{
    for (int i = 0; i < n; i++) {
        dst[i] = m_random_noise(mean[i], amplitude[i]);
    }
}

/** dst = mean +- sigma * normalized_rand() */
O_INLINE
void vecn_random_normal(float *dst, float mean, float sigma, int n)
{
    for (int i = 0; i < n; i++) {
        dst[i] = m_random_normal(mean, sigma);
    }
}

/** dst = mean +- sigma * normalized_rand() */
O_INLINE
void vecn_random_normal_v(float *dst, const float *mean, const float *sigma, int n)
{
    for (int i = 0; i < n; i++) {
        dst[i] = m_random_normal(mean[i], sigma[i]);
    }
}

//
// m_vec2
//

/** dst = rand() [0:1] */
O_INLINE
m_vec2 vec2_random(void)
{
    m_vec2 res;
    vecn_random(res.v, 2);
    return res;
}

/** dst = rand() [a:b] */
O_INLINE
m_vec2 vec2_random_range(float a, float b)
{
    m_vec2 res;
    vecn_random_range(res.v, a, b, 2);
    return res;
}

/** dst = rand() [a:b] */
O_INLINE
m_vec2 vec2_random_range_v(m_vec2 a, m_vec2 b)
{
    m_vec2 res;
    vecn_random_range_v(res.v, a.v, b.v, 2);
    return res;
}

/** dst = mean +- amplitude * rand() */
O_INLINE
m_vec2 vec2_random_noise(float mean, float amplitude)
{
    m_vec2 res;
    vecn_random_noise(res.v, mean, amplitude, 2);
    return res;
}

/** dst = mean +- amplitude * rand() */
O_INLINE
m_vec2 vec2_random_noise_v(m_vec2 mean, m_vec2 amplitude)
{
    m_vec2 res;
    vecn_random_noise_v(res.v, mean.v, amplitude.v, 2);
    return res;
}

/** dst = mean +- sigma * normalized_rand() */
O_INLINE
m_vec2 vec2_random_normal(float mean, float sigma)
{
    m_vec2 res;
    vecn_random_normal(res.v, mean, sigma, 2);
    return res;
}

/** dst = mean +- sigma * normalized_rand() */
O_INLINE
m_vec2 vec2_random_normal_v(m_vec2 mean, m_vec2 sigma)
{
    m_vec2 res;
    vecn_random_normal_v(res.v, mean.v, sigma.v, 2);
    return res;
}

//
// m_vec3
//

/** dst = rand() [0:1] */
O_INLINE
m_vec3 vec3_random(void)
{
    m_vec3 res;
    vecn_random(res.v, 3);
    return res;
}

/** dst = rand() [a:b] */
O_INLINE
m_vec3 vec3_random_range(float a, float b)
{
    m_vec3 res;
    vecn_random_range(res.v, a, b, 3);
    return res;
}

/** dst = rand() [a:b] */
O_INLINE
m_vec3 vec3_random_range_v(m_vec3 a, m_vec3 b)
{
    m_vec3 res;
    vecn_random_range_v(res.v, a.v, b.v, 3);
    return res;
}

/** dst = mean +- amplitude * rand() */
O_INLINE
m_vec3 vec3_random_noise(float mean, float amplitude)
{
    m_vec3 res;
    vecn_random_noise(res.v, mean, amplitude, 3);
    return res;
}

/** dst = mean +- amplitude * rand() */
O_INLINE
m_vec3 vec3_random_noise_v(m_vec3 mean, m_vec3 amplitude)
{
    m_vec3 res;
    vecn_random_noise_v(res.v, mean.v, amplitude.v, 3);
    return res;
}

/** dst = mean +- sigma * normalized_rand() */
O_INLINE
m_vec3 vec3_random_normal(float mean, float sigma)
{
    m_vec3 res;
    vecn_random_normal(res.v, mean, sigma, 3);
    return res;
}

/** dst = mean +- sigma * normalized_rand() */
O_INLINE
m_vec3 vec3_random_normal_v(m_vec3 mean, m_vec3 sigma)
{
    m_vec3 res;
    vecn_random_normal_v(res.v, mean.v, sigma.v, 3);
    return res;
}


//
// m_vec4
//

/** dst = rand() [0:1] */
O_INLINE
m_vec4 vec4_random(void)
{
    m_vec4 res;
    vecn_random(res.v, 4);
    return res;
}

/** dst = rand() [a:b] */
O_INLINE
m_vec4 vec4_random_range(float a, float b)
{
    m_vec4 res;
    vecn_random_range(res.v, a, b, 4);
    return res;
}

/** dst = rand() [a:b] */
O_INLINE
m_vec4 vec4_random_range_v(m_vec4 a, m_vec4 b)
{
    m_vec4 res;
    vecn_random_range_v(res.v, a.v, b.v, 4);
    return res;
}

/** dst = mean +- amplitude * rand() */
O_INLINE
m_vec4 vec4_random_noise(float mean, float amplitude)
{
    m_vec4 res;
    vecn_random_noise(res.v, mean, amplitude, 4);
    return res;
}

/** dst = mean +- amplitude * rand() */
O_INLINE
m_vec4 vec4_random_noise_v(m_vec4 mean, m_vec4 amplitude)
{
    m_vec4 res;
    vecn_random_noise_v(res.v, mean.v, amplitude.v, 4);
    return res;
}

/** dst = mean +- sigma * normalized_rand() */
O_INLINE
m_vec4 vec4_random_normal(float mean, float sigma)
{
    m_vec4 res;
    vecn_random_normal(res.v, mean, sigma, 4);
    return res;
}

/** dst = mean +- sigma * normalized_rand() */
O_INLINE
m_vec4 vec4_random_normal_v(m_vec4 mean, m_vec4 sigma)
{
    m_vec4 res;
    vecn_random_normal_v(res.v, mean.v, sigma.v, 4);
    return res;
}

#endif // M_UTILS_FLT_RANDOM
