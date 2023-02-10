#include "ui_core.h"
#include "../platform/platform.h"
#include "../renderer/batch_renderer.h"
#include "../core/global_state.h"

static ui_state s_state;

void ui_init() {
    s_state.font_shader = shader_program_create("../engine/assets/shaders/font_vertex.glsl",
                                                "../engine/assets/shaders/font_fragment.glsl");
    shader_program_bind(s_state.font_shader);
    shader_program_upload_mat4(s_state.font_shader, "u_proj", orthographic_matrix(0.0f, g_state->app->wnd->props.width, 0.0f, g_state->app->wnd->props.height));
}

void ui_shutdown() {
    shader_program_delete(s_state.font_shader);
}
ui_state ui_get_state() {
    return s_state;
}

bool8 ui_image_button(vector2 position, vector2 scale, vector2 uv, vector2 sprite_cell_size) {
    batch_renderer_render_sprite(position, scale, 0.0f, uv, sprite_cell_size, vector4_create(1.0f, 1.0f, 1.0f, 1.0f), true);

    AABB button_aabb = aabb_create(vector2_create(scale.x / 2.0f, scale.y / 2.0f), position);
    return aabb_collides_with_point(button_aabb, vector2_create(platform_get_mouse_x(), g_state->app->wnd->props.height - platform_get_mouse_y())) && platform_mouse_button_went_down(BUTTON_LEFT);
}