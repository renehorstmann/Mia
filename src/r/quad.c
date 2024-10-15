#include "r/quad.h"
#include "m/flt.h"


struct r_quad r_quad_new(int tex_cols, int tex_rows)
{
    // minimum 1 to create a valid size
    tex_cols = m_max(1, tex_cols);
    tex_rows = m_max(1, tex_rows);

    struct r_quad q;
    // pose has a position of (0, 0) and a size equal to the tex size
    q.pose = mat4_eye();
    q.pose.m00 = (float) tex_cols / 2.0f;
    q.pose.m11 = (float) tex_rows / 2.0f;
    // uv maps the whole tex, so as the pose:
    //      position of (0, 0) and a size equal to the tex size
    q.uv = q.pose;
    q.s = vec4_(1);
    q.t = vec4_(0);
    q.u = vec4_(0);
    q.v = vec4_(0);
    return q;
}
