#include "application.h"
#include "event_system.h"
#include "global_state.h"

#include <stdlib.h>

#include "logging.h"

#include "../renderer/render_command.h"
#include "../renderer/render_defines.h"
#include "../renderer/renderer2d.h"

#include "../platform/opengl/gl_functions.h"

#include "../math/matrix.h"
#include "../math/radians.h"

static quad s_quad;
static quad s_quad2;

static void close_callback(void* data) {
    application* app = (application*)data;
    app->state.running = false;
}

static void resize_callback(void* data) {
    u32* size = (u32*)data;
    u32 width = size[0];
    u32 height = size[1];
    g_state->app->wnd->props.width = width;

    render_command_resize_viewport(width, height);
    shader_program_bind(g_state->app->shader);

    shader_program_upload_mat4(g_state->app->shader, "u_proj",
                               orthographic_matrix(0.0f, 1280.0f, 0.0f, 720.0f));
}

application* application_create(window_properties props) {
    application* ret = malloc(sizeof(application));
    ret->wnd = platform_window_create(props);

    ASSERT_MSG(ret != nullptr || sizeof(ret) != sizeof(application), "Failed to create application.");

    ret->state.running = true;
    init_listeners(event_listeners, max_events);

    register_event(window_close_event, close_callback, "closeCb");
    register_event(window_resize_event, resize_callback, "resizeCb");

    platform_input_init();

    ret->state.delta_time = 0.0f;

    g_state = malloc(sizeof(global_state));
    g_state->app = ret;

    renderer2d_init();

    s_quad = quad_create("Quad1", vector2_create(300.0f, 300.0f),
                         vector2_create(100.0f, 100.0f), 0.0f, vector4_create(1.0f, 1.0f, 1.0f, 1.0f),
                         "../engine/assets/textures/grass.jpg");

    s_quad2 = quad_create("Quad2", vector2_create(300.0f, 400.0f),
                          vector2_create(100.0f, 100.0f), 0.0f, vector4_create(1.0f, 1.0f, 1.0f, 1.0f),
                          "../engine/assets/textures/player.png");

    renderer2d_add_quad(&s_quad);
    renderer2d_add_quad(&s_quad2);

    return ret;
}

void application_run(application* app) {
    while (app->state.running) {
        if (platform_window_close_requested(app->wnd))
            application_stop(app);
        if (platform_is_key_down(KEY_ESCAPE)) {
            application_stop(app);
        }
        quad* quad2 = renderer2d_get_quad_by_tag("Quad2");
        if (quad2 != nullptr) {
            if (platform_is_key_down(KEY_A) && quad2->position.x > 0.0f + (quad2->scale.x / 2.0f)) {
                quad_move_x(quad2, -2.5f);
            }
            if (platform_is_key_down(KEY_D) && quad2->position.x < app->wnd->props.width - (quad2->scale.x / 2.0f)) {
                quad_move_x(quad2, 2.5f);
            }
            if (platform_is_key_down(KEY_S) && quad2->position.y > 0.0f + (quad2->scale.y / 2.0f)) {
                quad_move_y(quad2, -2.5f);
            }
            if (platform_is_key_down(KEY_W) && quad2->position.y < app->wnd->props.height - (quad2->scale.y / 2.0f)) {
                quad_move_y(quad2, 2.5f);
            }
        }

        platform_window_update(app->wnd);

        render_command_clear_buffers(CNGN_COLOR_BUFFER_BIT);
        render_command_clear_color(0.1f, 0.1f, 0.1f, 1.0f);

        renderer2d_render_quads();

        platform_input_update();
    }
}

void application_shutdown(application* app) {
    app->state.running = false;
    platform_window_destroy(app->wnd);
    destroy_listeners(event_listeners, max_events);
    renderer2d_terminate();
    platform_input_shutdown();
    free(g_state);
}

void application_stop(application* app) {
    app->state.running = false;
}