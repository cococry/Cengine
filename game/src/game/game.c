#include "game.h"

#include <cengine/core/event_system.h>
#include <cengine/core/global_state.h>
#include <cengine/core/logging.h>
#include <cengine/renderer/asset_pool.h>
#include <cengine/renderer/batch_renderer.h>
#include <cengine/renderer/tilemap.h>
#include <stdlib.h>
#include <string.h>

#include "player_camera.h"

#include <cengine/ecs/ecs.h>
#include <cengine/ecs/components.h>
#include <cengine/renderer/sprite_animation.h>

static entity player;
static sprite_animation anim;
static tile_map map;
static vector3 position;

static void entity_moved_callback(void* data) {
    u32 entty_id = *(u32*)data;
    transform_component* tc = ecs_get_component(entty_id, 0);
    if (tc->movement.x == 1.0f) {
        anim.tick_speed = 0.075f;
        sprite_animation_set_anim_contigues_x(&anim, 8, 16, 0, 8);
    }
    if (tc->movement.x == -1.0f) {
        anim.tick_speed = 0.075f;
        sprite_animation_set_anim_contigues_x(&anim, 16.0f, 24.0f, 0.0f, 8);
    }
    if (tc->movement.x == 0.0f && tc->movement.y == 0.0f) {
        anim.tick_speed = 0.5f;
        sprite_animation_set_anim_contigues_x(&anim, 0.0f, 4.0f, 0.0f, 4);
    }
}

void game_init() {
    batch_renderer_set_sprite_sheet(asset_pool_load_texture("../game/assets/textures/terrain.png"));

    player = entity_create();
    transform_component tc = transform_component_create(vector2_create(500.0f, 500.0f), vector2_create(100.0f, 100.0f), 0.0f);
    sprite_component sc = sprite_component_create(vector2_create(1.0f, 0.0f), vector2_create(24.0f, 32.0f), vector4_create(1.0f, 1.0f, 1.0f, 1.0f));
    ecs_add_component(player.id, 0, &tc);
    ecs_add_component(player.id, 1, &sc);

    anim = sprite_animaton_create_empty(4, 0.5f, vector2_create(24.0f, 32.0f));
    sprite_animation_set_anim_contigues_x(&anim, 0.0f, 4.0f, 0.0f, 4);

    register_event(entity_moved_event, entity_moved_callback, "entityMovedCb");

    map = tile_map_create(vector2_create(16.0f, 16.0f), vector2_create(20.0f, 20.0f), vector2_create(300.0f, 300.0f));

    tile_map_register_tile(&map, registered_tile_create(vector4_create(0.0f, 0.0f, 255.0f, 255.0f), vector2_create(3.0f, 0.0f)));
    tile_map_register_tile(&map, registered_tile_create(vector4_create(0.0f, 255.0f, 0.0f, 255.0f), vector2_create(1.0f, 0.0f)));
    tile_map_register_tile(&map, registered_tile_create(vector4_create(0.0f, 0.0f, 0.0f, 255.0f), vector2_create(0.0f, 0.0f)));
    tile_map_register_tile(&map, registered_tile_create(vector4_create(255.0f, 255.0f, 255.0f, 255.0f), vector2_create(4.0f, 0.0f)));

    tile_map_load_from_file(&map, "../game/assets/textures/tilemap_test.png");

    position = vector3_create(0.0f, 0.0f, 0.0f);
}

void start_moving_system() {
    ecs_query_result* qr = ecs_query(1, 0);
    for (u32 i = 0; i < qr->count; i++) {
        transform_component* tc = (transform_component*)ecs_get_component(qr->list[i], 0);
        tc->last_position = tc->position;
    }
}

void end_moving_system() {
    ecs_query_result* qr = ecs_query(1, 0);
    for (u32 i = 0; i < qr->count; i++) {
        transform_component* tc = (transform_component*)ecs_get_component(qr->list[i], 0);
        if (!vector2_compare(tc->last_position, tc->position)) {
            vector2 tmp_movement = tc->movement;
            if (tc->position.x > tc->last_position.x) {
                tc->movement.x = 1.0f;
            } else if (tc->position.x < tc->last_position.x) {
                tc->movement.x = -1.0f;
            }
            if (tc->position.y > tc->last_position.y) {
                tc->movement.y = 1.0f;
            } else if (tc->position.y < tc->last_position.y) {
                tc->movement.y = -1.0f;
            }
            if (!vector2_compare(tmp_movement, tc->movement)) {
                dispatch_event(entity_moved_event, &qr->list[i]);
            }
        } else {
            vector2 tmp_movement = tc->movement;
            tc->movement = vector2_create(0.0f, 0.0f);
            if (!vector2_compare(tmp_movement, tc->movement)) {
                dispatch_event(entity_moved_event, &qr->list[i]);
            }
        }
    }
}

void game_update() {
    if (platform_is_key_down(KEY_A)) {
        position.x += 200.0f * g_state->app->state.delta_time;
    }
    if (platform_is_key_down(KEY_D)) {
        position.x -= 200.0f * g_state->app->state.delta_time;
    }
    if (platform_is_key_down(KEY_S)) {
        position.y += 200.0f * g_state->app->state.delta_time;
    }
    if (platform_is_key_down(KEY_W)) {
        position.y -= 200.0f * g_state->app->state.delta_time;
    }
    matrix4 view = translate_mv(matrix4_identity(), position);
    batch_renderer_begin_render(view);
    tile_map_render(&map);
    batch_renderer_end_render();
    start_moving_system();

    sprite_animation_tick_on_entity(&anim, player);
    end_moving_system();
}
void game_terminate() {
    sprite_animation_delete(&anim);
}