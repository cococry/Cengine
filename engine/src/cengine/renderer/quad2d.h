#pragma once

#include "../math/vector.h"
#include "vertex_array.h"
#include "texture2d.h"
#include "sprite_animation.h"

#include "subtexture2d.h"

typedef struct quad quad;
typedef void (*quad_update_callback)(quad*);

typedef struct quad {
    const char* tag;

    vector2 position;
    vector2 movement;
    vector2 scale;
    vector2 hit_box;
    float rotation;

    vector4 color;

    vertex_array va;
    vertex_buffer vb;
    texture2d* texture;

    u32 render_level;

    sprite_animation sprite_anim;

    quad_update_callback update_callback;
} quad;

#define QUAD_LEFT_MOVE_VECTOR vector2_create(-1.0f, 0.0f)
#define QUAD_RIGHT_MOVE_VECTOR vector2_create(1.0f, 0.0f)
#define QUAD_UP_MOVE_VECTOR vector2_create(0.0f, 1.0f)
#define QUAD_DOWN_MOVE_VECTOR vector2_create(0.0f, -1.0f)
#define QUAD_IDLE_VECTOR vector2_create(0.0f, 0.0f)

quad* quad_create(const char* tag, vector2 position, vector2 scale, vector2 hit_box, float rotation, u32 render_level, vector4 color, subtexture2d* subtexture);

void quad_load_texture(quad* quad, const char* texture_filepath);

void quad_set_sprite_animation(quad* quad, sprite_animation anim);

void quad_delete(quad* quad);

void quad_render(quad* quad);

void quad_move_x(quad* quad, float x);

void quad_move_y(quad* quad, float y);

void default_quad_update_callback(quad* this);

void quad_change_texture_coords(quad* quad, subtexture_coords coords);

bool8 quad_collding_with_quad(quad* quad1, quad* quad2);