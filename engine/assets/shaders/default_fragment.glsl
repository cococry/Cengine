#version 460 core

out vec4 o_color;

uniform vec4 u_color;

uniform sampler2D uTexture;

in vec2 v_texcoord;

void main() {
    vec4 color = texture(uTexture, v_texcoord);
    color.r *= u_color.r;
    color.g *= u_color.g;
    color.b *= u_color.b;
    color.a *= u_color.a;
    
    o_color = color;

    if(color.a == 0.0) {
        discard;
    }
}