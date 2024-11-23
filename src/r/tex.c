#include "r/tex.h"
#include "r/RTex.h"
#include "o/ODelcallback.h"

static struct {
    oobj o;
} tex_L;

O_STATIC
oobj tex_L_o(void)
{
    if (!tex_L.o) {
        tex_L.o = OObj_new(r_root());
        OObj_name_set(tex_L.o, "r_tex");
    }
    return tex_L.o;
}



//
// public
//

oobj r_tex_white(void)
{
    static RTex *lazy = NULL;
    if (!lazy) {
        bvec4 pixel = bvec4_(255);
        lazy = RTex_new(tex_L_o(), &pixel, 1, 1);
        ODelcallback_new_assert(lazy, "r_tex_white", "deleted!");
    }
    return lazy;
}

oobj r_tex_kernel_block2(bool normalized)
{
    static RTex *lazy = NULL;
    static RTex *lazy_n = NULL;
    if (normalized) {
        if (!lazy_n) {
            lazy_n = RTex_new_kernel(tex_L_o(), 2, 2, 1.0f, false, true);
            ODelcallback_new_assert(lazy_n, "r_tex_kernel_block2", "deleted!");
        }
        return lazy_n;
    }
    if (!lazy) {
        lazy = RTex_new_kernel(tex_L_o(), 2, 2, 1.0f, false, false);
        ODelcallback_new_assert(lazy, "r_tex_kernel_block2", "deleted!");
    }
    return lazy;
}


oobj r_tex_kernel_block3(bool normalized)
{
    static RTex *lazy = NULL;
    static RTex *lazy_n = NULL;
    if (normalized) {
        if (!lazy_n) {
            lazy_n = RTex_new_kernel(tex_L_o(), 3, 3, 1.0f, false, true);
            ODelcallback_new_assert(lazy_n, "r_tex_kernel_block3_n", "deleted!");
        }
        return lazy_n;
    }
    if (!lazy) {
        lazy = RTex_new_kernel(tex_L_o(), 3, 3, 1.0f, false, false);
        ODelcallback_new_assert(lazy, "r_tex_kernel_block3", "deleted!");
    }
    return lazy;
}


oobj r_tex_kernel_plus3(bool normalized)
{
    static RTex *lazy = NULL;
    static RTex *lazy_n = NULL;
    if (normalized) {
        if (!lazy_n) {
            lazy_n = RTex_new_kernel(tex_L_o(), 3, 3, 1.0f, true, true);
            ODelcallback_new_assert(lazy_n, "r_tex_kernel_plus3_n", "deleted!");
        }
        return lazy_n;
    }
    if (!lazy) {
        lazy = RTex_new_kernel(tex_L_o(), 3, 3, 1.0f, true, false);
        ODelcallback_new_assert(lazy, "r_tex_kernel_plus3", "deleted!");
    }
    return lazy;
}


oobj r_tex_kernel_gauss9(bool normalized)
{
    static RTex *lazy = NULL;
    static RTex *lazy_n = NULL;
    if (normalized) {
        if (!lazy_n) {
            lazy_n = RTex_new_kernel_gauss(tex_L_o(), 9, 9, vec2_(-1), true);
            ODelcallback_new_assert(lazy_n, "r_tex_kernel_gauss9_n", "deleted!");
        }
        return lazy_n;
    }
    if (!lazy) {
        lazy = RTex_new_kernel_gauss(tex_L_o(), 9, 9, vec2_(-1), false);
        ODelcallback_new_assert(lazy, "r_tex_kernel_gauss9", "deleted!");
    }
    return lazy;
}

//
// files
//


#define r_tex_DECL(file)\
oobj r_tex_ ## file (void)\
{\
    static RTex *lazy = NULL;                \
    if(!lazy) {                              \
        lazy = RTex_new_file(tex_L_o(), "res/r/" #file ".png"); \
        ODelcallback_new_assert(lazy, "r_tex_" #file , "deleted!");\
    }                                        \
    return lazy;\
}


r_tex_DECL(font35)
r_tex_DECL(font35_shadow)

