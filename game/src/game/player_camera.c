#include "player_camera.h"

#include <cengine/core/global_state.h>
#include <cengine/math/matrix_transform.h>
#include <cengine/platform/platform.h>
#include <cengine/core/event_system.h>
#include <cengine/renderer/batch_renderer.h>

player_camera player_camera_create(vector2 position) {
    player_camera ret;
    ret.position = position;
    ret.view_matrix = matrix4_identity();
    return ret;
}

void player_camera_update(player_camera* camera) {
    if (platform_is_key_down(KEY_W)) {
        camera->position.y += 130 * g_state->app->state.delta_time;
    }
    if (platform_is_key_down(KEY_S)) {
        camera->position.y -= 130 * g_state->app->state.delta_time;
    }
    if (platform_is_key_down(KEY_D)) {
        camera->position.x += 130 * g_state->app->state.delta_time;
    }
    if (platform_is_key_down(KEY_A)) {
        camera->position.x -= 130 * g_state->app->state.delta_time;
    }
    matrix4 transform = translate_mv(matrix4_identity(),
                                     vector3_create(camera->position.x, camera->position.y, 0.0f));
    camera->view_matrix = matrix4_inverse(transform);
}