#include "tilemap.h"
#include "batch_renderer.h"
#include "../core/logging.h"
#include "../core/util.h"

#include "../../../vendor/stb_image/stb_image.h"
#include <stdlib.h>
#include <string.h>

#define MAX_REGISTERED_TILE_COUNT 64
#define TILE_COUNT_IN_CHUNK 4
#define MAX_VISIBLE_CHUNK_COUNT 512

typedef struct pixel_neighbour_colors {
    vector4 color_left, color_right, color_up, color_down;
    vector4 color_diagonal_left_down, color_diagonal_right_down, color_diagonal_left_up, color_diagonal_right_up;
} pixel_neighbour_colors;

static bool8 tile_has_registered_variant(tile_map* map, vector4 color) {
    for (u32 i = 0; i < map->registered_tile_variatiant_count; i++) {
        if (vector4_compare(map->registered_tile_variants[i].color, color)) {
            return true;
        }
    }
    return false;
}

static i32 tile_get_variant_index_by_color(tile_map* map, vector4 color) {
    for (i32 i = 0; i < map->registered_tile_variatiant_count; i++) {
        if (vector4_compare(map->registered_tile_variants[i].color, color)) {
            return i;
        }
    }
    return -1;
}
registered_tile_variant registered_tile_variant_create(vector4 color, vector2 base_point) {
    registered_tile_variant ret;
    ret.color = color;
    ret.uv_base_point = base_point;
    return ret;
}
tile_map tile_map_create(vector2 tile_size, vector2 tile_scale, vector2 position) {
    tile_map ret;
    ret.tile_size = tile_size;
    ret.tile_scale = tile_scale;
    ret.registered_tile_variatiant_count = 0;
    ret.chunk_aabbs_count = 0;
    ret.position = position;
    ret.registered_tile_variants = malloc(sizeof(registered_tile_variant) * MAX_REGISTERED_TILE_COUNT);
    return ret;
}
void tile_map_register_tile_variants(tile_map* map, registered_tile_variant* tile_variants, u32 tile_variant_count) {
    if (map->registered_tile_variants == nullptr) return;

    for (u32 i = map->registered_tile_variatiant_count; i < tile_variant_count; i++) {
        if (tile_get_variant_index_by_color(map, tile_variants[i].color) != -1) continue;
        map->registered_tile_variants[map->registered_tile_variatiant_count++] = tile_variants[i];
    }
}

void tile_map_register_tile_variant(tile_map* map, registered_tile_variant tile_variant) {
    map->registered_tile_variants[map->registered_tile_variatiant_count++] = tile_variant;
}

static void get_pixel(stbi_uc* image, size_t imageWidth, size_t x, size_t y, stbi_uc* r, stbi_uc* g, stbi_uc* b, stbi_uc* a) {
    const stbi_uc* p = image + (4 * (y * imageWidth + x));
    *r = p[0];
    *g = p[1];
    *b = p[2];
    *a = p[3];
}

