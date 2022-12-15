#pragma once

#include "texture2d.h"
#include "../math/vector2.h"

#define UNUSED_ANIM_FRAME_COORD vector2_create(-1.0f, -1.0f)

typedef struct quad quad;

typedef struct sprite_animation {
    texture2d* sprite_sheet;
    vector2* texture_coords;
    float tick_speed;
    vector2 frame_cell_size, frame_sprite_scale;
    u32 frame_count;
    float _anim_timer;
    u32 _anim_index;
} sprite_animation;

sprite_animation sprite_animation_create(texture2d* sprite_sheet, u32 frame_count, float tick_speed, vector2 frame_cell_size, vector2 frame_sprite_scale);

void sprite_animation_set_anim_by_values(sprite_animation* anim, vector2* anim_coords, u32 anim_count);

void sprite_animation_set_anim_contiguous_x(sprite_animation* anim, u32 first_x, u32 last_x, u32 y);
void sprite_animation_set_anim_contiguous_y(sprite_animation* anim, u32 first_y, u32 last_y, u32 x);

void sprite_aimation_set_tick_speed(sprite_animation* anim, float tick_speed);

void sprite_animation_tick(quad* obj);
