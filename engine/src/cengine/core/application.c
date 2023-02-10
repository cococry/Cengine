#include "application.h"
#include "event_system.h"
#include "global_state.h"

#include <stdlib.h>
#include <time.h>

#include "logging.h"

#include "../renderer/render_command.h"
#include "../renderer/render_defines.h"
#include "../renderer/asset_pool.h"
#include "../renderer/batch_renderer.h"

#include "../platform/opengl/gl_functions.h"
#include "../math/matrix.h"
#include "../math/radians.h"

#include "../ecs/ecs.h"
#include "../ecs/components.h"
#include "../ecs/systems.h"

#include "../ui/ui_core.h"

static void
close_callback(void* data) {
    application* app = (application*)data;
    app->state.running = false;
}

static void resize_callback(void* data) {
    u32* size = (u32*)data;
    u32 width = size[0];
    u32 height = size[1];

    render_command_resize_viewport(width, height);
    if (width == 0 || height == 0) {
        g_state->app->state.minimized = true;
    } else {
        g_state->app->state.minimized = false;
    }
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

    ret->state.delta_time = 0.0f;
    ret->state.minimized = false;
    ret->game_cbs = game_cbs;

    g_state = malloc(sizeof(global_state));
    g_state->app = ret;
    g_state->entity_count = 0;

    asset_pool_init();
    platform_input_init();
    batch_renderer_init();
    ui_init();
    ecs_init(3, sizeof(transform_component), sizeof(sprite_component), sizeof(camera_component));

    if (ret->game_cbs.game_init_cb != nullptr)
        ret->game_cbs.game_init_cb();

    return ret;
}

void application_run(application* app) {
    while (app->state.running) {
        if (!app->state.minimized) {
            if (platform_window_close_requested(app->wnd))
                application_stop(app);
            if (platform_is_key_down(KEY_ESCAPE)) {
                application_stop(app);
            }

            render_command_clear_buffers(CNGN_COLOR_BUFFER_BIT);
            render_command_clear_color(RGB_COLOR(69, 69, 69), 1.0f);

            start_moving_system();
            if (app->game_cbs.game_update_cb != nullptr)
                app->game_cbs.game_update_cb();
            end_moving_system();
            platform_input_update();
        }
        platform_window_update(app->wnd);
    }
}

void application_shutdown(application* app) {
    app->state.running = false;
    platform_window_destroy(app->wnd);
    destroy_listeners(event_listeners, max_events);
    platform_input_shutdown();
    batch_renderer_terminate();
    ui_shutdown();
    if (app->game_cbs.game_terminate_cb != nullptr)
        app->game_cbs.game_terminate_cb();
    free(g_state);
}

void application_stop(application* app) {
    app->state.running = false;
}