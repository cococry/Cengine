#version 460 core

out vec4 o_color;

in vec4 v_color;
in vec2 v_texcoord;
in flat int v_using_textures;
uniform sampler2D u_texture;		

void main() {
    if(v_using_textures == 1) {
        o_color = texture(u_texture, v_texcoord) * v_color;
    } else {
        o_color = v_color;
    }
}