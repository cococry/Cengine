#pragma once

#include "../core/defines.h"

#include "vertex_array.h"
#include "texture2d.h"
#include "../math/vector.h"

typedef struct triangle triangle;
typedef void (*triangle_update_callback)(triangle*);

typedef struct triangle {
    const char* tag;

    vector2 position, scale;
    float rotation;

    vector2* vertices;
    vector4 color;

    vertex_array va;
    texture2d* texture;

    triangle_update_callback update_callback;

    u32 render_level;

} triangle;

triangle* triangle_create(const char* tag, vector2 position, vector2 scale, float rotation, vector2 vert1,
                          vector2 vert2, vector2 vert3, vector4 color, u32 render_level);

void triangle_load_texture(triangle* obj, const char* texture_filepath);

void triangle_delete(triangle* obj);

void triangle_render(triangle obj);

void triangle_move_x(triangle* obj, float x);

void triangle_move_y(triangle* obj, float y);

void default_triangle_update_callback(triangle* this);