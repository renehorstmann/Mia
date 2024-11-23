#include "r/program.h"
#include "r/RProgram.h"
#include "o/ODelcallback.h"

static struct {
    oobj o;
} program_L;

O_STATIC
oobj L_o(void)
{
    if(!program_L.o) {
        program_L.o = OObj_new(r_root());
        OObj_name_set(program_L.o, "r_program");
    }
    return program_L.o;
}


void r_program__foo(void)
{
    // noop, just here to jump to src with an ide
}

#define r_program_DECL(file, num_draw_buffer)\
oobj r_program_ ## file (void)\
{\
    static RProgram *lazy = NULL;            \
    if(!lazy) {                              \
        lazy = RProgram_new_file(L_o(), "res/r/" #file ".glsl", NULL, (num_draw_buffer)); \
        ODelcallback_new_assert(lazy, "r_program", "deleted!");\
    }                                         \
    return lazy;\
}


r_program_DECL(Quad, 1)
r_program_DECL(Quad_addscaled, 1)
r_program_DECL(Quad_color, 1)
r_program_DECL(Quad_color_hsva, 1)
r_program_DECL(Quad_hsva_from_rgba, 1)
r_program_DECL(Quad_hsva_into_rgba, 1)
r_program_DECL(Quad_mixer, 1)
r_program_DECL(QuadChannelmerge, 1)
r_program_DECL(QuadDab, 2)
r_program_DECL(QuadKernel_contour, 1)
r_program_DECL(QuadKernel_conv, 1)
r_program_DECL(QuadKernel_contour_inv, 1)
r_program_DECL(QuadMerge, 1)
r_program_DECL(Rect, 1)
r_program_DECL(Rect_color, 1)
