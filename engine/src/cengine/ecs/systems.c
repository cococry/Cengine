#include "systems.h"
#include "components.h"

#include "../renderer/batch_renderer.h"
#include "../core/event_system.h"

#include <stdlib.h>

void scene_render_system() {
    ecs_query_result* qr = ecs_query(2, component_type_transform, component_type_sprite);

    for (u32 i = 0; i < qr->count; i++) {
        transform_component* tc = (transform_component*)ecs_get_component(qr->list[i], component_type_transform);
        sprite_component* sc = (sprite_component*)ecs_get_component(qr->list[i], component_type_sprite);
        if (vector2_compare(sc->uv, vector2_create(-1.0f, -1.0f))) {
            batch_renderer_render_quad(tc->position, tc->scale, tc->rotation, sc->color, false);
        } else {
            batch_renderer_render_sprite(tc->position, tc->scale, tc->rotation, sc->uv, sc->cell_size, sc->color, false);
        }
    }
}

void start_moving_system() {
    ecs_query_result* qr = ecs_query(1, component_type_transform);
    for (u32 i = 0; i < qr->count; i++) {
        transform_component* tc = (transform_component*)ecs_get_component(qr->list[i], component_type_transform);
        tc->last_position = tc->position;
    }
}

void end_moving_system() {
    ecs_query_result* qr = ecs_query(1, component_type_transform);
    for (u32 i = 0; i < qr->count; i++) {
        transform_component* tc = (transform_component*)ecs_get_component(qr->list[i], component_type_transform);
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
                tc->moved = true;
            } else {
                tc->moved = false;
            }
        } else {
            vector2 tmp_movement = tc->movement;
            tc->movement = vector2_create(0.0f, 0.0f);
            if (!vector2_compare(tmp_movement, tc->movement)) {
                dispatch_event(entity_moved_event, &qr->list[i]);
                tc->moved = true;
            } else {
                tc->moved = false;
            }
        }
    }
}

camera_component* get_render_camera() {
    ecs_query_result* qr_camera = ecs_query(1, component_type_camera);
    camera_component* selected_cam = nullptr;
    for (u32 i = 0; i < qr_camera->count; i++) {
        camera_component* cc = ecs_get_component(qr_camera->list[i], component_type_camera);
        if (cc->selected_for_rendering) {
            selected_cam = cc;
            break;
        }
    }
    return selected_cam;
}