#pragma once

#include "../core/defines.h"
#include "../math/vector.h"
#include "../renderer/shader.h"

typedef struct ui_state {
    shader_program* font_shader;
} ui_state;

void ui_init();

void ui_shutdown();

ui_state ui_get_state();

bool8 ui_image_button(vector2 position, vector2 scale, vector2 uv, vector2 sprite_cell_size);