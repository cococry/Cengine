#include "sprite_animation.h"
#include "../core/global_state.h"
#include "../ecs/components.h"

#include <stdlib.h>
#include <string.h>

#include "batch_renderer.h"

#include "../core/logging.h"

sprite_animation sprite_animaton_create(vector2* animation_frames, u32 frame_count, float tick_speed, vector2 frame_cell_size) {
    sprite_animation ret;
    ret._timer = 0.0f;
    ret._anim_index = 0;
    ret.animation_frame_uvs = animation_frames;
    ret.frame_cell_size = frame_cell_size;
    ret.tick_speed = tick_speed;
    ret.frame_count = frame_count;
    return ret;
}

sprite_animation sprite_animaton_create_empty(u32 frame_count, float tick_speed, vector2 frame_cell_size) {
    sprite_animation ret;
    ret._timer = 0.0f;
    ret._anim_index = 0;
    ret.animation_frame_uvs = malloc(sizeof(vector2) * frame_count);
    ret.frame_cell_size = frame_cell_size;
    ret.tick_speed = tick_speed;
    ret.frame_count = frame_count;
    return ret;
}

void sprite_animation_tick_on_entity(sprite_animation* anim, entity entty) {
    anim->_timer += g_state->app->state.delta_time;
    if (anim->_timer >= anim->tick_speed) {
        anim->_timer = 0.0f;
        if (!ecs_has_component(entty.id, 0) || !ecs_has_component(entty.id, 1)) {
            return;
        }
        sprite_component* sprite_comp = (sprite_component*)ecs_get_component(entty.id, 1);
        if (!vector2_compare(sprite_comp->cell_size, anim->frame_cell_size)) {
            sprite_comp->cell_size = anim->frame_cell_size;
        }
        sprite_comp->uv = anim->animation_frame_uvs[anim->_anim_index];
        if (anim->_anim_index >= anim->frame_count - 1) {
            anim->_anim_index = 0;
        } else {
            anim->_anim_index++;
        }
    }
}

void sprite_animation_set_anim_contigues_x(sprite_animation* anim, u32 start_x, u32 end_x, u32 y, u32 frame_count) {
    if (frame_count > anim->frame_count) {
        vector2* tmp = anim->animation_frame_uvs;
        free(anim->animation_frame_uvs);
        anim->animation_frame_uvs = nullptr;
        anim->animation_frame_uvs = malloc(sizeof(vector2) * frame_count);
        memcpy(anim->animation_frame_uvs, tmp, sizeof(vector2) * frame_count);
    }
    u32 i = 0;
    for (u32 x = start_x; x < end_x; x++) {
        anim->animation_frame_uvs[i] = vector2_create(x, y);
        i++;
    }
    anim->frame_count = frame_count;
    anim->_anim_index = 0;
    anim->_timer = anim->tick_speed;
}

void sprite_animation_delete(sprite_animation* anim) {
    if (anim->animation_frame_uvs != nullptr) {
        free(anim->animation_frame_uvs);
    }
}