#include "sprite_animation.h"

#include "../core/global_state.h"
#include "../core/logging.h"
#include "quad2d.h"

#include <stdlib.h>

sprite_animation sprite_animation_create(texture2d* sprite_sheet, u32 frame_count, float tick_speed, vector2 frame_cell_size, vector2 frame_sprite_scale) {
    sprite_animation ret;
    ret.sprite_sheet = sprite_sheet;
    if (frame_count != 0) {
        ret.texture_coords = malloc(sizeof(vector2) * frame_count + 1);
        for (u32 i = 0; i < frame_count + 1; i++) {
            ret.texture_coords[i] = UNUSED_ANIM_FRAME_COORD;
        }
    } else {
        ret.texture_coords = nullptr;
    }
    ret.tick_speed = tick_speed;
    ret.frame_cell_size = frame_cell_size;
    ret.frame_sprite_scale = frame_sprite_scale;
    ret._anim_timer = 0.0f;
    ret._anim_index = 0;
    ret.frame_count = frame_count;
    return ret;
}

void sprite_animation_set_anim_by_values(sprite_animation* anim, vector2* anim_coords, u32 anim_count) {
    if (anim_count > anim->frame_count) {
        free(anim->texture_coords);
        for (u32 i = 0; i < anim_count + 1; i++) {
            anim->texture_coords[i] = UNUSED_ANIM_FRAME_COORD;
        }
    }
    u32 i = 0;
    for (; i < anim_count + 1; i++) {
        anim->texture_coords[i] = anim_coords[i];
    }
    for (; i < anim->frame_count + 1; i++) {
        anim->texture_coords[i] = UNUSED_ANIM_FRAME_COORD;
    }
    anim->frame_count = anim_count;
}

void sprite_animation_set_anim_contiguous_x(sprite_animation* anim, u32 first_x, u32 last_x, u32 y) {
    u32 anim_count = (last_x - first_x) + 1;
    u32 x = first_x;
    for (u32 i = 0; i < anim_count; i++) {
        anim->texture_coords[i] = vector2_create(x, y);
        x++;
    }

    for (u32 i = anim_count - 1; i < anim->frame_count; i++) {
        anim->texture_coords[i] = UNUSED_ANIM_FRAME_COORD;
    }
    anim->frame_count = anim_count - 1;
}

void sprite_animation_set_anim_contiguous_y(sprite_animation* anim, u32 first_y, u32 last_y, u32 x) {
    u32 anim_count = (last_y - first_y) + 1;
    u32 y = first_y;
    for (u32 i = 0; i < anim_count; i++) {
        anim->texture_coords[i] = vector2_create(x, y);
        y++;
    }

    for (u32 i = anim_count - 1; i < anim->frame_count; i++) {
        anim->texture_coords[i] = UNUSED_ANIM_FRAME_COORD;
    }
    anim->frame_count = anim_count - 1;
}
void sprite_animation_tick(quad* obj) {
    obj->sprite_anim._anim_timer += g_state->app->state.delta_time;
    if (!vector2_compare(obj->sprite_anim.texture_coords[obj->sprite_anim._anim_index], UNUSED_ANIM_FRAME_COORD)) {
        if (obj->sprite_anim._anim_timer >= obj->sprite_anim.tick_speed) {
            subtexture2d texture_frame = subtexture2d_create(obj->sprite_anim.sprite_sheet,
                                                             obj->sprite_anim.texture_coords[obj->sprite_anim._anim_index],
                                                             obj->sprite_anim.frame_cell_size, obj->sprite_anim.frame_sprite_scale);
            quad_change_texture_coords(obj, subtexture2d_get_texcoords(texture_frame));
            if (obj->sprite_anim._anim_index < obj->sprite_anim.frame_count) {
                obj->sprite_anim._anim_index++;
            } else {
                obj->sprite_anim._anim_index = 0;
            }
            obj->sprite_anim._anim_timer = 0.0f;
        }
    } else {
        if (obj->sprite_anim._anim_index < obj->sprite_anim.frame_count) {
            obj->sprite_anim._anim_index++;
        } else {
            obj->sprite_anim._anim_index = 0;
        }
        obj->sprite_anim._anim_timer = 0.0f;
    }
}
