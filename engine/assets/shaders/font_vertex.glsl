#version 460 core

layout(location = 0) in vec2 a_position; 
layout(location = 1) in vec2 a_texcoord; 

uniform mat4 u_proj;

out vec2 v_texcoord;

void main() {
    v_texcoord = a_texcoord;
    gl_Position = u_proj * vec4(a_position, 0.0, 1.0);
}