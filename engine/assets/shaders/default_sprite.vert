#version 450 core

#define SOUND_OFFSET 32

struct Material {
    vec3 normal;
    vec2 uv;
    float textureId;
};

layout (location = 0) in vec3 a_Position;
layout (location = 1) in vec3 a_Normal;
layout (location = 2) in vec2 a_Uv;
layout (location = 3) in float a_TextureId;

// out Material o_Material;

void main() {
    gl_Position = vec4(a_Position.xyz, 1.0);
}