#include "game.h"

#include <cengine/core/event_system.h>
#include <cengine/core/global_state.h>
#include <cengine/core/logging.h>
#include <cengine/renderer/asset_pool.h>
#include <cengine/renderer/batch_renderer.h>
#include <stdlib.h>
#include <string.h>

void game_init() {
    batch_renderer_set_sprite_sheet(asset_pool_load_texture("../game/assets/textures/terrain.png"));
}

void game_update() {
    batch_renderer_begin_render();
    for (u32 y = 3; y < 20; y++) {
        for (u32 x = 3; x < 30; x++) {
            batch_renderer_render_sprite(vector2_create(25.0f * x, 25.0f * y),
                                         vector2_create(25.0f, 25.0f), 0.0f, x % 2 == 0 ? vector2_create(0.0f, 0.0f) : vector2_create(1.0f, 0.0f),
                                         vector2_create(16.0f, 16.0f), vector4_create(1.0f, 1.0f, 1.0f, 1.0f));
        }
    }
    for (u32 y = 16; y < 40; y++) {
        for (u32 x = 16; x < 40; x++) {
            batch_renderer_render_quad(vector2_create(25.0f * x, 25.0f * y),
                                       vector2_create(25.0f, 25.0f), 0.0f, x % 2 == 0 ? vector4_create(0.2f, 0.3f, 0.8f, 1.0f) : vector4_create(0.8f, 0.3f, 0.2f, 1.0f));
        }
    }
    batch_renderer_end_render();
    LOG_INFO("%f", g_state->app->state.delta_time);
}
void game_terminate() {}