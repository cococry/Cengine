#include "quad2d.h"

#include "render_buffers.h"

#include "../core/global_state.h"
#include "../core/event_system.h"
#include "render_command.h"
#include "asset_pool.h"

#include "../core/logging.h"

#include <stdlib.h>

void default_quad_update_callback(quad* this) {
}

quad* quad_create(const char* tag, vector2 position, vector2 scale, vector2 hit_box, float rotation, u32 render_level, vector4 color, subtexture2d* subtexture) {
    quad* ret = malloc(sizeof(quad));
    ret->tag = tag;
    ret->position = position;
    ret->scale = scale;
    ret->rotation = rotation;
    ret->color = color;
    ret->texture = nullptr;
    ret->update_callback = default_quad_update_callback;
    ret->sprite_anim = sprite_animation_create(nullptr, 0, 0.0f, vector2_create(0.0f, 0.0f), vector2_create(0.0f, 0.0f));
    ret->hit_box = hit_box;
    ret->render_level = render_level;
    ret->movement = vector2_create(0.0f, 0.0f);
    float vertices[] = {
        -0.5f, -0.5f, 0.0f, (subtexture == nullptr ? 0.0f : subtexture2d_get_texcoords(*subtexture).min.x),
        (subtexture == nullptr ? 0.0f : subtexture2d_get_texcoords(*subtexture).min.y),
        -0.5f, 0.5f, 0.0f, (subtexture == nullptr ? 0.0f : subtexture2d_get_texcoords(*subtexture).min.x),
        (subtexture == nullptr ? 1.0f : subtexture2d_get_texcoords(*subtexture).max.y),
        0.5f, 0.5f, 0.0f, (subtexture == nullptr ? 1.0f : subtexture2d_get_texcoords(*subtexture).max.x),
        (subtexture == nullptr ? 1.0f : subtexture2d_get_texcoords(*subtexture).max.y),
        0.5f, -0.5f, 0.0f, (subtexture == nullptr ? 1.0f : subtexture2d_get_texcoords(*subtexture).max.x),
        (subtexture == nullptr ? 0.0f : subtexture2d_get_texcoords(*subtexture).min.y)};

    u32 indices[] = {0, 1, 2, 2, 3, 0};

    ret->va = vertex_array_create();

    ret->vb = vertex_buffer_create(vertices, sizeof(vertices), CNGN_STATIC_DRAW, 2);

    vertex_buffer_add_layout_attribute(&ret->vb, vertex_layout_attribute_create(vertex_layout_attrib_type_vector3f));
    vertex_buffer_add_layout_attribute(&ret->vb, vertex_layout_attribute_create(vertex_layout_attrib_type_vector2f));

    index_buffer ib = index_buffer_create(indices, 6, CNGN_STATIC_DRAW);

    vertex_array_add_vertex_buffer(&ret->va, ret->vb);
    vertex_array_set_index_buffer(&ret->va, ib);

    if (subtexture != nullptr) {
        quad_load_texture(ret, subtexture->texture->filepath);
    }
    return ret;
}

void quad_load_texture(quad* quad, const char* texture_filepath) {
    quad->texture = asset_pool_load_texture(texture_filepath);
    shader_program_upload_int(g_state->app->shader, "uTexture", 0);
}

void quad_set_sprite_animation(quad* quad, sprite_animation anim) {
    if (quad->sprite_anim.texture_coords != nullptr && anim.texture_coords != quad->sprite_anim.texture_coords) {
        free(quad->sprite_anim.texture_coords);
    }
    quad->sprite_anim = anim;

    subtexture2d subtexture = subtexture2d_create(quad->texture, anim.texture_coords[0], quad->sprite_anim.frame_cell_size, quad->sprite_anim.frame_sprite_scale);
    subtexture_coords coords = subtexture2d_get_texcoords(subtexture);
    quad_change_texture_coords(quad, coords);
    quad_load_texture(quad, quad->sprite_anim.sprite_sheet->filepath);
    quad->scale = vector2_multiply(quad->scale, anim.frame_sprite_scale);
}

void quad_delete(quad* quad) {
    vertex_array_delete(&quad->va);
    free(quad);
}

void quad_render(quad* quad) {
    if (quad->sprite_anim.sprite_sheet != nullptr) {
        sprite_animation_tick(quad);
    }
    if (quad->texture != nullptr) {
        texture2d_bind(quad->texture);
        texture2d_bind_unit(quad->texture, 0);
        shader_program_upload_int(g_state->app->shader, "u_using_textures", true);
    } else {
        shader_program_upload_int(g_state->app->shader, "u_using_textures", false);
    }
    matrix4 model_matrix =
        matrix4_multiply(
            translate_mv(matrix4_identity(), vector3_create(quad->position.x / 100.0f, quad->position.y / 100.0f, 0.0f)),
            scale_mv(matrix4_identity(), vector3_create(quad->scale.x, quad->scale.y, 1.0f)));

    shader_program_upload_vec4(g_state->app->shader, "u_color", quad->color);
    shader_program_upload_mat4(g_state->app->shader, "u_model", model_matrix);
    render_command_draw_indexed(&quad->va);
}

void quad_move_x(quad* quad, float x) {
    quad->movement.x = (x > 0.0f) ? 1.0f : -1.0f;
    quad->movement.y = 0.0f;
    quad->position.x += x * 100.0f * g_state->app->state.delta_time;
}

void quad_move_y(quad* quad, float y) {
    quad->movement.y = (y > 0.0f) ? 1.0f : -1.0f;
    quad->movement.x = 0.0f;
    quad->position.y += y * 100.0f * g_state->app->state.delta_time;
}

void quad_change_texture_coords(quad* quad, subtexture_coords coords) {
    float vertices[] = {
        -0.5f, -0.5f, 0.0f, coords.min.x, coords.min.y,
        -0.5f, 0.5f, 0.0f, coords.min.x, coords.max.y,
        0.5f, 0.5f, 0.0f, coords.max.x, coords.max.y,
        0.5f, -0.5f, 0.0f, coords.max.x, coords.min.y};

    vertex_array_bind(&quad->va);
    vertex_buffer_bind(&quad->vb);
    vertex_buffer_set_data(&quad->vb, vertices, sizeof(vertices), 0);
}

bool8 quad_collding_with_quad(quad* quad1, quad* quad2) {
    bool8 collision_x = quad1->position.x + quad1->hit_box.x >= quad2->position.x &&
                        quad2->position.x + quad2->hit_box.x >= quad1->position.x;

    bool8 collision_y = quad1->position.y + quad1->hit_box.y >= quad2->position.y &&
                        quad2->position.y + quad2->hit_box.y >= quad1->position.y;
    return collision_x && collision_y;
}