#version 450 core

#define MAX_DIFFUSE_TEXTURES    4
#define MAX_SPECULAR_TEXTURES   4
#define MAX_AMBIENT_TEXTURES    4

struct MaterialSamplers {
    // sampler2D ambient[MAX_AMBIENT_TEXTURES];
    sampler2D diffuse[MAX_DIFFUSE_TEXTURES];
    // sampler2D specular[MAX_SPECULAR_TEXTURES];
};

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

uniform MaterialSamplers u_material_samplers;

vec4 calcAmbient(vec4 base_color) {
    return base_color;
}

vec4 calcDiffuse(vec4 base_color) {
    vec4 diffuse = vec4(0.0);
    if (material_texture_indices.diffuse_range.y > 0) {
        int end = material_texture_indices.diffuse_range.x + material_texture_indices.diffuse_range.y;
        for (int i = material_texture_indices.diffuse_range.x; i < end; i++) {
            diffuse += texture(u_material_samplers.diffuse[i], material_coords.uv);
        }
    }

    return base_color * diffuse;
}

vec4 calcSpecular(vec4 base_color) {
    return base_color;
}

void main() {
    vec4 base_color = vec4(1.0);
    base_color = calcAmbient(base_color);
    base_color = calcDiffuse(base_color);
    base_color = calcSpecular(base_color);

    if (base_color.a > 0.0) {
        if (material_properties.overlay_color.a > 0.05) {
            vec4 overlay_color = base_color * material_properties.overlay_color; 
            base_color = base_color + overlay_color;
        }

        frag_color = base_color;
        return;
    }
    discard;
}