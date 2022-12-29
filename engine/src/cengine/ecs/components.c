#include "components.h"

transform_component transform_component_create(vector2 position, vector2 scale, float rotation) {
    transform_component ret;
    ret.position = position;
    ret.scale = scale;
    ret.rotation = rotation;
    ret.movement = vector2_create(0.0f, 0.0f);
    ret.last_position = position;
    return ret;
}

sprite_component sprite_component_create(vector2 uv, vector2 cell_size, vector4 color) {
    sprite_component ret;
    ret.uv = uv;
    ret.cell_size = cell_size;
    ret.color = color;
    return ret;
}