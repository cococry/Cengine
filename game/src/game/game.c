#include "game.h"

#include <cengine/renderer/triangle2d.h>
#include <cengine/renderer/renderer2d.h>
#include <cengine/renderer/asset_pool.h>
#include <cengine/core/event_system.h>
#include <cengine/core/global_state.h>
#include <cengine/core/logging.h>
#include <cengine/renderer/tilemap.h>

#include <stdlib.h>

#include "player_camera.h"

static game_state s_state;

static void player_set_move_left_anim(quad* player) {
    sprite_aimation_set_tick_speed(&s_state.player.quad_instc->sprite_anim, 0.05f);
    sprite_animation_set_anim_contiguous_x(&s_state.player.quad_instc->sprite_anim, 0.0f, 4.0f, 0.0f);
    quad_set_sprite_animation(s_state.player.quad_instc, s_state.player.quad_instc->sprite_anim);
}
static void player_set_move_down_anim(quad* player) {
    sprite_aimation_set_tick_speed(&s_state.player.quad_instc->sprite_anim, 0.05f);
    sprite_animation_set_anim_contiguous_x(&s_state.player.quad_instc->sprite_anim, 0.0f, 4.0f, 2.0f);
    quad_set_sprite_animation(s_state.player.quad_instc, s_state.player.quad_instc->sprite_anim);
}
static void player_set_move_up_anim(quad* player) {
    sprite_aimation_set_tick_speed(&s_state.player.quad_instc->sprite_anim, 0.05f);
    sprite_animation_set_anim_contiguous_x(&s_state.player.quad_instc->sprite_anim, 0.0f, 4.0f, 3.0f);
    quad_set_sprite_animation(s_state.player.quad_instc, s_state.player.quad_instc->sprite_anim);
}
static void player_set_move_right_anim(quad* player) {
    sprite_aimation_set_tick_speed(&s_state.player.quad_instc->sprite_anim, 0.05f);
    sprite_animation_set_anim_contiguous_x(&s_state.player.quad_instc->sprite_anim, 0.0f, 4.0f, 1.0f);
    quad_set_sprite_animation(s_state.player.quad_instc, s_state.player.quad_instc->sprite_anim);
}
static void player_set_idle_anim(quad* player) {
    sprite_aimation_set_tick_speed(&s_state.player.quad_instc->sprite_anim, 0.3f);
    sprite_animation_set_anim_contiguous_x(&s_state.player.quad_instc->sprite_anim, 5.0f, 7.0f, 1.0f);
    quad_set_sprite_animation(s_state.player.quad_instc, s_state.player.quad_instc->sprite_anim);
}

static void player_moved_callback(void* data) {
    quad* player = (quad*)data;
    if (player->tag != s_state.player.quad_instc->tag) {
        return;
    }
    if (player->movement.y == -1.0f) {
        player_set_move_down_anim(player);
        return;
    } else if (player->movement.y == 1.0f) {
        player_set_move_up_anim(player);
        return;
    }
    if (player->movement.x == -1.0f) {
        player_set_move_left_anim(player);
        return;
    } else if (player->movement.x == 1.0f) {
        player_set_move_right_anim(player);
        return;
    }

    else if (vector2_compare(player->movement, QUAD_IDLE_VECTOR)) {
        player_set_idle_anim(player);
        return;
    }
}

static void player_update_callback(quad* player) {
    if (platform_is_key_down(KEY_A)) {
        quad_move_x(player, -2.5f);
    } else if (platform_is_key_down(KEY_D)) {
        quad_move_x(player, 2.5f);
    }
    if (platform_is_key_down(KEY_S)) {
        quad_move_y(player, -2.5f);
    } else if (platform_is_key_down(KEY_W)) {
        quad_move_y(player, 2.5f);
    }
}
void game_init() {
    texture2d* player_sprite_atlas = asset_pool_load_texture("../engine/assets/textures/spriteatlas.png");

    sprite_animation anim = sprite_animation_create(player_sprite_atlas, 4, 0.3f, vector2_create(16.0f, 20.0f), vector2_create(1.0f, 1.0f));
    sprite_animation_set_anim_contiguous_x(&anim, 5.0f, 7.0f, 1.0f);
    s_state.player.quad_instc = quad_create("Player", vector2_create((g_state->app->wnd->props.width / 2.0f), g_state->app->wnd->props.height / 2.0f), vector2_create(100.0f, 100.0f), vector2_create(40.0f, 100.0f), 0.0f, 2, vector4_create(1.0f, 1.0f, 1.0f, 1.0f), nullptr);
    s_state.player.quad_instc->texture = player_sprite_atlas;
    quad_set_sprite_animation(s_state.player.quad_instc, anim);
    renderer2d_add_quad(s_state.player.quad_instc);
    s_state.player.quad_instc->update_callback = player_update_callback;
    s_state.player.camera = player_camera_create(vector2_create(0.0f, 0.0f));
    register_event(quad_moved_event, player_moved_callback, "playerMovedCb");

    texture2d* mc_sprite_sheet = asset_pool_load_texture("../engine/assets/textures/mc_spritesheet.png");
    tile_map map = tile_map_create(mc_sprite_sheet, vector2_create(16.0f, 16.0f), vector2_create(50.0f, 50.0f), 0);
    tile_map_register_tile(&map, tile_map_tile_create("g", vector2_create(1.0f, 0.0f)));

    tile_map_commit_to_render_box(&map, "g", vector2_create(0.0f, 0.0f), vector2_create(10.0f, 10.0f));
}

void game_update() {
    player_camera_update(&s_state.player.camera);

    printf("%f\n", g_state->app->state.delta_time);
}
void game_terminate() {
}