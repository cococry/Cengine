#include "quad2d.h"

#include "render_buffers.h"

#include "../core/global_state.h"
#include "render_command.h"
#include "asset_pool.h"

#include "../core/logging.h"

#include <stdlib.h>

quad* quad_create(const char* tag, vector2 position, vector2 scale, float rotation, vector4 color) {
    quad* ret = malloc(sizeof(quad));
    ret->tag = tag;
    ret->position = position;
    ret->scale = scale;
    ret->rotation = rotation;
    ret->color = color;
    ret->texture = nullptr;

    float vertices[] = {
        -0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
        -0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
        0.5f, 0.5f, 0.0f, 1.0f, 1.0f,
        0.5f, -0.5f, 0.0f, 1.0f, 0.0f};

    u32 indices[] = {0, 1, 2, 2, 3, 0};

    ret->va = vertex_array_create();

    vertex_buffer* vb = vertex_buffer_create(vertices, sizeof(vertices), CNGN_STATIC_DRAW, 2);

    vertex_buffer_add_layout_attribute(vb, vertex_layout_attribute_create(vertex_layout_attrib_type_vector3f));
    vertex_buffer_add_layout_attribute(vb, vertex_layout_attribute_create(vertex_layout_attrib_type_vector2f));

    index_buffer* ib = index_buffer_create(indices, 6, CNGN_STATIC_DRAW);

    vertex_array_add_vertex_buffer(ret->va, vb);
    vertex_array_set_index_buffer(ret->va, ib);

    return ret;
}

void quad_load_texture(quad* quad, const char* texture_filepath) {
    quad->texture = asset_pool_load_texture(texture_filepath);
    shader_program_upload_int(g_state->app->shader, "uTexture", 0);
}

void quad_delete(quad* quad) {
    vertex_array_delete(quad->va);
    free(quad);
}

void quad_render(quad quad) {
    if (quad.texture != nullptr) {
        texture2d_bind(quad.texture);
        texture2d_bind_unit(quad.texture, 0);
        shader_program_upload_int(g_state->app->shader, "u_using_textures", 1);
    } else {
        shader_program_upload_int(g_state->app->shader, "u_using_textures", 0);
    }
    matrix4 model_matrix =
        matrix4_multiply(
            translate_mv(matrix4_identity(), vector3_create(quad.position.x / 100.0f, quad.position.y / 100.0f, 0.0f)),
            scale_mv(matrix4_identity(), vector3_create(quad.scale.x, quad.scale.y, 1.0f)));

    shader_program_upload_vec4(g_state->app->shader, "u_color", quad.color);
    shader_program_upload_mat4(g_state->app->shader, "u_model", model_matrix);

    render_command_draw_indexed(quad.va);
}

void quad_move_x(quad* quad, float x) {
    quad->position.x += x * g_state->app->wnd->props.width / 10.0f * g_state->app->state.delta_time;
}

void quad_move_y(quad* quad, float y) {
    quad->position.y += y * g_state->app->wnd->props.width / 10.0f * g_state->app->state.delta_time;
}
