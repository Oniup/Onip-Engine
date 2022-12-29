#version 450 core

out vec4 frag_color;
in vec4 overlay_color;

void main() {
    frag_color = overlay_color;
}