static pixel_neighbour_colors get_pixel_neighbour_colors(u8* image_data, u32 image_width, u32 image_height, vector2 pixel) {
    pixel_neighbour_colors ret;
    if (pixel.x + 1 < image_width) {
        stbi_uc r, g, b, a;
        get_pixel(image_data, image_width, pixel.x + 1, pixel.y, &r, &g, &b, &a);
        ret.color_right = vector4_create(r, g, b, a);
    } else {
        ret.color_right = INVALID_COLOR;
    }

    if (pixel.x - 1 >= 0) {
        stbi_uc r, g, b, a;
        get_pixel(image_data, image_width, pixel.x - 1, pixel.y, &r, &g, &b, &a);
        ret.color_left = vector4_create(r, g, b, a);
    } else {
        ret.color_left = INVALID_COLOR;
    }

    if (pixel.y + 1 < image_height) {
        stbi_uc r, g, b, a;
        get_pixel(image_data, image_width, pixel.x, pixel.y + 1, &r, &g, &b, &a);
        ret.color_up = vector4_create(r, g, b, a);
    } else {
        ret.color_up = INVALID_COLOR;
    }
    if (pixel.y - 1 >= 0) {
        stbi_uc r, g, b, a;
        get_pixel(image_data, image_width, pixel.x, pixel.y - 1, &r, &g, &b, &a);
        ret.color_down = vector4_create(r, g, b, a);
    } else {
        ret.color_down = INVALID_COLOR;
    }
    if (pixel.x - 1 >= 0 && pixel.y - 1 >= 0) {
        stbi_uc r, g, b, a;
        get_pixel(image_data, image_width, pixel.x - 1, pixel.y - 1, &r, &g, &b, &a);
        ret.color_diagonal_left_down = vector4_create(r, g, b, a);
    } else {
        ret.color_diagonal_left_down = INVALID_COLOR;
    }
    if (pixel.x + 1 < image_width && pixel.y - 1 >= 0) {
        stbi_uc r, g, b, a;
        get_pixel(image_data, image_width, pixel.x + 1, pixel.y - 1, &r, &g, &b, &a);
        ret.color_diagonal_right_down = vector4_create(r, g, b, a);
    } else {
        ret.color_diagonal_right_down = INVALID_COLOR;
    }
    if (pixel.x - 1 < image_width && pixel.y + 1 < image_height) {
        stbi_uc r, g, b, a;
        get_pixel(image_data, image_width, pixel.x - 1, pixel.y + 1, &r, &g, &b, &a);
        ret.color_diagonal_left_up = vector4_create(r, g, b, a);
    } else {
        ret.color_diagonal_left_up = INVALID_COLOR;
    }
    if (pixel.x + 1 < image_width && pixel.y + 1 < image_height) {
        stbi_uc r, g, b, a;
        get_pixel(image_data, image_width, pixel.x + 1, pixel.y + 1, &r, &g, &b, &a);
        ret.color_diagonal_right_up = vector4_create(r, g, b, a);
    } else {
        ret.color_diagonal_right_up = INVALID_COLOR;
    }
    return ret;
}

