#include "u/gradient.h"
#include "m/vec/bvec4.h"
#include "m/types/flt.h"
#include "m/utils/color.h"

void u_gradient_hue(bvec4 *rgba_data, int size)
{
    for(int i=0; i<size; i++) {
        float hue = (float) i / (float) size;
        vec4 hsva = {{hue, 1, 1, 1}};
        vec4 rgba = vec4_hsv2rgb(hsva);
        rgba_data[i] = bvec4_cast_float_1(rgba.v);
    }
}

void u_gradient_sat(bvec4 *rgba_data, int size, float hue)
{
    for(int i=0; i<size; i++) {
        float sat = (float) i / (float) size;
        vec4 hsva = {{hue, sat, 1, 1}};
        vec4 rgba = vec4_hsv2rgb(hsva);
        rgba_data[i] = bvec4_cast_float_1(rgba.v);
    }
}

void u_gradient_val(bvec4 *rgba_data, int size, float hue, float sat)
{
    for(int i=0; i<size; i++) {
        float val = (float) i / (float) size;
        vec4 hsva = {{hue, sat, val, 1}};
        vec4 rgba = vec4_hsv2rgb(hsva);
        rgba_data[i] = bvec4_cast_float_1(rgba.v);
    }
}

void u_gradient_red(bvec4 *rgba_data, int size, float green, float blue)
{
    for(int i=0; i<size; i++) {
        float red = (float) i / (float) size;
        vec4 rgba = vec4_(red, green, blue, 1);
        rgba_data[i] = bvec4_cast_float_1(rgba.v);
    }
}

void u_gradient_green(bvec4 *rgba_data, int size, float red, float blue)
{
    for(int i=0; i<size; i++) {
        float green = (float) i / (float) size;
        vec4 rgba = vec4_(red, green, blue, 1);
        rgba_data[i] = bvec4_cast_float_1(rgba.v);
    }
}

void u_gradient_blue(bvec4 *rgba_data, int size, float red, float green)
{
    for(int i=0; i<size; i++) {
        float blue = (float) i / (float) size;
        vec4 rgba = vec4_(red, green, blue, 1);
        rgba_data[i] = bvec4_cast_float_1(rgba.v);
    }
}

void u_gradient_alpha(bvec4 *rgba_data, int size, float red, float green, float blue)
{
    for(int i=0; i<size; i++) {
        float alpha = (float) i / (float) size;
        vec4 rgba = vec4_(red, green, blue, alpha);
        rgba_data[i] = bvec4_cast_float_1(rgba.v);
    }
}
