#version 460 core

out vec4 o_color;

in vec2 v_texcoord;

uniform sampler2D u_texture;		

void main() {
    o_color = vec4(texture(u_texture, v_texcoord).a);
}