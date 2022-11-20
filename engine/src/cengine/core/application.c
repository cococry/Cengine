#include "application.h"
#include "event_system.h"
#include "global_state.h"

#include <stdlib.h>
#include <time.h>

#include "logging.h"

#include "../platform/opengl/loader/glad.h"
#include "../renderer/render_command.h"

static void close_callback(void* data) {
    application* app = (application*)data;
    app->state.running = false;
}
application* application_create(window_properties props) {
    application* ret = malloc(sizeof(application));
    ret->wnd = platform_window_create(props);

    ASSERT_MSG(ret != nullptr || sizeof(ret) != sizeof(application), "Failed to create application.");

    ret->state.running = true;
    init_listeners(event_listeners, max_events);

    register_event(window_close_event, close_callback, "closeCb");

    platform_input_init();

    ret->state.delta_time = 0.0f;

    g_state = malloc(sizeof(global_state));
    g_state->app = ret;

    return ret;
}

void application_run(application* app) {
    while (app->state.running) {
        if (platform_window_close_requested(app->wnd))
            application_stop(app);
        if (platform_is_key_down(KEY_ESCAPE)) {
            application_stop(app);
        }

        render_command_clear_buffers(GL_COLOR_BUFFER_BIT);
        render_command_clear_color(0.1f, 0.1f, 0.1f, 1.0f);

        platform_window_update(app->wnd);
        platform_input_update();
    }
}

void application_shutdown(application* app) {
    app->state.running = false;
    platform_window_destroy(app->wnd);
    destroy_listeners(event_listeners, max_events);
    platform_input_shutdown();
    free(g_state);
}

void application_stop(application* app) {
    app->state.running = false;
}