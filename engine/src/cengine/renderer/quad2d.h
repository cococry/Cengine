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
    vector2 scale;
    float rotation;

    vector4 color;

    vertex_array* va;
    vertex_buffer* vb;
    texture2d* texture;

    sprite_animation sprite_anim;

    quad_update_callback update_callback;
} quad;

quad* quad_create(const char* tag, vector2 position, vector2 scale, float rotation, vector4 color, subtexture2d* subtexture);

void quad_load_texture(quad* quad, const char* texture_filepath);

void quad_set_sprite_animation(quad* quad, sprite_animation anim, bool8 delete_last_tex_coords);

void quad_delete(quad* quad);

void quad_render(quad* quad);

void quad_move_x(quad* quad, float x);

void quad_move_y(quad* quad, float y);

void default_quad_update_callback(quad* this);

void quad_change_texture_coords(quad* quad, subtexture_coords coords);