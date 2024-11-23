#ifdef MIA_SHADER_VERTEX

layout(location = 0) in vec4 in_rect;
layout(location = 1) in vec4 in_uv_rect;
layout(location = 2) in vec4 in_s;
layout(location = 3) in vec4 in_t;

out vec2 v_tex_coord;
flat out vec4 v_s;
flat out vec4 v_t;

uniform mat4 u_vp;

uniform vec2 u_tex_scale;

//
//// common
//////
uniform float u_c_viewport_scale_double;
uniform vec2 u_c_viewport_size_half;
uniform vec2 u_c_viewport_even_offset;
const vec2 c_rect_vertices[4] = vec2[](
vec2(-0.5, +0.5),
vec2(+0.5, +0.5),
vec2(-0.5, -0.5),
vec2(+0.5, -0.5)
);
// rounds the rect center to half a pixel (thats why _double == 2*viewport_scale)
vec4 c_rect_rect_round_center(vec4 rect)
{
    rect.x = (0.1 + round(rect.x * u_c_viewport_scale_double)) / u_c_viewport_scale_double;
    rect.y = (0.1 + round(rect.y * u_c_viewport_scale_double)) / u_c_viewport_scale_double;
    return rect;
}
// basic rect to vertex transformation
vec4 c_rect_vertex_transform(mat4 vp, vec4 rect)
{
    vec4 vertex = vec4(rect.x + rect.z * c_rect_vertices[gl_VertexID].x,
    rect.y + rect.w * c_rect_vertices[gl_VertexID].y,
    0.0, 1.0);
    return vp * vertex;
}
// a vertex to be exactly on a pixel
vec4 c_rect_vertex_round(vec4 vertex)
{
    vertex.xy = (round(vertex.xy * u_c_viewport_size_half) - u_c_viewport_even_offset) / u_c_viewport_size_half;
    return vertex;
}
vec4 c_rect_vertex(mat4 vp, vec4 rect)
{
    rect = c_rect_rect_round_center(rect);
    vec4 vertex = c_rect_vertex_transform(vp, rect);
    vertex = c_rect_vertex_round(vertex);
    return vertex;
}

// basic rect to tex_coord transformation
vec2 c_rect_tex_coord(vec4 uv_rect, vec2 tex_scale)
{
    vec2 tex_coord = vec2(uv_rect.x + uv_rect.z * c_rect_vertices[gl_VertexID].x,
    uv_rect.y + uv_rect.w * c_rect_vertices[gl_VertexID].y);
    tex_coord = tex_coord * tex_scale + vec2(0.5);
    return tex_coord;
}
//////
//// end common
//




void main() {
    gl_Position = c_rect_vertex(u_vp, in_rect);
    v_tex_coord = c_rect_tex_coord(in_uv_rect, u_tex_scale);

    v_s = in_s;
    v_t = in_t;
}
#endif


#ifdef MIA_SHADER_FRAGMENT

in vec2 v_tex_coord;
flat in vec4 v_s;
flat in vec4 v_t;

layout(location = 0) out vec4 f_rgba;

uniform sampler2D u_tex;

uniform vec4 u_s;
uniform vec4 u_t;

void main() {
    vec4 rgba = texture(u_tex, v_tex_coord);

    f_rgba = rgba;
}

#endif
