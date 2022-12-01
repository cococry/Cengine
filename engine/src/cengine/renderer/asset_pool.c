#include "asset_pool.h"

#include "../core/logging.h"

#include <stdlib.h>

static asset_pool_state s_asset_pool_state;

#define MAX_TEXTURES 512

void asset_pool_init() {
    s_asset_pool_state.textures = malloc(sizeof(texture2d) * MAX_TEXTURES);
    s_asset_pool_state.texture_count = 0;
}

texture2d* asset_pool_load_texture(const char* filepath) {
    for (u32 i = 0; i < s_asset_pool_state.texture_count; i++) {
        if (s_asset_pool_state.textures[i]->filepath == filepath) {
            return s_asset_pool_state.textures[i];
        }
    }
    texture2d* tex = texture2d_create(filepath);
    asset_pool_add_texture(tex);
    return tex;
}

void asset_pool_add_texture(texture2d* texture) {
    s_asset_pool_state.textures[s_asset_pool_state.texture_count++] = texture;
}

void asset_pool_remove_texture(texture2d* texture) {
    ASSERT_MSG(asset_pool_get_texture_index(texture) != -1, "Tried to remove non existent texture from asset pool.");

    free(s_asset_pool_state.textures[asset_pool_get_texture_index(texture)]);
    s_asset_pool_state.textures[asset_pool_get_texture_index(texture)] = nullptr;
}

u32 asset_pool_get_texture_index(texture2d* texture) {
    for (u32 i = 0; i < s_asset_pool_state.texture_count; i++) {
        if (s_asset_pool_state.textures[i]->filepath == texture->filepath) {
            return i;
        }
    }
    return -1;
}