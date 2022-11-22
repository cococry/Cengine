#pragma once


#include "../core/defines.h"
#include "render_defines.h"

typedef struct shader_program {
    render_id id;
} shader_program;

shader_program* shader_program_create(const char* vertex_filepath, const char* fragment_filepath);

void shader_program_bind(shader_program* program);

void shader_program_unbind(shader_program* program);

void shader_program_delete(shader_program* program);

void _link_gl_shader_program(shader_program* program, render_id vertex_shader, render_id fragment_shader);

render_id _compile_gl_shader(const char* shader_source, u32 type);