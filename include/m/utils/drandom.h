#ifndef M_UTILS_DBL_DRANDOM
#define M_UTILS_DBL_DRANDOM

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

#include "m/m_types/dbl.h"
#include "m/sca/dbl.h"

/** dst = rand() [0:1] */
O_INLINE
double md_random(void)
{
    return (double) 1 * M_RANDOM_FUNCTION() / (double) M_RANDOM_FUNCTION_MAX;
}

/** dst = rand() [a:b] */
O_INLINE
double md_random_range(double a, double b)
{
    return a + (b - a) * md_random();
}

/** dst = mean +- amplitude * rand() */
O_INLINE
double md_random_noise(double mean, double amplitude)
{
    return mean - amplitude + 2 * amplitude * md_random();
}

/** dst = mean +- sigma * normalized_rand() */
O_INLINE
double md_random_normal(double mean, double sigma)
{
    double u1, u2;
    do {
        u1 = md_random();
    } while (u1 <= (double) 0.00000001);
    u2 = md_random();

    double mag = sigma * md_sqrt((double) -2 * log(u1));
    return mean + mag * md_cos(2 * md_PI * u2);
}

//
// vecn
//

/** dst = rand() [0:1] */
O_INLINE
void dvecn_random(double *dst, int n)
{
    for (int i = 0; i < n; i++) {
        dst[i] = md_random();
    }
}

/** dst = rand() [a:b] */
O_INLINE
void dvecn_random_range(double *dst, double a, double b, int n)
{
    for (int i = 0; i < n; i++) {
        dst[i] = md_random_range(a, b);
    }
}

/** dst = rand() [a:b] */
O_INLINE
void dvecn_random_range_vec(double *dst, const double *a, const double *b, int n)
{
    for (int i = 0; i < n; i++) {
        dst[i] = md_random_range(a[i], b[i]);
    }
}

/** dst = mean +- amplitude * rand() */
O_INLINE
void dvecn_random_noise(double *dst, double mean, double amplitude, int n)
{
    for (int i = 0; i < n; i++) {
        dst[i] = md_random_noise(mean, amplitude);
    }
}

/** dst = mean +- amplitude * rand() */
O_INLINE
void dvecn_random_noise_vec(double *dst, const double *mean, const double *amplitude, int n)
{
    for (int i = 0; i < n; i++) {
        dst[i] = md_random_noise(mean[i], amplitude[i]);
    }
}

/** dst = mean +- sigma * normalized_rand() */
O_INLINE
void dvecn_random_normal(double *dst, double mean, double sigma, int n)
{
    for (int i = 0; i < n; i++) {
        dst[i] = md_random_normal(mean, sigma);
    }
}

/** dst = mean +- sigma * normalized_rand() */
O_INLINE
void dvecn_random_normal_vec(double *dst, const double *mean, const double *sigma, int n)
{
    for (int i = 0; i < n; i++) {
        dst[i] = md_random_normal(mean[i], sigma[i]);
    }
}

//
// m_dvec2
//

/** dst = rand() [0:1] */
O_INLINE
m_dvec2 dvec2_random(void)
{
    m_dvec2 res;
    dvecn_random(res.v, 2);
    return res;
}

/** dst = rand() [a:b] */
O_INLINE
m_dvec2 dvec2_random_range(double a, double b)
{
    m_dvec2 res;
    dvecn_random_range(res.v, a, b, 2);
    return res;
}

/** dst = rand() [a:b] */
O_INLINE
m_dvec2 dvec2_random_range_vec(m_dvec2 a, m_dvec2 b)
{
    m_dvec2 res;
    dvecn_random_range_vec(res.v, a.v, b.v, 2);
    return res;
}

/** dst = mean +- amplitude * rand() */
O_INLINE
m_dvec2 dvec2_random_noise(double mean, double amplitude)
{
    m_dvec2 res;
    dvecn_random_noise(res.v, mean, amplitude, 2);
    return res;
}

/** dst = mean +- amplitude * rand() */
O_INLINE
m_dvec2 dvec2_random_noise_vec(m_dvec2 mean, m_dvec2 amplitude)
{
    m_dvec2 res;
    dvecn_random_noise_vec(res.v, mean.v, amplitude.v, 2);
    return res;
}

