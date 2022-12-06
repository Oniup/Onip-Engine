#version 450 core 

struct Material {
    vec3 normal;
    vec2 uv;
    float textureId;
};

out vec4 fragColor;

// in Material o_Material;

void main() {
    fragColor = vec4(1.0, 0.7, 0.3, 1.0);
}