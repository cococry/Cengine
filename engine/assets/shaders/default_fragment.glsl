#version 460 core

out vec4 o_color;

uniform vec4 uColor;

uniform sampler2D uTexture;

in vec2 v_texcoord;

void main() {
    o_color = texture(uTexture, v_texcoord);
}