#pragma once

#include "../math/vector.h"
#include "../math/matrix4.h"

enum {
    component_type_transform = 0,
    component_type_sprite = 1,
    component_type_camera = 2
};

typedef struct transform_component {
    vector2 position, scale, last_position;
    bool8 moved;
    float rotation;
    vector2 movement;
} transform_component;

typedef struct sprite_component {
    vector2 uv, cell_size;
    vector4 color;
} sprite_component;

typedef struct camera_component {
    matrix4 view;
    vector2 position;
    bool8 selected_for_rendering;
} camera_component;

transform_component transform_component_create(vector2 position, vector2 scale, float rotation);

sprite_component sprite_component_create_color(vector2 cell_size, vector4 color);

sprite_component sprite_component_create_sprite(vector2 uv, vector2 cell_size, vector4 color);

camera_component camera_component_create(vector2 position, bool8 selected_for_rendering);