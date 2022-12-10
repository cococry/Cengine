#include "sprite_animation.h"

#include "../core/global_state.h"
#include "../core/logging.h"
#include "quad2d.h"

sprite_animation sprite_animation_create(texture2d* sprite_sheet, vector2* texture_coords, float tick_speed, u32 frame_count, vector2 frame_cell_size, vector2 frame_sprite_scale) {
    sprite_animation ret;
    ret.sprite_sheet = sprite_sheet;
    ret.texture_coords = texture_coords;
    ret.tick_speed = tick_speed;
    ret.frame_cell_size = frame_cell_size;
    ret.frame_sprite_scale = frame_sprite_scale;
    ret._anim_timer = 0.0f;
    ret._anim_index = 0;
    ret.frame_count = frame_count;
    return ret;
}
void sprite_animation_tick(quad* obj) {
    obj->sprite_anim._anim_timer += g_state->app->state.delta_time;
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
}