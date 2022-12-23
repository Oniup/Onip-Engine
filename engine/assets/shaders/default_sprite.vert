#version 450 core

#define MAX_MODEL_MATRICES_SIZE 500

layout (location = 0) in vec3 a_position;
layout (location = 1) in vec2 a_uv;
layout (location = 2) in vec3 a_normals;
layout (location = 3) in float a_transform_id;
layout (location = 4) in vec2 a_diffuse_texture_range;
layout (location = 5) in vec2 a_specular_texture_range;
layout (location = 6) in vec2 a_ambient_texture_range;
layout (location = 7) in float a_specular_shininess;

uniform mat4 model_matrices[MAX_MODEL_MATRICES_SIZE];
uniform mat4 projection_matrix;
uniform mat4 view_matrix;

void main() {
    gl_Position = vec4(a_position.xyz, 1.0);
}