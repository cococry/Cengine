#include "triangle2d.h"

#include "../math/matrix.h"
#include "../math/matrix_transform.h"

#include "render_buffers.h"
#include "asset_pool.h"

#include "../core/global_state.h"
#include "render_command.h"

#include <stdlib.h>

void default_triangle_update_callback(triangle* this) {
}

triangle* triangle_create(const char* tag, vector2 position, vector2 scale, float rotation, vector2 vert1,
                          vector2 vert2, vector2 vert3, vector4 color, u32 render_level) {
    triangle* ret = malloc(sizeof(triangle));

    ret->tag = tag;
    ret->position = position;
    ret->scale = scale;
    ret->rotation = rotation;
    ret->color = color;
    ret->texture = nullptr;
    ret->update_callback = default_triangle_update_callback;
    ret->render_level = render_level;

    float vertices[] = {
        vert1.x,
        vert1.y,
        0.0f,
        0.0f,
        0.0f,
        vert2.x,
        vert2.y,
        0.0f,
        0.5f,
        1.0f,
        vert3.x,
        vert3.y,
        0.0f,
        1.0f,
        0.0f,
    };
    u32 indices[] = {0, 1, 2, 2, 3, 0};

    ret->va = vertex_array_create();

    vertex_buffer vb = vertex_buffer_create(vertices, sizeof(vertices), CNGN_STATIC_DRAW, 2);

    vertex_buffer_add_layout_attribute(&vb, vertex_layout_attribute_create(vertex_layout_attrib_type_vector3f));
    vertex_buffer_add_layout_attribute(&vb, vertex_layout_attribute_create(vertex_layout_attrib_type_vector2f));

    index_buffer ib = index_buffer_create(indices, 6, CNGN_STATIC_DRAW);

    vertex_array_add_vertex_buffer(&ret->va, vb);
    vertex_array_set_index_buffer(&ret->va, ib);

    return ret;
}

void triangle_load_texture(triangle* obj, const char* texture_filepath) {
    obj->texture = asset_pool_load_texture(texture_filepath);
    shader_program_upload_int(g_state->app->shader, "uTexture", 0);
}

void triangle_delete(triangle* obj) {
    vertex_array_delete(&obj->va);
}

void triangle_render(triangle obj) {
    if (obj.texture != nullptr) {
        texture2d_bind(obj.texture);
        texture2d_bind_unit(obj.texture, 0);
        shader_program_upload_int(g_state->app->shader, "u_using_textures", 1);
    } else {
        shader_program_upload_int(g_state->app->shader, "u_using_textures", 0);
    }

    matrix4 model_matrix =
        matrix4_multiply(
            translate_mv(matrix4_identity(), vector3_create(obj.position.x / 100.0f, obj.position.y / 100.0f, 0.0f)),
            scale_mv(matrix4_identity(), vector3_create(obj.scale.x, obj.scale.y, 1.0f)));

    shader_program_upload_vec4(g_state->app->shader, "u_color", obj.color);
    shader_program_upload_mat4(g_state->app->shader, "u_model", model_matrix);

    render_command_draw_indexed(&obj.va);
}

void triangle_move_x(triangle* obj, float x) {
    obj->position.x += x * g_state->app->wnd->props.width / 10.0f * g_state->app->state.delta_time;
}

void triangle_move_y(triangle* obj, float y) {
    obj->position.y += y * g_state->app->wnd->props.width / 10.0f * g_state->app->state.delta_time;
}