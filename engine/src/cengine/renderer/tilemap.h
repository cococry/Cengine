#pragma once

#include "../math/vector4.h"
#include "../physics/phys_def.h"

#include "subtexture2d.h"

typedef struct registered_tile_variant {
    vector4 color;
    vector2 uv_base_point;
} registered_tile_variant;

typedef struct registered_tile_variant_uvs {
    vector2 uv_left, uv_right, uv_up, uv_down,
        uv_diagonal_left_down, uv_diagonal_right_down,
        uv_diagonal_left_up, uv_diagonal_right_up,
        uv_straight_horizontal, uv_straight_vertical,
        uv_outstanding_left, uv_outstanding_right,
        uv_outstanding_up, uv_outstanding_down;
} registered_tile_variant_uvs;

typedef struct tile_map_tile {
    vector2 uv;
} tile_map_tile;

typedef struct tile_map {
    vector2 map_size;
    vector2 tile_size;
    vector2 tile_scale;
    vector2 position;
    tile_map_tile* tiles;
    u32 tile_count;
    registered_tile_variant* registered_tile_variants;
    u32 registered_tile_variatiant_count;
    AABB* chunk_aabbs;
    u32 chunk_aabbs_count;
} tile_map;

registered_tile_variant registered_tile_variant_create(vector4 color, vector2 base_point);

tile_map tile_map_create(vector2 tile_size, vector2 tile_scale, vector2 position);

void tile_map_register_tile_variants(tile_map* map, registered_tile_variant* tile_variants, u32 tile_variant_count);

void tile_map_register_tile_variant(tile_map* map, registered_tile_variant tile_variant);

void tile_map_load_from_file(tile_map* map, const char* filepath);

void tile_map_render(tile_map* map, AABB camera_aabb);

AABB tile_map_get_chunk_aabb(vector2 chunk_position, tile_map map);

void tile_map_chunk_render(tile_map* map, AABB chunk_aabb);

vector2 tile_map_chunk_get_grid_position(AABB chunk_aabb);