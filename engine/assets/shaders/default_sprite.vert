#version 450 core

#define MAX_MODEL_MATRICES_SIZE 50
#define MAX_OVERLAY_COLOR_SIZE  50

struct MaterialTextureIndices {
    ivec2 diffuse_range;
    ivec2 specular_range;
    ivec2 ambient_range;
};

struct MaterialCoords {
    vec2 uv;
    vec3 normals;
};

struct MaterialProperties {
    vec4 overlay_color;
    float specular_shininess;
};

layout (location = 0) in vec3 a_position;
layout (location = 1) in float a_overlay_color_index;
layout (location = 2) in float a_transform_id;
layout (location = 3) in vec2 a_uv;
layout (location = 4) in vec2 a_diffuse_range;

uniform mat4 u_model_matrices[MAX_MODEL_MATRICES_SIZE];
uniform mat4 u_projection_matrix;
uniform mat4 u_view_matrix;
uniform vec4 u_overlay_colors[MAX_OVERLAY_COLOR_SIZE];

out flat MaterialTextureIndices material_texture_indices;
out MaterialCoords material_coords;
out MaterialProperties material_properties;

void pushDataToFrag() {
    material_texture_indices.diffuse_range = ivec2(a_diffuse_range);
    material_texture_indices.specular_range = ivec2(0);
    material_texture_indices.ambient_range = ivec2(0);

    material_coords.uv = a_uv;
    material_coords.normals = vec3(0.0);

    material_properties.specular_shininess = 0.0;
    material_properties.overlay_color = u_overlay_colors[int(a_overlay_color_index)];
}

void main() {
    gl_Position = u_projection_matrix * u_view_matrix * u_model_matrices[int(a_transform_id)] * vec4(a_position.xyz, 1.0);

    pushDataToFrag();
}