#version 460 core

out vec4 o_color;

uniform vec4 uColor;

void main() {
    o_color = uColor;
}