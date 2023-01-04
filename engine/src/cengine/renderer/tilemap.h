#pragma once

#include "../math/vector4.h"
#include "subtexture2d.h"

typedef struct registered_tile {
    vector4 color;
    vector2 uv;
} registered_tile;

typedef struct tile_map_tile {
    vector2 uv;
    vector2 position;
} tile_map_tile;

typedef struct tile_map {
    vector2 map_size;
    vector2 tile_size;
    vector2 tile_scale;
    vector2 position;
    tile_map_tile* tiles;
    u32 tile_count;
    registered_tile* regerstered_tiles;
    u32 registered_tile_count;
} tile_map;

registered_tile registered_tile_create(vector4 color, vector2 uv);

tile_map tile_map_create(vector2 tile_size, vector2 tile_scale, vector2 position);

void tile_map_register_tiles(tile_map* map, registered_tile* tiles, u32 tile_count);

void tile_map_register_tile(tile_map* map, registered_tile tile);

void tile_map_load_from_file(tile_map* map, const char* filepath);

void tile_map_render(tile_map* map);