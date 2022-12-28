#pragma once

#include "vertex_array.h"
#include "../core/defines.h"

void render_command_clear_buffers(u32 buffer_flags);

void render_command_clear_color(float r, float g, float b, float a);

void render_command_draw_indexed(vertex_array* va);

void render_command_draw_indexed_va_check(vertex_array* va, u32 index_count);

void render_command_resize_viewport(u32 width, u32 height);