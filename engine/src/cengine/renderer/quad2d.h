#pragma once

#include "../math/vector.h"
#include "vertex_array.h"
#include "texture.h"

typedef struct quad {
    const char* tag;

    vector2 position;
    vector2 scale;
    float rotation;

    vector4 color;

    vertex_array* va;
    texture2d* texture;

} quad;

quad quad_create(const char* tag, vector2 position, vector2 scale, float rotation, vector4 color, const char* texture_filepath);

void quad_delete(quad* quad);

void quad_render(quad quad);

void quad_move_x(quad* quad, float x);

void quad_move_y(quad* quad, float y);