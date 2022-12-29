#pragma once

#include "../math/vector.h"

#include "../ecs/ecs.h"

typedef struct sprite_animation {
    float _timer;
    u32 _anim_index;
    u32 frame_count;
    float tick_speed;
    vector2* animation_frame_uvs;
    vector2 frame_cell_size;
} sprite_animation;

sprite_animation sprite_animaton_create(vector2* animation_frames, u32 frame_count, float tick_speed, vector2 frame_cell_size);

sprite_animation sprite_animaton_create_empty(u32 frame_count, float tick_speed, vector2 frame_cell_size);

void sprite_animation_delete(sprite_animation* anim);

void sprite_animation_tick_on_entity(sprite_animation* anim, entity entty);

void sprite_animation_set_anim_contigues_x(sprite_animation* anim, u32 start_x, u32 end_x, u32 y, u32 frame_count);