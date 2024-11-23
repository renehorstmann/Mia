#include "app/mp/brush.h"
#include "o/OObj.h"
#include "r/common.h"
#include "r/tex.h"


static struct {
    oobj o;
    bvec4 color;
    oobj color_origin;

    oobj pattern;
    oobj pattern_origin;
} brush_L;


// protected, called by mp_main
O_EXTERN
oobj mp_brush__init(oobj parent)
{
    o_clear(&brush_L, sizeof brush_L, 1);

    brush_L.o = OObj_new(parent);
    brush_L.color = U_WHITE;

    brush_L.pattern = r_tex_white();

    return brush_L.o;
}


bvec4 mp_brush_color(void)
{
    return brush_L.color;
}

oobj mp_brush_color_origin(void)
{
    return brush_L.color_origin;
}

void mp_brush_color_set(bvec4 color, oobj origin)
{
    brush_L.color = color;
    brush_L.color_origin = origin;
}


oobj mp_brush_pattern(void)
{
    return brush_L.pattern;
}

oobj mp_brush_pattern_origin(void)
{
    return brush_L.pattern_origin;
}

void mp_brush_pattern_set(oobj pattern, oobj origin)
{
    brush_L.pattern = pattern;
    brush_L.pattern_origin = origin;
}
