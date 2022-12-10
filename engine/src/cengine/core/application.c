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

#include "../game/game.h"

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
    game_init();

    return ret;
}

void application_run(application* app) {
    while (app->state.running) {
        if (platform_window_close_requested(app->wnd))
            application_stop(app);
        if (platform_is_key_down(KEY_ESCAPE)) {
            application_stop(app);
        }

        platform_window_update(app->wnd);

        render_command_clear_buffers(CNGN_COLOR_BUFFER_BIT);
        render_command_clear_color(0.1f, 0.1f, 0.1f, 1.0f);

        renderer2d_update_objects();
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
    game_terminate();
    free(g_state);
}

void application_stop(application* app) {
    app->state.running = false;
}