#include "player_camera.h"

#include "../core/global_state.h"

#include "../math/matrix_transform.h"

#include "../platform/platform.h"

player_camera player_camera_create(vector2 position, quad* player) {
    player_camera ret;
    ret.position = position;
    ret.player = player;
    ret.view_matrix = matrix4_identity();
    return ret;
}

void player_camera_update(player_camera* camera) {
    matrix4 transform = translate_mv(matrix4_identity(),
                                     vector3_create(camera->position.x, camera->position.y, 0.0f));
    camera->view_matrix = matrix4_inverse(transform);
    shader_program_bind(g_state->app->shader);
    shader_program_upload_mat4(g_state->app->shader, "u_view", camera->view_matrix);

    if (platform_is_key_down(KEY_A)) {
        camera->position.x -= 2.5f * 100.0f * g_state->app->state.delta_time;
    } else if (platform_is_key_down(KEY_D)) {
        camera->position.x += 2.5f * 100.0f * g_state->app->state.delta_time;
    }
    if (platform_is_key_down(KEY_W)) {
        camera->position.y += 2.5f * 100.0f * g_state->app->state.delta_time;
    } else if (platform_is_key_down(KEY_S)) {
        camera->position.y -= 2.5f * 100.0f * g_state->app->state.delta_time;
    }
}