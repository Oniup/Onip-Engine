#version 450 core

#define MAX_MODEL_MATRICES_SIZE 50
#define MAX_OVERLAY_COLOR_SIZE  50

layout (location = 0) in vec3 a_position;
layout (location = 1) in float a_overlay_color_index;
layout (location = 2) in float a_transform_id;
// layout (location = 1) in vec2 a_uv;
// layout (location = 2) in vec3 a_normals;
// layout (location = 4) in vec2 a_diffuse_texture_range;
// layout (location = 5) in vec2 a_specular_texture_range;
// layout (location = 6) in vec2 a_ambient_texture_range;
// layout (location = 7) in float a_specular_shininess;

uniform mat4 u_model_matrices[MAX_MODEL_MATRICES_SIZE];
uniform mat4 u_projection_matrix;
uniform mat4 u_view_matrix;
uniform vec4 u_overlay_colors[MAX_OVERLAY_COLOR_SIZE];

out vec4 overlay_color;

void main() {
    gl_Position = u_projection_matrix * u_view_matrix * u_model_matrices[int(a_transform_id)] * vec4(a_position.xyz, 1.0);

    overlay_color = u_overlay_colors[int(a_overlay_color_index)];
}