#ifdef MIA_SHADER_VERTEX

layout(location = 0) in mat4 in_pose;
// uses location [0:3] (for each col)

layout(location = 4) in mat4 in_uv;
// uses location [4:7] (for each col)

layout(location = 8) in vec4 in_s;
layout(location = 9) in vec4 in_t;
layout(location = 10) in vec4 in_u;
layout(location = 11) in vec4 in_v;

out vec2 v_tex_coord;
out vec2 v_position;
flat out vec4 v_s;
flat out vec4 v_t;
flat out vec4 v_u;
flat out vec4 v_v;

uniform mat4 u_vp;

uniform vec2 u_tex_scale;

//
//// common
//////
uniform float u_c_viewport_scale_double;
uniform vec2 u_c_viewport_size_half;
uniform vec2 u_c_viewport_even_offset;
const vec4 c_quad_vertices[4] = vec4[](
vec4(-1.0, +1.0, 0, 1),
vec4(+1.0, +1.0, 0, 1),
vec4(-1.0, -1.0, 0, 1),
vec4(+1.0, -1.0, 0, 1)
);
// rounds the pose center to half a pixel (thats why _double == 2*viewport_scale)
mat4 c_quad_pose_round_center(mat4 pose)
{
    pose[3][0] = (0.1 + round(pose[3][0] * u_c_viewport_scale_double)) / u_c_viewport_scale_double;
    pose[3][1] = (0.1 + round(pose[3][1] * u_c_viewport_scale_double)) / u_c_viewport_scale_double;
    return pose;
}
// basic pose to vertex transformation
vec4 c_quad_pose_vertex_transform(mat4 vp, mat4 pose)
{
    return vp * pose * c_quad_vertices[gl_VertexID];
}
// a vertex to be exactly on a pixel
vec4 c_quad_vertex_round(vec4 vertex)
{
    vertex.xy = (round(vertex.xy * u_c_viewport_size_half) - u_c_viewport_even_offset) / u_c_viewport_size_half;
    return vertex;
}
// combines basic transformation to generate a vertex from a pose on an exact pixel
vec4 c_quad_vertex(mat4 vp, mat4 pose)
{
    pose = c_quad_pose_round_center(pose);
    vec4 vertex = c_quad_pose_vertex_transform(vp, pose);
    vertex = c_quad_vertex_round(vertex);
    return vertex;
}
// basic pose to tex_coord transformation
vec2 c_quad_tex_coord(mat4 uv, vec2 tex_scale)
{
    vec2 tex_coord = (uv * c_quad_vertices[gl_VertexID]).xy;
    tex_coord = tex_coord * tex_scale + vec2(0.5);
    return tex_coord;
}
//////
//// end common
//

void main() {
    gl_Position = c_quad_vertex(u_vp, in_pose);
    v_tex_coord = c_quad_tex_coord(in_uv, u_tex_scale);
    v_position = c_quad_vertices[gl_VertexID].xy;

    v_s = in_s;
    v_t = in_t;
    v_u = in_u;
    v_v = in_v;
}
#endif


#ifdef MIA_SHADER_FRAGMENT

in vec2 v_tex_coord;
in vec2 v_position;
flat in vec4 v_s;
flat in vec4 v_t;
flat in vec4 v_u;
flat in vec4 v_v;

layout(location = 0) out vec4 f_rgba;
layout(location = 1) out vec4 f_mask;

uniform sampler2D u_tex;
uniform sampler2D u_pattern;

uniform vec2 u_pattern_size_inv;
uniform vec2 u_pattern_offset;

uniform vec4 u_s;
uniform vec4 u_t;
uniform vec4 u_u;
uniform vec4 u_v;