static bool8 pixel_no_neighbour(u8* image_data, u32 image_width, u32 image_height, vector2 pixel) {
    pixel_neighbour_colors neighbour_colors = get_pixel_neighbour_colors(image_data, image_width, image_height, pixel);
    return (vector4_compare(neighbour_colors.color_left, INVALID_COLOR) &&
            vector4_compare(neighbour_colors.color_right, INVALID_COLOR) &&
            vector4_compare(neighbour_colors.color_up, INVALID_COLOR) &&
            vector4_compare(neighbour_colors.color_down, INVALID_COLOR) &&
            vector4_compare(neighbour_colors.color_diagonal_left_down, INVALID_COLOR) &&
            vector4_compare(neighbour_colors.color_diagonal_right_down, INVALID_COLOR) &&
            vector4_compare(neighbour_colors.color_diagonal_left_up, INVALID_COLOR) &&
            vector4_compare(neighbour_colors.color_diagonal_right_up, INVALID_COLOR));
}
static bool8 pixel_left_neighbour(u8* image_data, u32 image_width, u32 image_height, vector2 pixel) {
    pixel_neighbour_colors neighbour_colors = get_pixel_neighbour_colors(image_data, image_width, image_height, pixel);
    stbi_uc r, g, b, a;
    get_pixel(image_data, image_width, pixel.x, pixel.y, &r, &g, &b, &a);
    return (vector4_compare(vector4_create(r, g, b, a), neighbour_colors.color_left));
}
static bool8 pixel_right_neighbour(u8* image_data, u32 image_width, u32 image_height, vector2 pixel) {
    pixel_neighbour_colors neighbour_colors = get_pixel_neighbour_colors(image_data, image_width, image_height, pixel);
    stbi_uc r, g, b, a;
    get_pixel(image_data, image_width, pixel.x, pixel.y, &r, &g, &b, &a);
    return (vector4_compare(vector4_create(r, g, b, a), neighbour_colors.color_right));
}
static bool8 pixel_up_neighbour(u8* image_data, u32 image_width, u32 image_height, vector2 pixel) {
    pixel_neighbour_colors neighbour_colors = get_pixel_neighbour_colors(image_data, image_width, image_height, pixel);
    stbi_uc r, g, b, a;
    get_pixel(image_data, image_width, pixel.x, pixel.y, &r, &g, &b, &a);
    return (vector4_compare(vector4_create(r, g, b, a), neighbour_colors.color_up));
}
static bool8 pixel_down_neighbour(u8* image_data, u32 image_width, u32 image_height, vector2 pixel) {
    pixel_neighbour_colors neighbour_colors = get_pixel_neighbour_colors(image_data, image_width, image_height, pixel);
    stbi_uc r, g, b, a;
    get_pixel(image_data, image_width, pixel.x, pixel.y, &r, &g, &b, &a);
    return (vector4_compare(vector4_create(r, g, b, a), neighbour_colors.color_down));
}
static registered_tile_variant_uvs get_registered_tile_variant_uvs(registered_tile_variant variant) {
    registered_tile_variant_uvs ret;
    ret.uv_diagonal_left_down = vector2_create(variant.uv_base_point.x - 1.0f, variant.uv_base_point.y - 1.0f);
    ret.uv_diagonal_right_down = vector2_create(variant.uv_base_point.x + 1.0f, variant.uv_base_point.y - 1.0f);
    ret.uv_diagonal_left_up = vector2_create(variant.uv_base_point.x - 1.0f, variant.uv_base_point.y + 1.0f);
    ret.uv_diagonal_right_up = vector2_create(variant.uv_base_point.x + 1.0f, variant.uv_base_point.y + 1.0f);
    ret.uv_left = vector2_create(variant.uv_base_point.x - 1.0f, variant.uv_base_point.y);
    ret.uv_right = vector2_create(variant.uv_base_point.x + 1.0f, variant.uv_base_point.y);
    ret.uv_up = vector2_create(variant.uv_base_point.x, variant.uv_base_point.y + 1.0f);
    ret.uv_down = vector2_create(variant.uv_base_point.x, variant.uv_base_point.y - 1.0f);
    ret.uv_straight_horizontal = vector2_create(variant.uv_base_point.x + 2.0f, variant.uv_base_point.y - 1.0f);
    ret.uv_straight_vertical = vector2_create(variant.uv_base_point.x + 3.0f, variant.uv_base_point.y - 1.0f);
    ret.uv_outstanding_up = vector2_create(variant.uv_base_point.x + 2.0f, variant.uv_base_point.y);
    ret.uv_outstanding_down = vector2_create(variant.uv_base_point.x + 3.0f, variant.uv_base_point.y);
    ret.uv_outstanding_left = vector2_create(variant.uv_base_point.x + 3.0f, variant.uv_base_point.y + 1.0f);
    ret.uv_outstanding_right = vector2_create(variant.uv_base_point.x + 2.0f, variant.uv_base_point.y + 1.0f);
    return ret;
}
void tile_map_load_from_file(tile_map* map, const char* filepath) {
    i32 width, height, number_of_channels;
    u8* data = stbi_load(filepath, &width, &height, &number_of_channels, 4);

    map->map_size.x = width;
    map->map_size.y = height;
    map->tiles = malloc(sizeof(vector2) * sizeof(vector2) * width * height);
    map->tile_count = 0;
    map->chunk_aabbs = malloc(sizeof(AABB) * width * height);

    if (!data) {
        LOG_ERROR("Failed to load tilemap file at location %s", filepath);
    }

    for (u32 y = 0; y < height; y++) {
        for (u32 x = 0; x < width; x++) {
            stbi_uc r, g, b, a;
            get_pixel(data, width, x, y, &r, &g, &b, &a);
            vector4 color = vector4_create(r, g, b, a);
            if (tile_get_variant_index_by_color(map, color) != -1) {
                if (tile_has_registered_variant(map, color)) {
                    registered_tile_variant_uvs tile_variant = get_registered_tile_variant_uvs(map->registered_tile_variants[tile_get_variant_index_by_color(map, color)]);
                    if (pixel_no_neighbour(data, width, height, vector2_create(x, y))) {
                        map->tiles[y * width + x].uv = map->registered_tile_variants[tile_get_variant_index_by_color(map, color)].uv_base_point;
                        map->tile_count++;
                        continue;
                    } else if (pixel_right_neighbour(data, width, height, vector2_create(x, y)) &&
                               pixel_left_neighbour(data, width, height, vector2_create(x, y)) &&
                               pixel_up_neighbour(data, width, height, vector2_create(x, y)) &&
                               pixel_down_neighbour(data, width, height, vector2_create(x, y))) {
                        map->tiles[y * width + x].uv = map->registered_tile_variants[tile_get_variant_index_by_color(map, color)].uv_base_point;
                        map->tile_count++;
                        continue;
                    } else if (pixel_right_neighbour(data, width, height, vector2_create(x, y)) &&
                               !pixel_left_neighbour(data, width, height, vector2_create(x, y)) &&
                               !pixel_up_neighbour(data, width, height, vector2_create(x, y)) &&
                               !pixel_down_neighbour(data, width, height, vector2_create(x, y))) {
                        map->tiles[y * width + x].uv = tile_variant.uv_outstanding_left;
                        map->tile_count++;
                        continue;
                    } else if (pixel_left_neighbour(data, width, height, vector2_create(x, y)) &&
                               !pixel_right_neighbour(data, width, height, vector2_create(x, y)) &&
                               !pixel_up_neighbour(data, width, height, vector2_create(x, y)) &&
                               !pixel_down_neighbour(data, width, height, vector2_create(x, y))) {
                        map->tiles[y * width + x].uv = tile_variant.uv_outstanding_right;
                        map->tile_count++;
                        continue;
                    } else if (pixel_down_neighbour(data, width, height, vector2_create(x, y)) &&
                               !pixel_right_neighbour(data, width, height, vector2_create(x, y)) &&
                               !pixel_up_neighbour(data, width, height, vector2_create(x, y)) &&
                               !pixel_left_neighbour(data, width, height, vector2_create(x, y))) {
                        map->tiles[y * width + x].uv = tile_variant.uv_outstanding_up;
                        map->tile_count++;
                        continue;
                    } else if (pixel_up_neighbour(data, width, height, vector2_create(x, y)) &&
                               !pixel_right_neighbour(data, width, height, vector2_create(x, y)) &&
                               !pixel_left_neighbour(data, width, height, vector2_create(x, y)) &&
                               !pixel_down_neighbour(data, width, height, vector2_create(x, y))) {
                        map->tiles[y * width + x].uv = tile_variant.uv_outstanding_down;
                        map->tile_count++;
                        continue;
                    } else if (pixel_right_neighbour(data, width, height, vector2_create(x, y)) &&
                               pixel_up_neighbour(data, width, height, vector2_create(x, y)) &&
                               pixel_down_neighbour(data, width, height, vector2_create(x, y))) {
                        map->tiles[y * width + x].uv = tile_variant.uv_left;
                        map->tile_count++;
                        continue;
                    } else if (pixel_left_neighbour(data, width, height, vector2_create(x, y)) &&
                               pixel_up_neighbour(data, width, height, vector2_create(x, y)) &&
                               pixel_down_neighbour(data, width, height, vector2_create(x, y))) {
                        map->tiles[y * width + x].uv = tile_variant.uv_right;
                        map->tile_count++;
                        continue;
                    } else if (pixel_right_neighbour(data, width, height, vector2_create(x, y)) &&
                               pixel_up_neighbour(data, width, height, vector2_create(x, y)) &&
                               pixel_left_neighbour(data, width, height, vector2_create(x, y))) {
                        map->tiles[y * width + x].uv = tile_variant.uv_down;
                        map->tile_count++;
                        continue;
                    } else if (pixel_right_neighbour(data, width, height, vector2_create(x, y)) &&
                               pixel_down_neighbour(data, width, height, vector2_create(x, y)) &&
                               pixel_left_neighbour(data, width, height, vector2_create(x, y))) {
                        map->tiles[y * width + x].uv = tile_variant.uv_up;
                        map->tile_count++;
                        continue;
                    } else if (pixel_left_neighbour(data, width, height, vector2_create(x, y)) && pixel_up_neighbour(data, width, height, vector2_create(x, y))) {
                        map->tiles[y * width + x].uv = tile_variant.uv_diagonal_right_down;
                        map->tile_count++;
                        continue;
                    } else if (pixel_left_neighbour(data, width, height, vector2_create(x, y)) && pixel_down_neighbour(data, width, height, vector2_create(x, y))) {
                        map->tiles[y * width + x].uv = tile_variant.uv_diagonal_right_up;
                        map->tile_count++;
                        continue;
                    } else if (pixel_right_neighbour(data, width, height, vector2_create(x, y)) && pixel_up_neighbour(data, width, height, vector2_create(x, y))) {
                        map->tiles[y * width + x].uv = tile_variant.uv_diagonal_left_down;
                        map->tile_count++;
                        continue;
                    } else if (pixel_right_neighbour(data, width, height, vector2_create(x, y)) && pixel_down_neighbour(data, width, height, vector2_create(x, y))) {
                        map->tiles[y * width + x].uv = tile_variant.uv_diagonal_left_up;
                        map->tile_count++;
                        continue;
                    } else if (pixel_right_neighbour(data, width, height, vector2_create(x, y)) && pixel_left_neighbour(data, width, height, vector2_create(x, y))) {
                        map->tiles[y * width + x].uv = tile_variant.uv_straight_horizontal;
                        map->tile_count++;
                        continue;
                    } else if (pixel_up_neighbour(data, width, height, vector2_create(x, y)) && pixel_down_neighbour(data, width, height, vector2_create(x, y))) {
                        map->tiles[y * width + x].uv = tile_variant.uv_straight_vertical;
                        map->tile_count++;
                        continue;
                    } else {
                        map->tiles[y * width + x].uv = map->registered_tile_variants[tile_get_variant_index_by_color(map, color)].uv_base_point;
                        map->tile_count++;
                        continue;
                    }
                    if ((pixel_up_neighbour(data, width, height, vector2_create(x, y)) || pixel_down_neighbour(data, width, height, vector2_create(x, y))) &&
                        !pixel_left_neighbour(data, width, height, vector2_create(x, y)) &&
                        !pixel_right_neighbour(data, width, height, vector2_create(x, y))) {
                        map->tiles[y * width + x].uv = tile_variant.uv_left;
                        map->tile_count++;
                    }
                } else {
                    map->tiles[y * width + x].uv = map->registered_tile_variants[tile_get_variant_index_by_color(map, color)].uv_base_point;
                    map->tile_count++;
                }
            } else {
                map->tiles[y * width + x].uv = INVALID_UV;
                map->tile_count++;
            }
        }
    }
    for (i32 y = 0; y < height / TILE_COUNT_IN_CHUNK; y++) {
        for (i32 x = 0; x < width / TILE_COUNT_IN_CHUNK; x++) {
            map->chunk_aabbs[y * width / TILE_COUNT_IN_CHUNK + x] = tile_map_get_chunk_aabb(vector2_create(x, y), *map);
            map->chunk_aabbs_count++;
        }
    }

    stbi_image_free(data);
}

