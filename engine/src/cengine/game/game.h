#pragma once

#include "../renderer/quad2d.h"

typedef struct game_state {
    quad* player;
    quad* background;
} game_state;

void game_init();
void game_terminate();