#pragma once

#include "../math/vector.h"

typedef struct transform_component {
    vector2 position, scale, last_position;
    float rotation;
    vector2 movement;
} transform_component;

typedef struct sprite_component {
    vector2 uv, cell_size;
    vector4 color;
} sprite_component;

transform_component transform_component_create(vector2 position, vector2 scale, float rotation);

sprite_component sprite_component_create(vector2 uv, vector2 cell_size, vector4 color);