void tile_map_render(tile_map* map, AABB camera_aabb) {
    i32 visible_chunk_indices[MAX_VISIBLE_CHUNK_COUNT] = {};
    u32 visible_chunks_count = 0;
    for (u32 i = 0; i < map->chunk_aabbs_count; i++) {
        if (vector2_distance(camera_aabb.position, map->chunk_aabbs[i].position) <= 1400.0f) {
            if (aabb_collides_with_aabb(map->chunk_aabbs[i], camera_aabb)) {
                visible_chunk_indices[visible_chunks_count++] = i;
            }
        }
    }
    for (u32 i = 0; i < visible_chunks_count; i++) {
        tile_map_chunk_render(map, map->chunk_aabbs[visible_chunk_indices[i]]);
    }
}

AABB tile_map_get_chunk_aabb(vector2 chunk_position, tile_map map) {
    AABB aabb;
    aabb.half_size = vector2_scaler_division(vector2_scaler_multiplication(map.tile_scale, TILE_COUNT_IN_CHUNK), 2.0f);
    aabb.position = vector2_multiply(chunk_position, vector2_scaler_multiplication(aabb.half_size, 2.0f));
    return aabb;
}

void tile_map_chunk_render(tile_map* map, AABB chunk_aabb) {
    vector2 chunk_position_begin = vector2_create(tile_map_chunk_get_grid_position(chunk_aabb).x * TILE_COUNT_IN_CHUNK,
                                                  tile_map_chunk_get_grid_position(chunk_aabb).y * TILE_COUNT_IN_CHUNK);
    for (u32 y = 0; y < TILE_COUNT_IN_CHUNK; y++) {
        for (u32 x = 0; x < TILE_COUNT_IN_CHUNK; x++) {
            u32 tile_index = (chunk_position_begin.y + y) * map->map_size.x + (chunk_position_begin.x + x);
            if (!vector2_compare(map->tiles[tile_index].uv, INVALID_UV)) {
                batch_renderer_render_sprite(vector2_create(aabb_get_min(chunk_aabb).x + (x * map->tile_scale.y),
                                                            aabb_get_min(chunk_aabb).y + (y * map->tile_scale.y)),
                                             map->tile_scale,
                                             0.0f,
                                             map->tiles[tile_index].uv,
                                             map->tile_size,
                                             vector4_create(1.0f, 1.0f, 1.0f, 1.0f), false);
            }
        }
    }
}

vector2 tile_map_chunk_get_grid_position(AABB chunk_aabb) {
    return vector2_divide(chunk_aabb.position, vector2_scaler_multiplication(chunk_aabb.half_size, 2.0f));
}