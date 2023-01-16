#include "batch_renderer.h"

#include "vertex_array.h"
#include "render_command.h"

#include "../core/global_state.h"
#include "../core/logging.h"
#include "../math/radians.h"
#include "../platform/opengl/gl_functions.h"

#include "subtexture2d.h"

#include <stdlib.h>

#define MAX_SPRITE_SHEETS 32

typedef struct vertex {
    vector3 position;
    vector4 color;
    vector2 texcoord;
    bool8 use_textures;
} vertex;

typedef struct batch_renderer_data {
    vertex* vertex_buffer;
    vertex* vertex_buffer_ptr;

    vertex_array vao;
    vertex_buffer vbo;
    index_buffer ibo;

    shader_program* shader;

    u32 max_quads;
    u32 index_count;

    vector4* vertex_positions;

    texture2d* sprite_sheet;
    u32 sprite_sheet_count;

    batch_renderer_stats stats;
} batch_renderer_data;

batch_renderer_data s_data;

void batch_renderer_init() {
    s_data.max_quads = 100000;
    s_data.index_count = 0;

    s_data.vao = vertex_array_create();

    s_data.vbo = vertex_buffer_create_empty(sizeof(vertex) * s_data.max_quads, 4);

    vertex_buffer_add_layout_attribute(&s_data.vbo, vertex_layout_attribute_create(vertex_layout_attrib_type_vector3f));
    vertex_buffer_add_layout_attribute(&s_data.vbo, vertex_layout_attribute_create(vertex_layout_attrib_type_vector4f));
    vertex_buffer_add_layout_attribute(&s_data.vbo, vertex_layout_attribute_create(vertex_layout_attrib_type_vector2f));
    vertex_buffer_add_layout_attribute(&s_data.vbo, vertex_layout_attribute_create(vertex_layout_attrib_type_int));

    vertex_array_add_vertex_buffer(&s_data.vao, s_data.vbo);

    s_data.vertex_buffer = malloc(sizeof(vertex) * s_data.max_quads * 4);

    u32* indices = malloc(sizeof(u32) * s_data.max_quads * 6);

    u32 offset = 0;
    for (u32 i = 0; i < s_data.max_quads * 6; i += 6) {
        indices[i + 0] = offset + 0;
        indices[i + 1] = offset + 1;
        indices[i + 2] = offset + 2;

        indices[i + 3] = offset + 2;
        indices[i + 4] = offset + 3;
        indices[i + 5] = offset + 0;

        offset += 4;
    }
    s_data.ibo = index_buffer_create(indices, s_data.max_quads * 6, CNGN_STATIC_DRAW);
    vertex_array_set_index_buffer(&s_data.vao, s_data.ibo);

    free(indices);

    s_data.shader = shader_program_create("../engine/assets/shaders/default_vertex.glsl", "../engine/assets/shaders/default_fragment.glsl");
    shader_program_bind(s_data.shader);
    shader_program_upload_mat4(s_data.shader, "u_proj", orthographic_matrix(0.0f, g_state->app->wnd->props.width, 0.0f, g_state->app->wnd->props.height));

    s_data.vertex_positions = malloc(sizeof(vector4) * 4);
    s_data.vertex_positions[0] = vector4_create(-0.5f, -0.5f, 0.0f, 1.0f);
    s_data.vertex_positions[1] = vector4_create(0.5f, -0.5f, 0.0f, 1.0f);
    s_data.vertex_positions[2] = vector4_create(0.5f, 0.5f, 0.0f, 1.0f);
    s_data.vertex_positions[3] = vector4_create(-0.5f, 0.5f, 0.0f, 1.0f);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}
void batch_renderer_terminate() {
    free(s_data.vertex_buffer);
    free(s_data.vertex_positions);
}

void batch_renderer_set_sprite_sheet(texture2d* spritesheet) {
    s_data.sprite_sheet = spritesheet;
    shader_program_bind(s_data.shader);
    shader_program_upload_int(s_data.shader, "u_texture", 0);
}

void batch_renderer_begin_render(matrix4 view_matrix) {
    shader_program_bind(s_data.shader);
    shader_program_upload_mat4(s_data.shader, "u_view", view_matrix);

    s_data.index_count = 0;
    s_data.vertex_buffer_ptr = s_data.vertex_buffer;
    s_data.stats.draw_calls = 0;
    s_data.stats.quad_count = 0;
}

