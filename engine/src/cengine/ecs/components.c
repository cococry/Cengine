#include "components.h"

transform_component transform_component_create(vector2 position, vector2 scale, float rotation) {
    transform_component ret;
    ret.position = position;
    ret.scale = scale;
    ret.rotation = rotation;
    ret.movement = vector2_create(0.0f, 0.0f);
    ret.last_position = position;
    ret.moved = false;
    return ret;
}

sprite_component sprite_component_create_color(vector2 cell_size, vector4 color) {
    sprite_component ret;
    ret.uv = vector2_create(-1.0f, -1.0f);
    ret.cell_size = cell_size;
    ret.color = color;
    return ret;
}
sprite_component sprite_component_create_sprite(vector2 uv, vector2 cell_size, vector4 color) {
    sprite_component ret;
    ret.uv = uv;
    ret.cell_size = cell_size;
    ret.color = color;
    return ret;
}

camera_component camera_component_create(vector2 position, bool8 selected_for_rendering) {
    camera_component ret;
    ret.position = position;
    ret.view = matrix4_identity();
    ret.selected_for_rendering = selected_for_rendering;
    return ret;
}