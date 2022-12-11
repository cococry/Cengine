#pragma once

#include "../math/vector3.h"
#include "../math/matrix4.h"

#include "../renderer/quad2d.h"

typedef struct player_camera {
    matrix4 view_matrix;
    vector2 position;
    quad* player;
} player_camera;

player_camera player_camera_create(vector2 position, quad* player);

void player_camera_update(player_camera* camera);