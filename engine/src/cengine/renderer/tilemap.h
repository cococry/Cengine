#pragma once

#include "../core/defines.h"
#include "../math/vector.h"
#include "texture2d.h"
#include "quad2d.h"

typedef struct tile_map_tile {
    char* identifier;
    vector2 texcoords;
} tile_map_tile;

typedef struct tile_map_tile_render {
    char* identifier;
    vector2 position;
} tile_map_tile_render;

tile_map_tile_render tile_map_tile_render_create(char* identifier, vector2 position);

tile_map_tile tile_map_tile_create(char* identifier, vector2 texcoords);

typedef struct tile_map {
    tile_map_tile* registered_tiles;
    u32 registered_tile_count;
    vector2 tile_cell_size;
    vector2 tile_scale;
    u32 render_level;
    texture2d* sprite_sheet;
} tile_map;

bool8 tile_map_compare(tile_map map1, tile_map map2);

tile_map tile_map_create(texture2d* sprite_sheet, vector2 tile_cell_size, vector2 tile_scale, u32 tilemap_render_level);

void tile_map_register_tile(tile_map* map, tile_map_tile tile);

void tile_map_commit_to_render(tile_map* map, tile_map_tile_render tiles[], u32 tile_count);

void tile_map_commit_to_render_contiguous(tile_map* map, char** identifiers, u32 tile_count, u32 x_size);

void tile_map_commit_to_render_box(tile_map* map, char* tile_identifier, vector2 min, vector2 max);

void tile_map_add_tile(tile_map* map, tile_map_tile_render tile);

quad* tile_map_get_quad_by_tile_pos(tile_map map, vector2 tile_pos);