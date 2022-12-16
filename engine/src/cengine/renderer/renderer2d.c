#include "renderer2d.h"

#include "../core/logging.h"
#include "../core/global_state.h"

#include "../platform/opengl/ext/glcorearb.h"

#include "../platform/opengl/gl_functions.h"
#include "../core/event_system.h"

#include <Windows.h>

#include <stdlib.h>

#include "shader.h"

static renderer2d_state s_state;

#define MAX_SCENE_COUNT 32

void renderer2d_init() {
    s_state.scenes = malloc(sizeof(scene) * MAX_SCENE_COUNT);
    s_state.scene_count = 0;

    g_state->app->shader = shader_program_create("../engine/assets/shaders/default_vertex.glsl", "../engine/assets/shaders/default_fragment.glsl");

    shader_program_bind(g_state->app->shader);
    shader_program_upload_mat4(g_state->app->shader, "u_proj",
                               orthographic_matrix(0.0f, 1280.0f, 0.0f,
                                                   720.0f));

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void renderer2d_terminate() {
    for (u32 i = 0; i < s_state.scene_count; i++) {
        scene_delete(s_state.scenes[i]);
    }
    shader_program_delete(g_state->app->shader);
}

void renderer2d_add_scene(scene* scene) {
    s_state.scenes[s_state.scene_count++] = scene;
}

void renderer2d_set_active_scene(scene* scene) {
    s_state.active_scene = scene;
    s_state.scenes[s_state.scene_count++] = scene;
}

void renderer2d_switch_active_scene_by_name(const char* name) {
    if (s_state.active_scene->name == name) {
        return;
    }
    if (strcmp(renderer2d_get_scene_by_name(name)->name, "Invalid") == 0) {
        return;
    }
    s_state.active_scene = renderer2d_get_scene_by_name(name);
}

scene* renderer2d_get_scene_by_name(const char* name) {
    for (u32 i = 0; i < s_state.scene_count; i++) {
        if (s_state.scenes[i]->name == name) {
            return s_state.scenes[i];
        }
    }
    return scene_create_empty("Invalid");
}

void renderer2d_add_quad(quad* obj) {
    scene_add_quad(s_state.active_scene, obj);
}

void renderer2d_remove_quad(quad* obj) {
    scene_remove_quad(s_state.active_scene, obj);
}

void renderer2d_render_objects() {
    shader_program_bind(g_state->app->shader);
    scene_render(s_state.active_scene);
}

void renderer2d_update_objects() {
    scene_update(s_state.active_scene);
}

i32 renderer2d_get_quad_index(quad* obj) {
    return scene_get_quad_index(s_state.active_scene, obj);
}

quad* renderer2d_get_quad_by_tag(const char* tag) {
    return scene_get_quad_by_tag(s_state.active_scene, tag);
}

renderer2d_state renderer2d_get_state() {
    return s_state;
}

void renderer2d_add_triangle(triangle* obj) {
    scene_add_triangle(s_state.active_scene, obj);
}

void renderer2d_remove_triangle(triangle* obj) {
    scene_remove_triangle(s_state.active_scene, obj);
}

i32 renderer2d_get_triangle_index(triangle* obj) {
    return scene_get_triangle_index(s_state.active_scene, obj);
}

triangle* renderer2d_get_triangle_by_tag(const char* tag) {
    return scene_get_triangle_by_tag(s_state.active_scene, tag);
}

quad* renderer2d_get_quad_in_tilemap(tile_map map, vector2 tile_pos) {
    return scene_get_quad_in_tilemap(s_state.active_scene, map, tile_pos);
}