//
//// common
//////
// All components are in the range [0:1], including hue.
vec3 c_rgb2hsv_unsafe(vec3 c)
{
    vec4 K = vec4(0.0, -1.0 / 3.0, 2.0 / 3.0, -1.0);
    vec4 p = mix(vec4(c.bg, K.wz), vec4(c.gb, K.xy), step(c.b, c.g));
    vec4 q = mix(vec4(p.xyw, c.r), vec4(c.r, p.yzx), step(p.x, c.r));

    float d = q.x - min(q.w, q.y);
    float e = 1.0e-10;
    return vec3(abs(q.z + (q.w - q.y) / (6.0 * d + e)), d / (q.x + e), q.x);
}
// All components are in the range [0:1], including hue.
vec3 c_hsv2rgb_unsafe(vec3 c)
{
    vec4 K = vec4(1.0, 2.0 / 3.0, 1.0 / 3.0, 3.0);
    vec3 p = abs(fract(c.xxx + K.xyz) * 6.0 - K.www);
    return c.z * mix(K.xxx, clamp(p - K.xxx, 0.0, 1.0), c.y);
}
// All components are in the range [0:1], including hue.
vec3 c_rgb2hsv(vec3 rgb)
{
    rgb = clamp(rgb, vec3(0.0), vec3(1.0));
    return c_rgb2hsv_unsafe(rgb);
}
// All components are in the range [0..1], including hue.
vec3 c_hsv2rgb(vec3 hsv)
{
    // hue already mod / fract
    hsv.yz = clamp(hsv.yz, vec2(0.0), vec2(1.0));
    return c_hsv2rgb_unsafe(hsv);
}
//
// rgba wrapper
//
// All components are in the range [0:1], including hue.
vec4 c_rgba2hsva_unsafe(vec4 rgba)
{
    return vec4(c_rgb2hsv_unsafe(rgba.rgb), rgba.a);
}
// All components are in the range [0:1], including hue.
vec4 c_hsva2rgba_unsafe(vec4 hsva)
{
    return vec4(c_hsv2rgb_unsafe(hsva.rgb), hsva.a);
}
// All components are in the range [0:1], including hue.
vec4 c_rgba2hsva(vec4 rgba)
{
    return vec4(c_rgb2hsv(rgba.rgb), rgba.a);
}
// All components are in the range [0:1], including hue.
vec4 c_hsva2rgba(vec4 hsva)
{
    return vec4(c_hsv2rgb(hsva.rgb), hsva.a);
}
//////
//// end common
//

void main() {
    vec4 rgba = texture(u_tex, v_tex_coord);

    // pattern tex coordination
    vec2 pattern_coord = (gl_FragCoord.xy + u_pattern_offset) * u_pattern_size_inv;
    vec4 pattern = texture(u_pattern, pattern_coord);

    //pattern.r = gl_FragCoord.x / textureSize(u_pattern, 0).x;
    //pattern.g = 0.0;
    //pattern.b = 0.0;
    //pattern.a = 1.0;

    // apply pattern
    rgba *= pattern;

    float dist_sqr = v_position.x * v_position.x + v_position.y * v_position.y;
    // dist_sqr max is 2.0 (edges), so recuding to [0:1]
    //dist_sqr *= 0.5;

    // calc opacity from hardness
    float hardness = v_v.x * u_v.x;
    float opacity = 1.0;
    if(dist_sqr < hardness) {
        opacity = mix(1.0, hardness, dist_sqr/hardness);
    } else if(hardness<1.0){
        opacity = mix(hardness, 0.0, min(1.0, (dist_sqr-hardness) / (1.0-hardness)));
    }

    // create mask
    float mask = rgba.a * opacity * v_v.y * u_v.y;

    // resulting alpha of the main rgba texture, should be 0 or 1.
    // alpha is copied to mask
    // dabs alpha is set by stuv
    rgba.a = step(0.0001, rgba.a);

    // coloring
    rgba = rgba * v_s * u_s;
    vec4 hsva = c_rgba2hsva(rgba);
    hsva = hsva + v_t + u_t;
    rgba = c_hsva2rgba(hsva);

    // results
    f_rgba = rgba;
    f_mask = vec4(1.0, 1.0, 1.0, mask);
}

#endif
