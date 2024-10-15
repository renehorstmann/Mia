#ifndef M_UTILS_DBL_COLOR_H
#define M_UTILS_DBL_COLOR_H

/**
 * @file utils/dcolor.h
 *
 * rgb <> hsv conversion
 *
 *
 * Implementation source from git gist: https://gist.github.com/983/e170a24ae8eba2cd174f:
 *       // because http://lolengine.net/blog/2013/07/27/rgb-to-hsv-in-glsl is often down
 *
 *      vec3 rgb2hsv(vec3 c)
 *      {
 *          vec4 K = vec4(0.0, -1.0 / 3.0, 2.0 / 3.0, -1.0);
 *          vec4 p = mix(vec4(c.bg, K.wz), vec4(c.gb, K.xy), step(c.b, c.g));
 *          vec4 q = mix(vec4(p.xyw, c.r), vec4(c.r, p.yzx), step(p.x, c.r));
 *
 *          float d = q.x - min(q.w, q.y);
 *          float e = 1.0e-10;
 *          return vec3(abs(q.z + (q.w - q.y) / (6.0 * d + e)), d / (q.x + e), q.x);
 *      }
 *
 *      vec3 hsv2rgb(vec3 c)
 *      {
 *          vec4 K = vec4(1.0, 2.0 / 3.0, 1.0 / 3.0, 3.0);
 *          vec3 p = abs(fract(c.xxx + K.xyz) * 6.0 - K.www);
 *          return c.z * mix(K.xxx, clamp(p - K.xxx, 0.0, 1.0), c.y);
 *      }
 *
 */


#include "m/vec/dvec2.h"
#include "m/vec/dvec3.h"
#include "m/vec/dvec4.h"

//
// unsafe versions, from the implementation above
//

/**
 * all components are in the range [0:1] including hue.
 */
O_INLINE
m_dvec3 dvec3_rgb2hsv_unsafe(m_dvec3 rgb)
{
    m_dvec4 bg_part = (m_dvec4) {{rgb.b, rgb.g, -1.0, 2.0 / 3.0}};
    m_dvec4 gb_part = (m_dvec4) {{rgb.g, rgb.b, 0.0, -1.0 / 3.0}};
    m_dvec4 bga = dvec4_mix(bg_part, gb_part, md_step(rgb.b, rgb.g));
    // anchor of components b and g

    m_dvec4 bga_r_part = (m_dvec4) {{bga.v0, bga.v1, bga.v3, rgb.r}};
    m_dvec4 r_bga_part = (m_dvec4) {{rgb.r, bga.v1, bga.v2, bga.v0}};
    m_dvec4 anchor = dvec4_mix(bga_r_part, r_bga_part, md_step(bga.v0, rgb.r));

    // value is anchor .v0
    float d = anchor.v0 - md_min(anchor.v3, anchor.v1);
    float eps = 1.0e-10;

    m_dvec3 hsv;
    hsv.v0 = md_abs(anchor.v2 + (anchor.v3 - anchor.v1) / (6.0 * d + eps));
    hsv.v1 = d / (anchor.v0 + eps);
    hsv.v2 = anchor.v0;
    return hsv;
}

/**
 * hue will be "mod" internally. (so 5.6 -> 0.6, or -0.1 -> 0.9).
 * all components are in the range [0:1] including hue.
 **/
O_INLINE
m_dvec3 dvec3_hsv2rgb_unsafe(m_dvec3 hsv)
{
    // no need to add 1.0 on hsv.v0 for the r component, will get discarded on fract...
    dvec3 rgb_anchor = {{hsv.v0, hsv.v0 + 2.0 / 3.0, hsv.v0 + 1.0 / 3.0}};
    rgb_anchor = dvec3_fract(rgb_anchor);
    rgb_anchor = dvec3_scale(rgb_anchor, 6.0);
    rgb_anchor = dvec3_sub(rgb_anchor, 3.0);
    rgb_anchor = dvec3_abs(rgb_anchor);

    rgb_anchor = dvec3_sub(rgb_anchor, 1.0);
    dvec3 rgb_saturated = dvec3_clamp(rgb_anchor, 0.0, 1.0);
    // with full saturation
    dvec3 rgb_valued = dvec3_mix(dvec3_set(1.0), rgb_saturated, hsv.v1);
    // with full value
    dvec3 rgb = dvec3_scale(rgb_valued, hsv.v2);
    return rgb;
}

//
// safer versions
//

/**
 * rgb will be clamped to [0:1]
 * all components are in the range [0:1] including hue.
 **/
O_INLINE
m_dvec3 dvec3_rgb2hsv(m_dvec3 rgb)
{
    rgb = dvec3_clamp(rgb, 0.0, 1.0);
    return dvec3_rgb2hsv_unsafe(rgb);
}

/**
 * hue will be "mod" internally. (so 5.6 -> 0.6, or -0.1 -> 0.9).
 * saturation and value will be clamped to [0:1]
 * all components are in the range [0:1] including hue.
 **/
O_INLINE
m_dvec3 dvec3_hsv2rgb(m_dvec3 hsv)
{
    hsv.yz = dvec2_clamp(hsv.yz, 0.0, 1.0);
    return dvec3_hsv2rgb_unsafe(hsv);
}


//
// dvec4 versions with an alpha copy
//

/**
 * all components are in the range [0:1] including hue.
 **/
O_INLINE
m_dvec4 dvec4_rgb2hsv_unsafe(m_dvec4 rgba)
{
    dvec4 hsva;
    hsva.xyz = dvec3_rgb2hsv_unsafe(rgba.xyz);
    hsva.a = rgba.a;
    return hsva;
}

/**
 * rgb will be clamped to [0:1]
 * all components are in the range [0:1] including hue.
 **/
O_INLINE
m_dvec4 dvec4_rgb2hsv(m_dvec4 rgba)
{
    dvec4 hsva;
    hsva.xyz = dvec3_rgb2hsv(rgba.xyz);
    hsva.a = rgba.a;
    return hsva;
}

/**
 * hue will be "mod" internally. (so 5.6 -> 0.6, or -0.1 -> 0.9).
 * all components are in the range [0:1] including hue.
 * Alpha version, which will just be copied
 **/
O_INLINE
m_dvec4 dvec4_hsv2rgb_unsafe(m_dvec4 hsva)
{
    dvec4 rgba;
    rgba.xyz = dvec3_hsv2rgb_unsafe(hsva.xyz);
    rgba.a = hsva.a;
    return rgba;
}


/**
 * hue will be "mod" internally. (so 5.6 -> 0.6, or -0.1 -> 0.9).
 * saturation and value will be clamped to [0:1]
 * all components are in the range [0:1] including hue.
 * Alpha version, which will just be copied
 **/
O_INLINE
m_dvec4 dvec4_hsv2rgb(m_dvec4 hsva)
{
    dvec4 rgba;
    rgba.xyz = dvec3_hsv2rgb(hsva.xyz);
    rgba.a = hsva.a;
    return rgba;
}


#endif //M_UTILS_DCL_COLOR_H
