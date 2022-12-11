#include "game.h"

#include "../renderer/triangle2d.h"
#include "../renderer/renderer2d.h"
#include "../renderer/asset_pool.h"

#include "player_camera.h"

#include "../core/global_state.h"

#include <stdlib.h>

static game_state s_state;

static vector2 player_movement;
static texture2d* sprite_atlas;
static vector2 last_player_pos;
static vector2* player_anim_tex_coords;
static vector2* idle_anim_tex_coords;
static bool8 player_moving_left = false;
static bool8 player_moving_right = false;
static bool8 player_moving_up = false;
static bool8 player_moving_down = false;
static bool8 player_idle = false;

static player_camera camera;

static void
player_set_move_left_anim(quad* player) {
    player_anim_tex_coords[0] = vector2_create(0.0f, 0.0f);
    player_anim_tex_coords[1] = vector2_create(1.0f, 0.0f);
    player_anim_tex_coords[2] = vector2_create(2.0f, 0.0f);
    player_anim_tex_coords[3] = vector2_create(3.0f, 0.0f);
    player_anim_tex_coords[4] = vector2_create(4.0f, 0.0f);
    sprite_animation anim = sprite_animation_create(sprite_atlas, player_anim_tex_coords, 0.05f, 4, vector2_create(16.0f, 20.0f), vector2_create(1.0f, 1.0f));
    quad_set_sprite_animation(s_state.player, anim, false);
}
static void player_set_move_down_anim(quad* player) {
    player_anim_tex_coords[0] = vector2_create(0.0f, 2.0f);
    player_anim_tex_coords[1] = vector2_create(1.0f, 2.0f);
    player_anim_tex_coords[2] = vector2_create(2.0f, 2.0f);
    player_anim_tex_coords[3] = vector2_create(3.0f, 2.0f);
    player_anim_tex_coords[4] = vector2_create(4.0f, 2.0f);
    sprite_animation anim = sprite_animation_create(sprite_atlas, player_anim_tex_coords, 0.05f, 4, vector2_create(16.0f, 20.0f), vector2_create(1.0f, 1.0f));
    quad_set_sprite_animation(s_state.player, anim, false);
}
static void player_set_move_up_anim(quad* player) {
    player_anim_tex_coords[0] = vector2_create(0.0f, 3.0f);
    player_anim_tex_coords[1] = vector2_create(1.0f, 3.0f);
    player_anim_tex_coords[2] = vector2_create(2.0f, 3.0f);
    player_anim_tex_coords[3] = vector2_create(3.0f, 3.0f);
    player_anim_tex_coords[4] = vector2_create(4.0f, 3.0f);
    sprite_animation anim = sprite_animation_create(sprite_atlas, player_anim_tex_coords, 0.05f, 4, vector2_create(16.0f, 20.0f), vector2_create(1.0f, 1.0f));
    quad_set_sprite_animation(s_state.player, anim, false);
}
static void player_set_move_right_anim(quad* player) {
    player_anim_tex_coords[0] = vector2_create(0.0f, 1.0f);
    player_anim_tex_coords[1] = vector2_create(1.0f, 1.0f);
    player_anim_tex_coords[2] = vector2_create(2.0f, 1.0f);
    player_anim_tex_coords[3] = vector2_create(3.0f, 1.0f);
    player_anim_tex_coords[4] = vector2_create(4.0f, 1.0f);
    sprite_animation anim = sprite_animation_create(sprite_atlas, player_anim_tex_coords, 0.05f, 4, vector2_create(16.0f, 20.0f), vector2_create(1.0f, 1.0f));
    quad_set_sprite_animation(s_state.player, anim, false);
}
static void player_set_idle_anim(quad* player) {
    idle_anim_tex_coords[0] = vector2_create(5.0f, 1.0f);
    idle_anim_tex_coords[1] = vector2_create(6.0f, 1.0f);
    idle_anim_tex_coords[2] = vector2_create(7.0f, 1.0f);
    sprite_animation anim = sprite_animation_create(sprite_atlas, idle_anim_tex_coords, 0.3f, 2, vector2_create(16.0f, 20.0f), vector2_create(1.0f, 1.0f));
    quad_set_sprite_animation(s_state.player, anim, false);
}

