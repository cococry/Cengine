#version 460 core

out vec4 o_color;

uniform vec4 u_color;

uniform sampler2D uTexture;

uniform int u_using_textures;

in vec2 v_texcoord;

void main() {
    vec4 color = vec4(1.0, 1.0, 1.0, 1.0);
    if(u_using_textures == 1) { 
        color = texture(uTexture, v_texcoord);
    }
    color.r *= u_color.r;
    color.g *= u_color.g;
    color.b *= u_color.b;
    color.a *= u_color.a;
    
    o_color = color;

}