#include "r/rect.h"
#include "m/flt.h"


struct r_rect r_rect_new(int tex_cols, int tex_rows)
{
    // minimum 1 to create a valid size
    tex_cols = m_max(1, tex_cols);
    tex_rows = m_max(1, tex_rows);

    struct r_rect q;
    // rect centered and a size equal to the tex size
    // integer division
    q.rect.x = 0.0f;
    q.rect.y = 0.0f;
    q.rect.v2 = (float) tex_cols;
    q.rect.v3 = (float) tex_rows;
    q.uv_rect = q.rect;
    q.s = vec4_(1);
    q.t = vec4_(0);
    return q;
}