static void player_update_callback(quad* player) {
    if (platform_is_key_down(KEY_A)) {
        quad_move_x(player, -2.5f);
        player_movement.x = -1.0f;
        if (!player_moving_left) {
            player_set_move_left_anim(player);
            player_moving_left = true;
            player_moving_right = false;
            player_moving_down = false;
            player_moving_up = false;
            player_idle = false;
        }
    }
    if (platform_is_key_down(KEY_D)) {
        quad_move_x(player, 2.5f);
        player_movement.x = 1.0f;
        if (!player_moving_right) {
            player_set_move_right_anim(player);
            player_moving_right = true;
            player_moving_left = false;
            player_moving_down = false;
            player_moving_up = false;
            player_idle = false;
        }
    }
    if (platform_is_key_down(KEY_S)) {
        quad_move_y(player, -2.5f);
        player_movement.y = -1.0f;
        if (!player_moving_down) {
            player_set_move_down_anim(player);
            player_moving_down = true;
            player_moving_left = false;
            player_moving_right = false;
            player_moving_up = false;
            player_idle = false;
        }
    }
    if (platform_is_key_down(KEY_W)) {
        quad_move_y(player, 2.5f);
        player_movement.y = 1.0f;
        if (!player_moving_up) {
            player_set_move_up_anim(player);
            player_moving_up = true;
            player_moving_right = false;
            player_moving_left = false;
            player_moving_down = false;
            player_idle = false;
        }
    }
    if (vector2_compare(player->position, last_player_pos)) {
        player_movement.x = 0.0f;
        player_movement.y = 0.0f;
        if (!player_idle) {
            player_idle = true;
            player_moving_left = false;
            player_moving_right = false;
            player_moving_down = false;
            player_moving_up = false;
            player_set_idle_anim(player);
        }
    }
    last_player_pos = player->position;
}
void game_init() {
    sprite_atlas = asset_pool_load_texture("../engine/assets/textures/spriteatlas.png");
    player_anim_tex_coords = malloc(sizeof(vector2) * 5);
    idle_anim_tex_coords = malloc(sizeof(vector2) * 3);
    player_anim_tex_coords[0] = vector2_create(0.0f, 0.0f);
    player_anim_tex_coords[1] = vector2_create(1.0f, 0.0f);
    player_anim_tex_coords[2] = vector2_create(2.0f, 0.0f);
    player_anim_tex_coords[3] = vector2_create(3.0f, 0.0f);
    player_anim_tex_coords[4] = vector2_create(4.0f, 0.0f);
    sprite_animation anim = sprite_animation_create(sprite_atlas, player_anim_tex_coords, 0.05f, 4, vector2_create(16.0f, 20.0f), vector2_create(1.0f, 1.0f));
    s_state.player = quad_create("Player", vector2_create((g_state->app->wnd->props.width / 2.0f), g_state->app->wnd->props.height / 2.0f), vector2_create(100.0f, 100.0f), 0.0f, vector4_create(1.0f, 1.0f, 1.0f, 1.0f), nullptr);
    s_state.player->texture = sprite_atlas;
    player_movement = vector2_create(0.0f, 0.0f);
    last_player_pos = s_state.player->position;
    quad_set_sprite_animation(s_state.player, anim, false);

    s_state.background = quad_create("Background", vector2_create(50.0f, 50.0f), vector2_create(1280.0f, 720.0f), 0.0f, vector4_create(1.0f, 1.0f, 1.0f, 1.0f), nullptr);
    quad_load_texture(s_state.background, "../engine/assets/textures/grass_bg.png");
    renderer2d_add_quad(s_state.background);

    renderer2d_add_quad(s_state.player);
    s_state.player->update_callback = player_update_callback;

    camera = player_camera_create(vector2_create(
                                      0.0f, 0.0f),
                                  s_state.player);
}

void game_update() {
    player_camera_update(&camera);
}
void game_terminate() {
    free(player_anim_tex_coords);
    free(idle_anim_tex_coords);
}