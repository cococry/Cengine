#pragma once

#include "quad2d.h"

typedef struct renderer2d_state {
    quad** quads;
    u32 quad_count;
} renderer2d_state;

void renderer2d_init();

void renderer2d_terminate();

void renderer2d_add_quad(quad* obj);

void renderer2d_remove_quad(quad* obj);

void renderer2d_render_quads();

i32 renderer2d_get_quad_index(quad* obj);

quad* renderer2d_get_quad_by_tag(const char* tag);

renderer2d_state renderer2d_get_state();