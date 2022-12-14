#include "game.h"

#include "../renderer/triangle2d.h"
#include "../renderer/renderer2d.h"
#include "../renderer/asset_pool.h"
#include "../core/event_system.h"

#include "player_camera.h"

#include "../core/global_state.h"

#include <stdlib.h>

#include "../core/logging.h"

static game_state s_state;

static player_camera camera;

static void player_set_move_left_anim(quad* player) {
    s_state.player->sprite_anim.tick_speed = 0.05f;
    sprite_animation_set_anim_contiguous_x(&s_state.player->sprite_anim, 0.0f, 4.0f, 0.0f);
    quad_set_sprite_animation(s_state.player, s_state.player->sprite_anim);
}
static void player_set_move_down_anim(quad* player) {
    s_state.player->sprite_anim.tick_speed = 0.05f;
    sprite_animation_set_anim_contiguous_x(&s_state.player->sprite_anim, 0.0f, 4.0f, 2.0f);
    quad_set_sprite_animation(s_state.player, s_state.player->sprite_anim);
}
static void player_set_move_up_anim(quad* player) {
    s_state.player->sprite_anim.tick_speed = 0.05f;
    sprite_animation_set_anim_contiguous_x(&s_state.player->sprite_anim, 0.0f, 4.0f, 3.0f);
    quad_set_sprite_animation(s_state.player, s_state.player->sprite_anim);
}
static void player_set_move_right_anim(quad* player) {
    s_state.player->sprite_anim.tick_speed = 0.05f;
    sprite_animation_set_anim_contiguous_x(&s_state.player->sprite_anim, 0.0f, 4.0f, 1.0f);
    quad_set_sprite_animation(s_state.player, s_state.player->sprite_anim);
}
static void player_set_idle_anim(quad* player) {
    texture2d* sprite_atlas = asset_pool_load_texture("../engine/assets/textures/spriteatlas.png");
    sprite_animation anim = sprite_animation_create(sprite_atlas, 4, 0.3f, vector2_create(16.0f, 20.0f), vector2_create(1.0f, 1.0f));
    sprite_animation_set_anim_contiguous_x(&anim, 5.0f, 7.0f, 1.0f);
    quad_set_sprite_animation(s_state.player, anim);
}

static void player_moved_callback(void* data) {
    quad* player = (quad*)data;
    if (player->tag != s_state.player->tag) {
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
    texture2d* sprite_atlas = asset_pool_load_texture("../engine/assets/textures/spriteatlas.png");

    sprite_animation anim = sprite_animation_create(sprite_atlas, 4, 0.3f, vector2_create(16.0f, 20.0f), vector2_create(1.0f, 1.0f));
    sprite_animation_set_anim_contiguous_x(&anim, 5.0f, 7.0f, 1.0f);
    s_state.player = quad_create("Player", vector2_create((g_state->app->wnd->props.width / 2.0f), g_state->app->wnd->props.height / 2.0f), vector2_create(100.0f, 100.0f), vector2_create(40.0f, 100.0f), 0.0f, 2, vector4_create(1.0f, 1.0f, 1.0f, 1.0f), nullptr);
    s_state.player->texture = sprite_atlas;
    quad_set_sprite_animation(s_state.player, anim);

    s_state.background = quad_create("Background", vector2_create(50.0f, 50.0f), vector2_create(1280.0f, 720.0f), vector2_create(1280.0f, 720.0f), 0.0f, 0, vector4_create(1.0f, 1.0f, 1.0f, 1.0f), nullptr);
    quad_load_texture(s_state.background, "../engine/assets/textures/grass_bg.png");
    renderer2d_add_quad(s_state.background);

    renderer2d_add_quad(s_state.player);
    s_state.player->update_callback = player_update_callback;

    camera = player_camera_create(vector2_create(
                                      0.0f, 0.0f),
                                  s_state.player);

    quad* chest = quad_create("Chest", vector2_create(300.0f, 200.0f), vector2_create(100.0f, 100.0f), vector2_create(100.0f, 100.0f), 0.0f, 1, vector4_create(1.0f, 1.0f, 1.0f, 1.0f), nullptr);
    quad_load_texture(chest, "../engine/assets/textures/chest.png");
    renderer2d_add_quad(chest);

    register_event(quad_moved_event, player_moved_callback, "playerMovedCb");
}

void game_update() {
    player_camera_update(&camera);
}
void game_terminate() {
}