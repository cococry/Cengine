#pragma once

#include "../core/defines.h"

#include "texture2d.h"

typedef struct asset_pool_state {
    texture2d** textures;
    u32 texture_count;
} asset_pool_state;

void asset_pool_init();

texture2d* asset_pool_load_texture(const char* filepath);

void asset_pool_add_texture(texture2d* texture);

void asset_pool_remove_texture(texture2d* texture);

u32 asset_pool_get_texture_index(texture2d* texture);