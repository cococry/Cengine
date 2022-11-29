#include "application.h"
#include "event_system.h"
#include "global_state.h"

#include <stdlib.h>

#include "logging.h"

#include "../renderer/render_command.h"
#include "../renderer/render_defines.h"
#include "../platform/opengl/gl_functions.h"

#include "../math/matrix.h"
#include "../math/radians.h"

#include "../../../vendor/stb_image/stb_image.h"

static vector3 cam_pos;

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
        -0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
        -0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
        0.5f, 0.5f, 0.0f, 1.0f, 1.0f,
        0.5f, -0.5f, 0.0f, 1.0f, 0.0f};

    u32 indices[] = {0, 1, 2, 2, 3, 0};

    ret->va = vertex_array_create();

    vertex_buffer* vb = vertex_buffer_create(vertices, sizeof(vertices), CNGN_STATIC_DRAW, 2);

    vertex_buffer_add_layout_attribute(vb, vertex_layout_attribute_create(vertex_layout_attrib_type_vector3f));
    vertex_buffer_add_layout_attribute(vb, vertex_layout_attribute_create(vertex_layout_attrib_type_vector2f));

    index_buffer* ib = index_buffer_create(indices, 6, CNGN_STATIC_DRAW);

    vertex_array_add_vertex_buffer(ret->va, vb);
    vertex_array_set_index_buffer(ret->va, ib);

    ret->shader = shader_program_create("../engine/assets/shaders/default_vertex.glsl", "../engine/assets/shaders/default_fragment.glsl");
    shader_program_bind(ret->shader);
    shader_program_upload_vec4(ret->shader, "uColor", vector4_create(0.5f, 0.7f, 0.3f, 1.0f));
    shader_program_upload_int(ret->shader, "uTexture", 0);

    cam_pos = vector3_create(300.0f, 300.0f, 0.0f);

    shader_program_upload_mat4(ret->shader, "u_proj",
                               orthographic_matrix(0.0f, 1280.0f, 0.0f, 720.0f));

    ret->texture = texture2d_create("../engine/assets/textures/grass.jpg");

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
        texture2d_bind(app->texture);
        texture2d_bind_unit(app->texture, 0);

        matrix4 model_matrix =
            matrix4_multiply(
                translate_mv(matrix4_identity(), vector3_create(1280.0f / cam_pos.x, 720.0f / cam_pos.y, 0.0f)),
                scale_mv(matrix4_identity(), vector3_create(100.0f, 100.0f, 0.0f)));

        shader_program_upload_mat4(app->shader, "u_model", model_matrix);

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