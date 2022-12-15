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
}

game_callbacks game_callbacks_create() {
    game_callbacks ret;
    ret.game_init_cb = nullptr;
    ret.game_update_cb = nullptr;
    ret.game_terminate_cb = nullptr;
    return ret;
}
application* application_create(window_properties props, game_callbacks game_cbs) {
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
    ret->game_cbs = game_cbs;

    g_state = malloc(sizeof(global_state));
    g_state->app = ret;

    asset_pool_init();
    renderer2d_init();
    if (ret->game_cbs.game_init_cb != nullptr)
        ret->game_cbs.game_init_cb();

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
        render_command_clear_color(RGB_COLOR(69, 69, 68), 1.0f);

        renderer2d_update_objects();
        renderer2d_render_objects();

        if (app->game_cbs.game_update_cb != nullptr)
            app->game_cbs.game_update_cb();

        platform_input_update();
    }
}

void application_shutdown(application* app) {
    app->state.running = false;
    platform_window_destroy(app->wnd);
    destroy_listeners(event_listeners, max_events);
    renderer2d_terminate();
    platform_input_shutdown();
    if (app->game_cbs.game_terminate_cb != nullptr)
        app->game_cbs.game_terminate_cb();
    free(g_state);
}

void application_stop(application* app) {
    app->state.running = false;
}