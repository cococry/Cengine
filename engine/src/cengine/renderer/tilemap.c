#include "tilemap.h"

#include "subtexture2d.h"

#include "quad2d.h"
#include "renderer2d.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_REGISTERED_TILES 256

tile_map_tile_render tile_map_tile_render_create(char* identifier, vector2 position) {
    tile_map_tile_render ret;
    ret.identifier = identifier;
    ret.position = position;
    return ret;
}

tile_map_tile tile_map_tile_create(char* identifier, vector2 texcoords) {
    tile_map_tile ret;
    ret.identifier = identifier;
    ret.texcoords = texcoords;
    return ret;
}

bool8 tile_map_compare(tile_map map1, tile_map map2) {
    if (map1.registered_tile_count != map2.registered_tile_count) {
        return false;
    }
    for (u32 i = 0; i < map1.registered_tile_count; i++) {
        if (map1.registered_tiles[i].identifier != map2.registered_tiles[i].identifier && !vector2_compare(map1.registered_tiles[i].texcoords, map2.registered_tiles[i].texcoords)) {
            return false;
        }
    }
    if (map1.render_level != map2.render_level) {
        return false;
    }
    if (!texture2d_compare(map1.sprite_sheet, map2.sprite_sheet)) {
        return false;
    }
    if (!vector2_compare(map1.tile_cell_size, map2.tile_cell_size)) {
        return false;
    }
    if (!vector2_compare(map1.tile_scale, map2.tile_scale)) {
        return false;
    }
    return true;
}

tile_map tile_map_create(texture2d* sprite_sheet, vector2 tile_cell_size, vector2 tile_scale, u32 tilemap_render_level) {
    tile_map ret;
    ret.registered_tiles = malloc(sizeof(tile_map_tile) * MAX_REGISTERED_TILES);
    ret.registered_tile_count = 0;
    ret.sprite_sheet = sprite_sheet;
    ret.tile_cell_size = tile_cell_size;
    ret.tile_scale = tile_scale;
    ret.render_level = tilemap_render_level;
    return ret;
}

void tile_map_register_tile(tile_map* map, tile_map_tile tile) {
    map->registered_tiles[map->registered_tile_count++] = tile;
}

void tile_map_commit_to_render(tile_map* map, tile_map_tile_render tiles[], u32 tile_count) {
    for (u32 i = 0; i < tile_count; i++) {
        tile_map_tile* tile = nullptr;
        for (u32 j = 0; j < map->registered_tile_count; j++) {
            if (tiles[i].identifier == map->registered_tiles[j].identifier) {
                tile = &map->registered_tiles[j];
                break;
            }
        }
        if (tile != nullptr) {
            subtexture2d texture = subtexture2d_create(map->sprite_sheet, tile->texcoords, map->tile_cell_size, vector2_create(1.0f, 1.0f));
            quad* quad_to_render = quad_create(
                "TilemapTile", vector2_multiply(tiles[i].position, vector2_create(100.0f, 100.0f)),
                map->tile_scale,
                map->tile_scale, 0.0f,
                map->render_level,
                vector4_create(1.0f, 1.0f, 1.0f, 1.0f),
                &texture);
            quad_to_render->associated_tilemap = map;
            quad_to_render->tile_map_tile_position = tiles[i].position;
            renderer2d_add_quad(quad_to_render);
        }
    }
}

void tile_map_commit_to_render_contiguous(tile_map* map, char** identifiers, u32 tile_count, u32 x_size) {
    float y = 0.0f;
    float x = 0.0f;
    for (u32 i = 0; i < tile_count; i++) {
        tile_map_tile* tile = nullptr;
        for (u32 j = 0; j < map->registered_tile_count; j++) {
            if (identifiers[i] == map->registered_tiles[j].identifier) {
                tile = &map->registered_tiles[j];
                break;
            }
        }
        if (tile != nullptr) {
            if (x >= x_size) {
                y--;
                x = 0.0f;
            }
            vector2 pos = vector2_create(x, y);

            subtexture2d texture = subtexture2d_create(map->sprite_sheet, tile->texcoords, map->tile_cell_size, vector2_create(1.0f, 1.0f));
            quad* quad_to_render = quad_create(
                "TilemapTile", vector2_multiply(pos, vector2_create(100.0f, 100.0f)),
                map->tile_scale,
                map->tile_scale, 0.0f,
                map->render_level,
                vector4_create(1.0f, 1.0f, 1.0f, 1.0f),
                &texture);
            quad_to_render->associated_tilemap = map;
            quad_to_render->tile_map_tile_position = pos;
            renderer2d_add_quad(quad_to_render);
            x++;
        }
    }
}

void tile_map_commit_to_render_box(tile_map* map, char* tile_identifier, vector2 min, vector2 max) {
    tile_map_tile* map_tile = nullptr;
    for (u32 i = 0; i < map->registered_tile_count; i++) {
        if (tile_identifier == map->registered_tiles[i].identifier) {
            map_tile = &map->registered_tiles[i];
            break;
        }
    }
    if (map_tile != nullptr) {
        for (u32 x = min.x; x < max.x; x++) {
            for (u32 y = min.y; y < max.y; y++) {
                subtexture2d texture = subtexture2d_create(map->sprite_sheet, map_tile->texcoords,
                                                           map->tile_cell_size, vector2_create(1.0f, 1.0f));
                quad* quad_to_render = quad_create("TilemapTile",
                                                   vector2_multiply(vector2_create(x, y), map->tile_scale),
                                                   map->tile_scale,
                                                   map->tile_scale,
                                                   0.0f,
                                                   map->render_level,
                                                   vector4_create(1.0f, 1.0f, 1.0f, 1.0f),
                                                   &texture);
                quad_to_render->associated_tilemap = map;
                quad_to_render->tile_map_tile_position = vector2_create(x, y);
                renderer2d_add_quad(quad_to_render);
            }
        }
    }
}
void tile_map_add_tile(tile_map* map, tile_map_tile_render tile) {
    tile_map_tile* map_tile = nullptr;
    for (u32 i = 0; i < map->registered_tile_count; i++) {
        if (tile.identifier == map->registered_tiles[i].identifier) {
            map_tile = &map->registered_tiles[i];
            break;
        }
    }
    if (map_tile != nullptr) {
        subtexture2d texture = subtexture2d_create(map->sprite_sheet, map_tile->texcoords,
                                                   map->tile_cell_size, vector2_create(1.0f, 1.0f));

        quad* quad_to_render = quad_create("TilemapTile",
                                           vector2_multiply(tile.position, vector2_create(100.0f, 100.0f)),
                                           map->tile_scale,
                                           map->tile_scale,
                                           0.0f,
                                           map->render_level,
                                           vector4_create(1.0f, 1.0f, 1.0f, 1.0f),
                                           &texture);
        quad_to_render->associated_tilemap = map;
        quad_to_render->tile_map_tile_position = tile.position;
        renderer2d_add_quad(quad_to_render);
    }
}

quad* tile_map_get_quad_by_tile_pos(tile_map map, vector2 tile_pos) {
    return renderer2d_get_quad_in_tilemap(map, tile_pos);
}