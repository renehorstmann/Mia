#include "o/img.h"
#include "m/flt.h"
#include "m/byte.h"
#include "r/RTex_manip.h"
#include "r/tex.h"

O_STATIC
void color(oobj obj)
{
    bvec4 src, dst;
    oobj img;
    
    src = bvec4_(250, 10, 107, 255);
    
    img = RTex_color(r_tex_white(), vec4_cast_byte_1(src.v), vec4_(0.0), R_FORMAT_RGBA_8);
    
    RTex_get(img, &dst);
    
    assert(bvec4_equals_v(src, dst));
    
}

O_STATIC
void outline_run(oobj obj, int cols, int rows)
{
    struct o_img src, dst;
    bvec4 *col;
    oobj img;
    ivec2 pos = {{cols/2, rows/2}};
    bvec4 outline = {{66, 55, 99, 255}};
    
    src = o_img_new0(obj, cols, rows);
    col = (bvec4*) o_img_at(src, m_2(pos));
    *col = bvec4_(10, 20, 30, 255);
    
    img = RTex_new(obj, src.rgba_data, cols, rows);
    img = RTex_outline(img, ivec2_(3), ivec2_(0), vec4_(0, 0, 0, 1), vec4_cast_byte_1(outline.v), R_FORMAT_RGBA_8);
    
    dst = o_img_new(obj, cols, rows);
    RTex_get(img, dst.rgba_data);
    
    //o_img_write_file(src, "debug.src.png");
    //o_img_write_file(dst, "debug.dst.png");
    
    // test dst
    assert(memcmp(o_img_at(dst, pos.x-1, pos.y), outline.v, sizeof outline) == 0);
    assert(memcmp(o_img_at(dst, pos.x+1, pos.y), outline.v, sizeof outline) == 0);
    assert(memcmp(o_img_at(dst, pos.x, pos.y-1), outline.v, sizeof outline) == 0);
    assert(memcmp(o_img_at(dst, pos.x, pos.y+1), outline.v, sizeof outline) == 0);
}

O_STATIC
void outline(oobj obj)
{
    outline_run(obj, 16, 16);
    outline_run(obj, 16, 33);
    outline_run(obj, 33, 16);
    outline_run(obj, 16, 49);
}

int RTex__test(oobj obj)
{
    color(obj);
    outline(obj);
    
    return 0;
}

