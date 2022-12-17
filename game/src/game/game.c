#include "game.h"

#include <cengine/renderer/triangle2d.h>
#include <cengine/renderer/renderer2d.h>
#include <cengine/renderer/asset_pool.h>
#include <cengine/core/event_system.h>
#include <cengine/core/global_state.h>
#include <cengine/core/logging.h>
#include <cengine/renderer/tilemap.h>

#include <stdlib.h>
#include <string.h>

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
    if (quad_collding_with_quad(player, s_state.door)) {
        if (strcmp(renderer2d_get_state().active_scene->name, "MainScene") == 0) {
            renderer2d_switch_active_scene_by_name("GrassScene");
            s_state.door->position = vector2_create(50.0f, 50.0f);
            return;
        } else if (strcmp(renderer2d_get_state().active_scene->name, "GrassScene") == 0) {
            renderer2d_switch_active_scene_by_name("MainScene");
            s_state.door->position = vector2_create(700.0f, 700.0f);
            return;
        }
    }
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
    scene* main_scene = scene_create_empty("MainScene");
    renderer2d_set_active_scene(main_scene);

    texture2d* player_sprite_atlas = asset_pool_load_texture("../engine/assets/textures/spriteatlas.png");

    sprite_animation anim = sprite_animation_create(player_sprite_atlas, 4, 0.3f, vector2_create(16.0f, 20.0f), vector2_create(1.0f, 1.0f));
    sprite_animation_set_anim_contiguous_x(&anim, 5.0f, 7.0f, 1.0f);
    s_state.player.quad_instc = quad_create("Player", vector2_create((g_state->app->wnd->props.width / 2.0f), g_state->app->wnd->props.height / 2.0f), vector2_create(100.0f, 100.0f), vector2_create(40.0f, 100.0f), 0.0f, 2, vector4_create(1.0f, 1.0f, 1.0f, 1.0f), nullptr);
    s_state.player.quad_instc->texture = player_sprite_atlas;
    quad_set_sprite_animation(s_state.player.quad_instc, anim);
    renderer2d_add_quad(s_state.player.quad_instc);
    s_state.player.quad_instc->update_callback = player_update_callback;
    s_state.player.camera = player_camera_create(vector2_create(0.0f, 0.0f), s_state.player.quad_instc);

    s_state.door = quad_create("Door", vector2_create(50.0f, 50.0f), vector2_create(100.0f, 100.0f), vector2_create(100.0f, 100.0f), 0.0f,
                               2, vector4_create(1.0f, 1.0f, 1.0f, 1.0f), nullptr);
    quad_load_texture(s_state.door, "../engine/assets/textures/door.png");
    renderer2d_add_quad(s_state.door);

    register_event(quad_moved_event, player_moved_callback, "playerMovedCb");

    texture2d* mc_sprite_sheet = asset_pool_load_texture("../engine/assets/textures/mc_spritesheet.png");

    tile_map map = tile_map_create(mc_sprite_sheet, vector2_create(16.0f, 16.0f), vector2_create(50.0f, 50.0f), 0);
    tile_map_register_tile(&map, tile_map_tile_create("grass", vector2_create(1.0f, 0.0f)));
    tile_map_commit_to_render_box(&map, "grass", vector2_create(0.0f, 0.0f), vector2_create(10.0f, 10.0f));

    tile_map map2 = tile_map_create(mc_sprite_sheet, vector2_create(16.0f, 16.0f), vector2_create(50.0f, 50.0f), 1);
    tile_map_register_tile(&map2, tile_map_tile_create("stone", vector2_create(5.0f, 0.0f)));
    tile_map_commit_to_render_box(&map2, "stone", vector2_create(2.0f, 2.0f), vector2_create(8.0f, 8.0f));

    {
        scene* grass_scene = scene_create_empty("GrassScene");
        renderer2d_set_active_scene(grass_scene);

        texture2d* mc_sprite_sheet = asset_pool_load_texture("../engine/assets/textures/mc_spritesheet.png");
        tile_map map = tile_map_create(mc_sprite_sheet, vector2_create(16.0f, 16.0f), vector2_create(50.0f, 50.0f), 0);
        tile_map_register_tile(&map, tile_map_tile_create("grass", vector2_create(1.0f, 0.0f)));
        tile_map_commit_to_render_box(&map, "grass", vector2_create(0.0f, 0.0f), vector2_create(15.0f, 15.0f));
        renderer2d_add_quad(s_state.player.quad_instc);
        renderer2d_add_quad(s_state.door);
    }
    renderer2d_set_player_quad(s_state.player.quad_instc);
}

void game_update() {
    player_camera_update(&s_state.player.camera);
}
void game_terminate() {
}