/** dst = mean +- sigma * normalized_rand() */
O_INLINE
m_dvec2 dvec2_random_normal(double mean, double sigma)
{
    m_dvec2 res;
    dvecn_random_normal(res.v, mean, sigma, 2);
    return res;
}

/** dst = mean +- sigma * normalized_rand() */
O_INLINE
m_dvec2 dvec2_random_normal_vec(m_dvec2 mean, m_dvec2 sigma)
{
    m_dvec2 res;
    dvecn_random_normal_vec(res.v, mean.v, sigma.v, 2);
    return res;
}

//
// m_dvec3
//

/** dst = rand() [0:1] */
O_INLINE
m_dvec3 dvec3_random(void)
{
    m_dvec3 res;
    dvecn_random(res.v, 3);
    return res;
}

/** dst = rand() [a:b] */
O_INLINE
m_dvec3 dvec3_random_range(double a, double b)
{
    m_dvec3 res;
    dvecn_random_range(res.v, a, b, 3);
    return res;
}

/** dst = rand() [a:b] */
O_INLINE
m_dvec3 dvec3_random_range_vec(m_dvec3 a, m_dvec3 b)
{
    m_dvec3 res;
    dvecn_random_range_vec(res.v, a.v, b.v, 3);
    return res;
}

/** dst = mean +- amplitude * rand() */
O_INLINE
m_dvec3 dvec3_random_noise(double mean, double amplitude)
{
    m_dvec3 res;
    dvecn_random_noise(res.v, mean, amplitude, 3);
    return res;
}

/** dst = mean +- amplitude * rand() */
O_INLINE
m_dvec3 dvec3_random_noise_vec(m_dvec3 mean, m_dvec3 amplitude)
{
    m_dvec3 res;
    dvecn_random_noise_vec(res.v, mean.v, amplitude.v, 3);
    return res;
}

/** dst = mean +- sigma * normalized_rand() */
O_INLINE
m_dvec3 dvec3_random_normal(double mean, double sigma)
{
    m_dvec3 res;
    dvecn_random_normal(res.v, mean, sigma, 3);
    return res;
}

/** dst = mean +- sigma * normalized_rand() */
O_INLINE
m_dvec3 dvec3_random_normal_vec(m_dvec3 mean, m_dvec3 sigma)
{
    m_dvec3 res;
    dvecn_random_normal_vec(res.v, mean.v, sigma.v, 3);
    return res;
}


//
// m_dvec4
//

/** dst = rand() [0:1] */
O_INLINE
m_dvec4 dvec4_random(void)
{
    m_dvec4 res;
    dvecn_random(res.v, 4);
    return res;
}

/** dst = rand() [a:b] */
O_INLINE
m_dvec4 dvec4_random_range(double a, double b)
{
    m_dvec4 res;
    dvecn_random_range(res.v, a, b, 4);
    return res;
}

/** dst = rand() [a:b] */
O_INLINE
m_dvec4 dvec4_random_range_vec(m_dvec4 a, m_dvec4 b)
{
    m_dvec4 res;
    dvecn_random_range_vec(res.v, a.v, b.v, 4);
    return res;
}

/** dst = mean +- amplitude * rand() */
O_INLINE
m_dvec4 dvec4_random_noise(double mean, double amplitude)
{
    m_dvec4 res;
    dvecn_random_noise(res.v, mean, amplitude, 4);
    return res;
}

/** dst = mean +- amplitude * rand() */
O_INLINE
m_dvec4 dvec4_random_noise_vec(m_dvec4 mean, m_dvec4 amplitude)
{
    m_dvec4 res;
    dvecn_random_noise_vec(res.v, mean.v, amplitude.v, 4);
    return res;
}

/** dst = mean +- sigma * normalized_rand() */
O_INLINE
m_dvec4 dvec4_random_normal(double mean, double sigma)
{
    m_dvec4 res;
    dvecn_random_normal(res.v, mean, sigma, 4);
    return res;
}

/** dst = mean +- sigma * normalized_rand() */
O_INLINE
m_dvec4 dvec4_random_normal_vec(m_dvec4 mean, m_dvec4 sigma)
{
    m_dvec4 res;
    dvecn_random_normal_vec(res.v, mean.v, sigma.v, 4);
    return res;
}

#endif // M_UTILS_DBL_DRANDOM