void batch_renderer_render_sprite(vector2 position, vector2 scale, float rotation, vector2 uv, vector2 cellsize, vector4 color) {
    subtexture2d subtexture = subtexture2d_create(s_data.sprite_sheet, uv, cellsize, vector2_create(1.0f, 1.0f));
    subtexture_coords coords = subtexture2d_get_texcoords(subtexture);
    vector2 texture_coords[] = {
        vector2_create(coords.min.x, coords.min.y),
        vector2_create(coords.max.x, coords.min.y),
        vector2_create(coords.max.x, coords.max.y),
        vector2_create(coords.min.x, coords.max.y)};

    matrix4 transform = matrix4_multiply(
        translate_mv(matrix4_identity(), vector3_create(position.x, position.y, 0.0f)),
        scale_mv(matrix4_identity(), vector3_create(scale.x, scale.y, 0.0f)));
    transform = matrix4_multiply(transform, rotate_mv_angle(matrix4_identity(), degrees_to_radians(rotation), vector3_create(0.0f, 0.0f, 1.0f)));

    for (u32 i = 0; i < 4; i++) {
        s_data.vertex_buffer_ptr->position = vector3_create(vector4_multiply_matrix4(transform, s_data.vertex_positions[i]).x,
                                                            vector4_multiply_matrix4(transform, s_data.vertex_positions[i]).y,
                                                            vector4_multiply_matrix4(transform, s_data.vertex_positions[i]).z);
        s_data.vertex_buffer_ptr->color = color;
        s_data.vertex_buffer_ptr->texcoord = texture_coords[i];
        s_data.vertex_buffer_ptr->use_textures = true;
        s_data.vertex_buffer_ptr++;
    }
    s_data.index_count += 6;
    s_data.stats.quad_count++;
}

void batch_renderer_render_aabb(AABB aabb, vector4 color) {
    batch_renderer_render_sprite(aabb.position, vector2_create(aabb.half_size.x * 2.0f, aabb.half_size.y * 2.0f),
                                 0.0f, vector2_create(5.0f, 0.0f), vector2_create(16.0f, 16.0f), color);
}
void batch_renderer_end_render() {
    u32 vertex_data_size = (u32)((u8*)s_data.vertex_buffer_ptr - (u8*)s_data.vertex_buffer);
    vertex_buffer_set_data(&s_data.vbo, s_data.vertex_buffer, vertex_data_size, 0);

    shader_program_bind(s_data.shader);
    texture2d_bind_unit(s_data.sprite_sheet, 0);

    render_command_draw_indexed_va_check(&s_data.vao, s_data.index_count);
    s_data.stats.draw_calls++;
}

void batch_renderer_render_quad(vector2 position, vector2 scale, float rotation, vector4 color) {
    matrix4 transform = matrix4_multiply(
        translate_mv(matrix4_identity(), vector3_create(position.x, position.y, 0.0f)),
        scale_mv(matrix4_identity(), vector3_create(scale.x, scale.y, 0.0f)));
    transform = matrix4_multiply(transform, rotate_mv_angle(matrix4_identity(), degrees_to_radians(rotation), vector3_create(0.0f, 0.0f, 1.0f)));

    _batch_renderer_render_quad_transform_matrix(transform, color);
}

void _batch_renderer_render_quad_transform_matrix(matrix4 transform, vector4 color) {
    vector2 texture_coords[] = {
        vector2_create(0.0f, 0.0f),
        vector2_create(1.0f, 0.0f),
        vector2_create(1.0f, 1.0f),
        vector2_create(0.0f, 1.0f)};

    for (u32 i = 0; i < 4; i++) {
        s_data.vertex_buffer_ptr->position = vector3_create(vector4_multiply_matrix4(transform, s_data.vertex_positions[i]).x,
                                                            vector4_multiply_matrix4(transform, s_data.vertex_positions[i]).y,
                                                            vector4_multiply_matrix4(transform, s_data.vertex_positions[i]).z);
        s_data.vertex_buffer_ptr->color = color;
        s_data.vertex_buffer_ptr->texcoord = texture_coords[i];
        s_data.vertex_buffer_ptr->use_textures = false;
        s_data.vertex_buffer_ptr++;
    }
    s_data.index_count += 6;
    s_data.stats.quad_count++;
}

texture2d* batch_renderer_get_sprite_sheet() {
    return s_data.sprite_sheet;
}
batch_renderer_stats batch_renderer_get_stats() {
    return s_data.stats;
}

shader_program* batch_renderer_get_shader() {
    return s_data.shader;
}