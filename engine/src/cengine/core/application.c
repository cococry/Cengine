#include "application.h"
#include "event_system.h"
#include "global_state.h"

#include <stdlib.h>
#include <time.h>

#include "logging.h"

#include "../renderer/render_command.h"
#include "../renderer/render_defines.h"
#include "../renderer/renderer2d.h"
#include "../renderer/asset_pool.h"

#include "../platform/opengl/gl_functions.h"

#include "../math/matrix.h"
#include "../math/radians.h"

static void close_callback(void* data) {
    application* app = (application*)data;
    app->state.running = false;
}

static void resize_callback(void* data) {
    u32* size = (u32*)data;
    u32 width = size[0];
    u32 height = size[1];

    render_command_resize_viewport(width, height);
    shader_program_bind(g_state->app->shader);
}

application* application_create(window_properties props) {
    srand(time(0));
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

    asset_pool_init();
    renderer2d_init();

    quad* player = quad_create("Player", vector2_create(200.0f, 200.0f), vector2_create(100.0f, 100.0f),
                               0.0f, vector4_create(1.0f, 1.0f, 1.0f, 1.0f));
    quad_load_texture(player, "../engine/assets/textures/player.png");

    for (u32 x = 0; x < 55; x++) {
        triangle* spike = triangle_create("SpikeBlue", vector2_create(25.0f + x * 50.0f, 0.0f + 50.0f), vector2_create(50.0f, 50.0f),
                                          0.0f, vector2_create(-0.5f, -0.5f),
                                          vector2_create(0.0f, 0.5f), vector2_create(0.5f, -0.5f), vector4_create(0.2f, 0.3f, 0.8f, 1.0f));
        renderer2d_add_triangle(spike);
    }
    for (u32 x = 0; x < 55; x++) {
        triangle* spike = triangle_create("SpikeRed", vector2_create(25.0f + x * 50.0f, 1390.0f), vector2_create(50.0f, 50.0f),
                                          0.0f, vector2_create(-0.5f, 0.5f),
                                          vector2_create(0.0f, -0.5f), vector2_create(0.5f, 0.5f), vector4_create(0.8f, 0.3f, 0.2f, 1.0f));
        renderer2d_add_triangle(spike);
    }

    quad* background = quad_create("Background", vector2_create(50.0f, 50.0f), vector2_create(1280.0f, 720.0f), 0.0f, vector4_create(1.0f, 1.0f, 1.0f, 1.0f));
    quad_load_texture(background, "../engine/assets/textures/background.jpg");
    renderer2d_add_quad(background);
    
    renderer2d_add_quad(player);
    return ret;
}

void application_run(application* app) {
    while (app->state.running) {
        if (platform_window_close_requested(app->wnd))
            application_stop(app);
        if (platform_is_key_down(KEY_ESCAPE)) {
            application_stop(app);
        }
        quad* player = renderer2d_get_quad_by_tag("Player");
        if (player != nullptr) {
            if (platform_is_key_down(KEY_A) && player->position.x > 0.0f + (player->scale.x / 2.0f)) {
                quad_move_x(player, -2.5f);
            }
            if (platform_is_key_down(KEY_D) && player->position.x < app->wnd->props.width - (player->scale.x / 2.0f)) {
                quad_move_x(player, 2.5f);
            }
            if (platform_is_key_down(KEY_S) && player->position.y > 0.0f + (player->scale.y / 2.0f)) {
                quad_move_y(player, -2.5f);
            }
            if (platform_is_key_down(KEY_W) && player->position.y < app->wnd->props.height - (player->scale.y / 2.0f)) {
                quad_move_y(player, 2.5f);
            }
        }

        platform_window_update(app->wnd);

        render_command_clear_buffers(CNGN_COLOR_BUFFER_BIT);
        render_command_clear_color(0.1f, 0.1f, 0.1f, 1.0f);

        renderer2d_render_objects();

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