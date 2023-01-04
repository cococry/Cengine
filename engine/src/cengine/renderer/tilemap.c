#include "tilemap.h"
#include "batch_renderer.h"
#include "../core/logging.h"

#include "../../../vendor/stb_image/stb_image.h"
#include <stdlib.h>

#define MAX_REGISTERED_TILE_COUNT 64

static bool8 tile_map_find_registered_tile_color(tile_map* map, vector4 color) {
    for (u32 i = 0; i < map->registered_tile_count; i++) {
        if (vector4_compare(map->regerstered_tiles[i].color, color)) {
            return true;
        }
    }
    return false;
}

static vector2 tile_get_get_uv_by_color(tile_map* map, vector4 color) {
    for (u32 i = 0; i < map->registered_tile_count; i++) {
        if (vector4_compare(map->regerstered_tiles[i].color, color)) {
            return map->regerstered_tiles[i].uv;
        }
    }
    return vector2_create(-1.0f, -1.0f);
}

registered_tile registered_tile_create(vector4 color, vector2 uv) {
    registered_tile ret;
    ret.color = color;
    ret.uv = uv;
    return ret;
}
tile_map tile_map_create(vector2 tile_size, vector2 tile_scale, vector2 position) {
    tile_map ret;
    ret.tile_size = tile_size;
    ret.tile_scale = tile_scale;
    ret.registered_tile_count = 0;
    ret.position = position;
    ret.regerstered_tiles = malloc(sizeof(registered_tile) * MAX_REGISTERED_TILE_COUNT);
    return ret;
}

void tile_map_register_tiles(tile_map* map, registered_tile* tiles, u32 tile_count) {
    if (map->regerstered_tiles == nullptr) return;

    for (u32 i = map->registered_tile_count; i < tile_count; i++) {
        if (tile_map_find_registered_tile_color(map, tiles[i].color)) continue;
        map->regerstered_tiles[map->registered_tile_count++] = tiles[i];
    }
}

void tile_map_register_tile(tile_map* map, registered_tile tile) {
    map->regerstered_tiles[map->registered_tile_count++] = tile;
}

static void get_pixel(stbi_uc* image, size_t imageWidth, size_t x, size_t y, stbi_uc* r, stbi_uc* g, stbi_uc* b, stbi_uc* a) {
    const stbi_uc* p = image + (4 * (y * imageWidth + x));
    *r = p[0];
    *g = p[1];
    *b = p[2];
    *a = p[3];
}
void tile_map_load_from_file(tile_map* map, const char* filepath) {
    i32 width, height, number_of_channels;
    u8* data = stbi_load(filepath, &width, &height, &number_of_channels, 4);

    map->map_size.x = width;
    map->map_size.y = height;
    map->tiles = malloc(sizeof(vector2) * sizeof(vector2) * width * height);
    map->tile_count = 0;

    if (!data) {
        LOG_ERROR("Failed to load tilemap file at location %s", filepath);
    }

    u32 tile_index = 0;
    for (u32 y = 0; y < height; y++) {
        for (u32 x = 0; x < width; x++) {
            stbi_uc r, g, b, a;
            get_pixel(data, width, x, y, &r, &g, &b, &a);
            vector4 color = vector4_create(r, g, b, a);
            if (tile_map_find_registered_tile_color(map, color)) {
                map->tiles[tile_index].position = vector2_create(x, y);
                map->tiles[tile_index].uv = tile_get_get_uv_by_color(map, color);
                map->tile_count++;
            }
            tile_index++;
        }
        tile_index++;
    }
    stbi_image_free(data);
}

void tile_map_render(tile_map* map) {
    u32 i = 0;
    for (u32 x = 0; x < map->map_size.x; x++) {
        for (u32 y = 0; y < map->map_size.y; y++) {
            batch_renderer_render_sprite(
                vector2_additition(
                    vector2_multiply(map->tile_scale, map->tiles[i].position),
                    map->position),
                map->tile_scale,
                0.0f,
                map->tiles[i].uv,
                map->tile_size,
                vector4_create(1.0f, 1.0f, 1.0f, 1.0f));
            i++;
        }
        i++;
    }
}