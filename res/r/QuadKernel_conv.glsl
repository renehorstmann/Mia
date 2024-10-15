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

    v_s = in_s;
    v_t = in_t;
    v_u = in_u;
    v_v = in_v;
}
#endif


#ifdef MIA_SHADER_FRAGMENT

in vec2 v_tex_coord;
flat in vec4 v_s;
flat in vec4 v_t;
flat in vec4 v_u;
flat in vec4 v_v;

layout(location = 0) out vec4 f_rgba;

uniform sampler2D u_tex;
uniform sampler2D u_kernel;

uniform vec4 u_s;
uniform vec4 u_t;
uniform vec4 u_u;
uniform vec4 u_v;

uniform ivec2 u_kernel_offset;// offset from the kernel center
uniform vec4 u_kernel_color;
uniform vec4 u_kernel_mask;// should be normalized


void main() {
    ivec2 tex_size = textureSize(u_tex, 0);
    vec2 tex_texel = 1.0 / vec2(tex_size);

    ivec2 kernel_size = textureSize(u_kernel, 0);
    vec2 kernel_texel = 1.0 / vec2(kernel_size);

    ivec2 pos = ivec2(v_tex_coord * vec2(tex_size));

    ivec2 offset = pos - u_kernel_offset - kernel_size/2;

    vec4 rgba = vec4(0.0);

    // traverse through the kernel
    for (int y=0; y<kernel_size.y; y++) {
        for (int x=0; x<kernel_size.x; x++) {
            
            // kernel and texture positions
            ivec2 kernel_pos = ivec2(x, y);
            ivec2 tex_pos = offset + kernel_pos;
            tex_pos = clamp(tex_pos, ivec2(0), tex_size - ivec2(1));
            
            // get kernel
            float kernel = texture(u_kernel, (vec2(kernel_pos)+vec2(0.1)) * kernel_texel).r;
            
            // get color
            vec4 tex_color = texture(u_tex, (vec2(tex_pos)+vec2(0.1)) * tex_texel);

            rgba = rgba + kernel * tex_color;
        }
    }

    rgba = rgba * u_kernel_color;

    f_rgba = rgba;
}

#endif
