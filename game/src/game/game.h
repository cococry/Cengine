#pragma once

#include <cengine/renderer/quad2d.h>
#include "player_camera.h"

typedef struct game_player {
    quad* quad_instc;
    player_camera camera;
} game_player;

typedef struct game_state {
    game_player player;
} game_state;

void game_init();
void game_update();
void game_terminate();