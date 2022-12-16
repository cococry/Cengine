#pragma once

#include "triangle2d.h"
#include "quad2d.h"
#include "tilemap.h"

typedef struct scene {
    const char* name;
    quad** quads;
    triangle** triangles;
    u32 quad_count, triangle_count;
    bool8 _need_sort_quads, _need_sort_triangles;
} scene;

scene* scene_create_empty(const char* name);

scene* scene_create(const char* name, quad** quads, triangle** triangles, u32 quad_count, u32 triangle_count);

void scene_add_quad(scene* scene, quad* quad);

void scene_add_triangle(scene* scene, triangle* triangle);

void scene_remove_quad(scene* scene, quad* quad);

void scene_remove_triangle(scene* scene, triangle* triangle);

i32 scene_get_quad_index(scene* scene, quad* quad);

i32 scene_get_triangle_index(scene* scene, triangle* triangle);

quad* scene_get_quad_by_tag(scene* scene, const char* tag);

triangle* scene_get_triangle_by_tag(scene* scene, const char* tag);

void scene_render(scene* scene);

void scene_update(scene* scene);

void scene_sort_quads_by_render_order(scene* scene);

void scene_sort_triangles_by_render_order(scene* scene);

quad* scene_get_quad_in_tilemap(scene* scene, tile_map map, vector2 tile_pos);