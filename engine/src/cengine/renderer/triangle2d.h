#pragma once

#include "../core/defines.h"

#include "vertex_array.h"
#include "texture.h"
#include "../math/vector.h"

typedef struct triangle {
    const char* tag;

    vector2 position, scale;
    float rotation;

    vector2* vertices;
    vector4 color;

    vertex_array* va;

    texture2d* texture;
} triangle;

triangle* triangle_create(const char* tag, vector2 position, vector2 scale, float rotation, vector2 vert1,
                          vector2 vert2, vector2 vert3, vector4 color);

void triangle_load_texture(triangle* obj, const char* texture_filepath);

void triangle_delete(triangle* obj);

void triangle_render(triangle obj);

void triangle_move_x(triangle* obj, float x);

void triangle_move_y(triangle* obj, float y);