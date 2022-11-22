#include "application.h"
#include "event_system.h"
#include "global_state.h"

#include <stdlib.h>

#include "logging.h"

#include "../renderer/render_command.h"
#include "../renderer/render_defines.h"
#include "../platform/opengl/gl_functions.h"

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

    float vertices[] = {
        -0.5f, -0.5f, 0.0f,
        -0.5f, 0.5f, 0.0f,
        0.5f, 0.5f, 0.0f,
        0.5f, -0.5f, 0.0f};

    u32 indices[] = {0, 1, 2, 2, 3, 0};

    ret->va = vertex_array_create();

    vertex_buffer* vb = vertex_buffer_create(vertices, sizeof(vertices), CNGN_STATIC_DRAW, 1);

    vertex_buffer_add_layout_attribute(vb, vertex_layout_attribute_create(vertex_layout_attrib_type_vector3f));

    index_buffer* ib = index_buffer_create(indices, 6, CNGN_STATIC_DRAW);

    vertex_array_add_vertex_buffer(ret->va, vb);
    vertex_array_set_index_buffer(ret->va, ib);

    ret->shader = shader_program_create("../engine/assets/shaders/default_vertex.glsl", "../engine/assets/shaders/default_fragment.glsl");

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
        render_command_clear_buffers(CNGN_COLOR_BUFFER_BIT);
        render_command_clear_color(0.1f, 0.1f, 0.1f, 1.0f);

        shader_program_bind(app->shader);
        render_command_draw_indexed(app->va);

        platform_window_update(app->wnd);
        platform_input_update();
    }
}

void application_shutdown(application* app) {
    app->state.running = false;
    platform_window_destroy(app->wnd);
    destroy_listeners(event_listeners, max_events);
    vertex_array_delete(app->va);
    platform_input_shutdown();
    free(g_state);
}

void application_stop(application* app) {
    app->state.running = false;
}