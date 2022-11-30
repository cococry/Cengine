#include "renderer2d.h"

#include "../core/logging.h"
#include "../core/global_state.h"

#include "../platform/opengl/ext/glcorearb.h"

#include "../platform/opengl/gl_functions.h"

#include <Windows.h>

#include <stdlib.h>

#include "shader.h"

static renderer2d_state s_state;

#define MAX_QUADS 10000

void renderer2d_init() {
    s_state.quads = malloc(sizeof(quad) * MAX_QUADS);
    s_state.quad_count = 0;

    g_state->app->shader = shader_program_create("../engine/assets/shaders/default_vertex.glsl", "../engine/assets/shaders/default_fragment.glsl");
    shader_program_bind(g_state->app->shader);

    shader_program_upload_mat4(g_state->app->shader, "u_proj",
                               orthographic_matrix(0.0f, 1280.0f, 0.0f, 720.0f));

    // glEnable(GL_BLEND);
    // glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void renderer2d_terminate() {
    for (u32 i = 0; i < s_state.quad_count; i++) {
        quad_delete(s_state.quads[i]);
        s_state.quads[i] = nullptr;
    }
    shader_program_delete(g_state->app->shader);
}

void renderer2d_add_quad(quad* obj) {
    s_state.quads[s_state.quad_count++] = obj;
}

void renderer2d_remove_quad(quad* obj) {
    ASSERT_MSG(renderer2d_get_quad_index(obj) != -1, "Tried to remove non existent quad from renderer 2D.");

    quad_delete(obj);
    free(s_state.quads[renderer2d_get_quad_index(obj)]);
    s_state.quads[renderer2d_get_quad_index(obj)] = nullptr;
}

void renderer2d_render_quads() {
    shader_program_bind(g_state->app->shader);
    for (u32 i = 0; i < s_state.quad_count; i++) {
        if (s_state.quads[i] != nullptr) {
            quad_render(*s_state.quads[i]);
        }
    }
}

i32 renderer2d_get_quad_index(quad* obj) {
    for (u32 i = 0; i < s_state.quad_count; i++) {
        if (obj->tag == s_state.quads[i]->tag) {
            return i;
        }
    }
    return -1;
}

quad* renderer2d_get_quad_by_tag(const char* tag) {
    for (u32 i = 0; i < s_state.quad_count; i++) {
        if (tag == s_state.quads[i]->tag) {
            return s_state.quads[i];
        }
    }
    return nullptr;
}
renderer2d_state renderer2d_get_state() {
    return s_state;
}