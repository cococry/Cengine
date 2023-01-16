#pragma once

#include "../math/vector.h"
#include "../math/matrix4.h"
#include "../physics/phys_def.h"
#include "texture2d.h"
#include "shader.h"

typedef struct batch_renderer_stats {
    u32 quad_count;
    u32 draw_calls;
} batch_renderer_stats;

void batch_renderer_init();

void batch_renderer_terminate();

void batch_renderer_set_sprite_sheet(texture2d* spritesheet);

void batch_renderer_begin_render(matrix4 view_matrix);

void batch_renderer_render_sprite(vector2 position, vector2 scale, float rotation, vector2 uv, vector2 cellsize, vector4 color);

void batch_renderer_render_aabb(AABB aabb, vector4 color);

void _batch_renderer_render_quad_transform_matrix(matrix4 transform, vector4 color);

void batch_renderer_render_quad(vector2 position, vector2 scale, float rotation, vector4 color);

void batch_renderer_end_render();

texture2d* batch_renderer_get_sprite_sheet();

shader_program* batch_renderer_get_shader();

batch_renderer_stats batch_renderer_get_stats();