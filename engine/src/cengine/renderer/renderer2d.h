#pragma once

#include "tilemap.h"
#include "scene.h"

typedef struct renderer2d_state {
    scene* active_scene;
    scene** scenes;
    u32 scene_count;
} renderer2d_state;

void renderer2d_init();

void renderer2d_terminate();

void renderer2d_add_scene(scene* scene);

void renderer2d_set_active_scene(scene* scene);

void renderer2d_switch_active_scene_by_name(const char* name);

scene* renderer2d_get_scene_by_name(const char* name);

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

quad* renderer2d_get_quad_in_tilemap(tile_map map, vector2 tile_pos);