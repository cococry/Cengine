#include "game.h"

#include <cengine/core/event_system.h>
#include <cengine/core/global_state.h>
#include <cengine/core/logging.h>
#include <cengine/renderer/asset_pool.h>
#include <cengine/renderer/batch_renderer.h>
#include <cengine/renderer/tilemap.h>
#include <cengine/physics/phys_def.h>
#include <cengine/core/util.h>
#include <cengine/ecs/ecs.h>
#include <cengine/ecs/components.h>
#include <cengine/ecs/systems.h>
#include <cengine/renderer/sprite_animation.h>
#include <cengine/ui/ui_core.h>

#include "player_camera.h"

#include <stdlib.h>
#include <string.h>
#include <time.h>

static entity player;
static tile_map map;

static tile_map map_water;
static entity cam;
static AABB camera_aabb;

void game_init() {
    camera_aabb = aabb_create(vector2_create((g_state->app->wnd->props.width / 2.0f) + 100.0f, (g_state->app->wnd->props.height / 2.0f) + 100.0f),
                              vector2_create((g_state->app->wnd->props.width / 2.0f), (g_state->app->wnd->props.height / 2.0f)));
    batch_renderer_set_sprite_sheet(asset_pool_load_texture("../game/assets/textures/terrain.png"));

    player = entity_create();
    transform_component tc = transform_component_create(vector2_create(1000.0f, 500.0f), vector2_create(100.0f, 100.0f), 0.0f);
    sprite_component sc = sprite_component_create_sprite(vector2_create(0.0f, 0.0f), vector2_create(16.0f, 16.0f), vector4_create(1.0f, 1.0f, 1.0f, 1.0f));
    ecs_add_component(player.id, component_type_transform, &tc);
    ecs_add_component(player.id, component_type_sprite, &sc);

    map = tile_map_create(vector2_create(16.0f, 16.0f), vector2_create(20.0f, 20.0f), vector2_create(0.0f, 0.0f));

    registered_tile_variant tile_variants[] = {
        registered_tile_variant_create(vector4_create(0.0f, 255.0f, 0.0f, 255.0), vector2_create(1.0f, 5.0f)),
        registered_tile_variant_create(vector4_create(0.0f, 0.0f, 0.0f, 255.0), vector2_create(6.0f, 2.0f))};
    tile_map_register_tile_variants(&map, tile_variants, 2);

    tile_map_load_from_file(&map, "../game/assets/textures/tilemap.png");

    map_water = tile_map_create(vector2_create(16.0f, 16.0f), vector2_create(20.0f, 20.0f), vector2_create(0.0f, 0.0f));

    registered_tile_variant variants_water_map[] = {
        registered_tile_variant_create(vector4_create(0.0f, 0.0f, 255.0f, 255.0), vector2_create(1.0f, 2.0f)),
        registered_tile_variant_create(vector4_create(0.0f, 0.0f, 0.0f, 255.0), vector2_create(6.0f, 2.0f)),
    };
    tile_map_register_tile_variants(&map_water, variants_water_map, 2);

    tile_map_load_from_file(&map_water, "../game/assets/textures/tilemap_water.png");

    cam = entity_create();
    camera_component cc = camera_component_create(vector2_create(0.0f, 0.0f), true);
    ecs_add_component(cam.id, component_type_camera, &cc);
}

void game_update() {
    transform_component* player_ts = ecs_get_component(player.id, component_type_transform);

    camera_component* cc = ecs_get_component(cam.id, component_type_camera);
    if (platform_is_key_down(KEY_A)) {
        cc->position.x += 200.0f * g_state->app->state.delta_time;
        player_ts->position.x -= 200.0f * g_state->app->state.delta_time;
        camera_aabb.position.x -= 200.0f * g_state->app->state.delta_time;
    }
    if (platform_is_key_down(KEY_D)) {
        cc->position.x -= 200.0f * g_state->app->state.delta_time;
        player_ts->position.x += 200.0f * g_state->app->state.delta_time;
        camera_aabb.position.x += 200.0f * g_state->app->state.delta_time;
    }
    if (platform_is_key_down(KEY_S)) {
        cc->position.y += 200.0f * g_state->app->state.delta_time;
        player_ts->position.y -= 200.0f * g_state->app->state.delta_time;
        camera_aabb.position.y -= 200.0f * g_state->app->state.delta_time;
    }
    if (platform_is_key_down(KEY_W)) {
        cc->position.y -= 200.0f * g_state->app->state.delta_time;
        player_ts->position.y += 200.0f * g_state->app->state.delta_time;
        camera_aabb.position.y += 200.0f * g_state->app->state.delta_time;
    }

    cc->view = translate_mv(matrix4_identity(), vector3_create(cc->position.x, cc->position.y, 0.0f));
    batch_renderer_begin_render(cc->view);
    tile_map_render(&map, camera_aabb);
    tile_map_render(&map_water, camera_aabb);
    batch_renderer_end_render();
}
void game_terminate() {
}
