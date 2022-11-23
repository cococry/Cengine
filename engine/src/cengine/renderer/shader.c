#include "shader.h"

#include "../platform/opengl/ext/glcorearb.h"
#include "../platform/opengl/gl_functions.h"

#include "../core/util.h"

#include "../core/logging.h"

#include <stdlib.h>

shader_program* shader_program_create(const char* vertex_filepath, const char* fragment_filepath) {
    shader_program* ret = malloc(sizeof(shader_program));
    const char* vertex_source = get_file_content(vertex_filepath);
    const char* fragment_source = get_file_content(fragment_filepath);

    render_id vertex_shader = _compile_gl_shader(vertex_source, GL_VERTEX_SHADER);
    render_id fragment_shader = _compile_gl_shader(fragment_source, GL_FRAGMENT_SHADER);

    _link_gl_shader_program(ret, vertex_shader, fragment_shader);

    return ret;
}

void shader_program_bind(shader_program* program) {
    glUseProgram(program->id);
}

void shader_program_unbind(shader_program* program) {
    glUseProgram(0);
}

void shader_program_delete(shader_program* program) {
    glDeleteProgram(program->id);

    free(program);
}

void shader_program_upload_vec2(shader_program* program, const char* uniform_name, vector2 uniform_value) {
    glUniform2fv(glGetUniformLocation(program->id, uniform_name), 1, vector2_value_ptr(uniform_value));
}
void shader_program_upload_vec3(shader_program* program, const char* uniform_name, vector3 uniform_value) {
    glUniform3fv(glGetUniformLocation(program->id, uniform_name), 1, vector3_value_ptr(uniform_value));
}

void shader_program_upload_vec4(shader_program* program, const char* uniform_name, vector4 uniform_value) {
    glUniform4fv(glGetUniformLocation(program->id, uniform_name), 1, vector4_value_ptr(uniform_value));
}

void shader_program_upload_int(shader_program* program, const char* uniform_name, i32 uniform_value) {
    glUniform1i(glGetUniformLocation(program->id, uniform_name), uniform_value);
}

void shader_program_upload_float(shader_program* program, const char* uniform_name, float uniform_value) {
    glUniform1f(glGetUniformLocation(program->id, uniform_name), uniform_value);
}
void _link_gl_shader_program(shader_program* program, render_id vertex_shader, render_id fragment_shader) {
    int linking_success;
    char info_log[512];

    program->id = glCreateProgram();

    glAttachShader(program->id, vertex_shader);
    glAttachShader(program->id, fragment_shader);

    glLinkProgram(program->id);

    glGetProgramiv(program->id, GL_LINK_STATUS, &linking_success);

    if (!linking_success) {
        glGetProgramInfoLog(program->id, 512, nullptr, info_log);

        LOG_ERROR("Failed to link shader program.");
        LOG_TRACE("%s", info_log);
    } else {
        LOG_INFO("Successfully linked shader program (render id: %i)", program->id);
    }

    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);
}

render_id _compile_gl_shader(const char* shader_source, u32 type) {
    int compile_success;
    char info_log[512];

    render_id ret;

    ret = glCreateShader(type);
    glShaderSource(ret, 1, &shader_source, nullptr);
    glCompileShader(ret);

    glGetShaderiv(ret, GL_COMPILE_STATUS, &compile_success);

    char* shader_name = malloc(sizeof(char) * 15);
    switch (type) {
        case GL_VERTEX_SHADER:
            shader_name = "vertex";
            break;
        case GL_FRAGMENT_SHADER:
            shader_name = "fragment";
            break;
        default:
            shader_name = "unknown";
            break;
    }

    if (!compile_success) {
        glGetShaderInfoLog(ret, 512, nullptr, info_log);

        LOG_ERROR("Failed to compile %s shader.", shader_name);
        LOG_TRACE("%s", info_log);
    } else {
        LOG_INFO("Successfully compiled %s shader.", shader_name);
    }

    return ret;
}