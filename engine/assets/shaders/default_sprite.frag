#version 450 core

#define MAX_DIFFUSE_TEXTURES    4
#define MAX_SPECULAR_TEXTURES   4
#define MAX_AMBIENT_TEXTURES    4

// struct MaterialSamplers {
//     sampler2D diffuse[MAX_DIFFUSE_TEXTURES];
//     sampler2D specular[MAX_SPECULAR_TEXTURES];
//     sampler2D ambient[MAX_AMBIENT_TEXTURES];
// };

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

out vec4 frag_color;

in flat MaterialTextureIndices material_texture_indices;
in MaterialCoords material_coords;
in MaterialProperties material_properties;

void main() {
    frag_color = material_properties.overlay_color;
}