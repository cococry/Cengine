#version 460 core

layout(location = 0) in vec3 a_position;
layout(location = 1) in vec4 a_color;
layout(location = 2) in vec2 a_texcoord;
layout(location = 3) in vec2 a_sprite_props; // spriteProps.x => using textures spriteProps.y => render as overlay

uniform mat4 u_proj;
uniform mat4 u_view;

out vec4 v_color;
out vec2 v_texcoord;
out flat int v_using_textures;

void main() {
    v_color = a_color;
    v_texcoord = a_texcoord;
    v_using_textures = int(a_sprite_props.x);
    if(a_sprite_props.y == 0) 
        gl_Position = u_proj * u_view * vec4(a_position, 1.0);
    else
        gl_Position = u_proj * vec4(a_position, 1.0);

}