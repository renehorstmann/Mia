#include "u/discr.h"
#include "o/OArray.h"
#include "m/vec/vec2.h"
#include "u/pose.h"

oobj u_discr_as_rects(oobj points_array, struct r_rect init)
{
    oobj res = OArray_new(points_array, NULL, sizeof(struct r_rect), o_num(points_array));
    for(osize i=0; i<o_num(res); i++) {
        const vec2 *src = o_at(points_array, i);
        struct r_rect *dst = o_at(res, i);
        *dst = init;
        dst->rect.x = src->x;
        dst->rect.y = src->y;
    }
    return res;
}

oobj u_discr_as_quads(oobj points_array, struct r_quad init)
{
    oobj res = OArray_new(points_array, NULL, sizeof(struct r_quad), o_num(points_array));
    for(osize i=0; i<o_num(res); i++) {
        const vec2 *src = o_at(points_array, i);
        struct r_quad *dst = o_at(res, i);
        *dst = init;
        u_pose_set_xy(&dst->pose, src->x, src->y);
    }
    return res;
}

void u_discr_point(oobj points_array, vec2 pt)
{
    OArray_push(points_array, &pt);
}

int u_discr_line(oobj points_array, vec2 a, vec2 b, float dist)
{
    vec2 ab = vec2_sub_v(b, a);
    float full_dist = vec2_length(ab);
    int samples = (int) m_ceil(full_dist/dist);
    samples = o_max(1, samples);
    // align dist for an equal density
    dist = full_dist / (float) samples;

    for(int i=0; i<samples; i++) {
        float t = (float) (i+1) / (float) samples;
        vec2 s = vec2_add_scaled(a, ab, t);
        u_discr_point(points_array, s);
    }

    return samples;
}

int u_discr_rect(oobj points_array, vec2 a, vec2 b, float dist)
{
    int sampled = 0;
    sampled += u_discr_line(points_array, vec2_(a.x, a.y), vec2_(b.x, a.y), dist);
    sampled += u_discr_line(points_array, vec2_(b.x, a.y), vec2_(b.x, b.y), dist);
    sampled += u_discr_line(points_array, vec2_(b.x, b.y), vec2_(a.x, b.y), dist);
    sampled += u_discr_line(points_array, vec2_(a.x, b.y), vec2_(a.x, a.y), dist);
    return sampled;
}


int u_discr_circle_bow(oobj points_array, vec2 center, float radius, float a_rad, float b_rad, float dist)
{
    float ab_rad = b_rad-a_rad;
    float full_dist = m_abs(radius * ab_rad);
    int samples = (int) m_ceil(full_dist/dist);
    samples = o_max(1, samples);
    // align dist for an equal density
    dist = full_dist / (float) samples;

    for(int i=0; i<samples; i++) {
        float t = (float) (i+1) / (float) samples;
        vec2 s = center;
        s.x += radius * m_cos(a_rad + ab_rad * t);
        s.y += radius * m_sin(a_rad + ab_rad * t);
        u_discr_point(points_array, s);
    }

    return samples;
}


int u_discr_bezier(oobj points_array, vec2 p0, vec2 p1, vec2 p2, float dist)
{
    int sampled = 0;
    float t = 0.0f;
    vec2 prev_point = p0;
    float step = 0.01f; // Initial step size

    while (t <= 1.0f) {
        float inv_t = 1.0f - t;
        // Quadratic Bezier formula
        vec2 point = vec2_add_v(
                vec2_add_v(vec2_scale(p0, inv_t * inv_t),
                         vec2_scale(p1, 2 * inv_t * t)),
                vec2_scale(p2, t * t));

        // Compute the distance from the previous point
        float segment_length = vec2_distance(prev_point, point);
        if (segment_length >= dist || t == 1.0f) {
            // Only add the point if the distance is sufficient or it's the last point
            u_discr_point(points_array, point);
            sampled++;
            prev_point = point;
            t += step;
        } else {
            // Increase the step to find the next suitable point
            t += step * (dist / segment_length);
        }
    }

    return sampled;
}


