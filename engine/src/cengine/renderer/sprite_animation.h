#pragma once

#include "texture2d.h"
#include "../math/vector2.h"

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

sprite_animation sprite_animation_create(texture2d* sprite_sheet, vector2* texture_coords, float tick_speed, u32 frame_count, vector2 frame_cell_size, vector2 frame_sprite_scale);

void sprite_animation_tick(quad* obj);