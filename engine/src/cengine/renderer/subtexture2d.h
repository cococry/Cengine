#pragma once

#include "../math/vector2.h"
#include "texture2d.h"

typedef struct subtexture2d {
    texture2d* texture;
    vector2 coords, cell_size, sprite_scale;
} subtexture2d;

subtexture2d subtexture2d_create(texture2d* texture, vector2 coords, vector2 cell_size, vector2 sprite_scale);

typedef struct subtexture_coords {
    vector2 min, max;
} subtexture_coords;
subtexture_coords subtexture2d_get_texcoords(subtexture2d texture);