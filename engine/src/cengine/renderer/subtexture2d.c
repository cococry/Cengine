#include "subtexture2d.h"

#include <stdlib.h>

subtexture2d subtexture2d_create(texture2d* texture, vector2 coords, vector2 cell_size, vector2 sprite_scale) {
    subtexture2d ret;
    ret.texture = texture;
    ret.coords = coords;
    ret.cell_size = cell_size;
    ret.sprite_scale = sprite_scale;
    return ret;
}
subtexture_coords subtexture2d_get_texcoords(subtexture2d texture) {
    vector2 min = vector2_create((float)((texture.coords.x * texture.cell_size.x) / texture.texture->width),
                                 (float)(texture.coords.y * texture.cell_size.y) / texture.texture->height);
    vector2 max = vector2_create((float)((texture.coords.x + texture.sprite_scale.x) * texture.cell_size.x) / texture.texture->width,
                                 (float)(((texture.coords.y + texture.sprite_scale.y) * texture.cell_size.y) / texture.texture->height));

    subtexture_coords coords;
    coords.min = min;
    coords.max = max;
    return coords;
}