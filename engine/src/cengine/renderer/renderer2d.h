#pragma once

#include "quad2d.h"
#include "triangle2d.h"

typedef struct renderer2d_state {
    quad** quads;
    triangle** triangles;
    u32 quad_count;
    u32 triangle_count;
} renderer2d_state;

void renderer2d_init();

void renderer2d_terminate();

void renderer2d_add_quad(quad* obj);

void renderer2d_remove_quad(quad* obj);

void renderer2d_render_objects();

void renderer2d_update_objects();

i32 renderer2d_get_quad_index(quad* obj);

quad* renderer2d_get_quad_by_tag(const char* tag);

renderer2d_state renderer2d_get_state();

void renderer2d_add_triangle(triangle* obj);

void renderer2d_remove_triangle(triangle* obj);

i32 renderer2d_get_triangle_index(triangle* obj);

triangle* renderer2d_get_triangle_by_tag(const char* tag);

void _renderer2d_sort_quads();