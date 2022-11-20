#pragma once

#include "application.h"

typedef struct global_state {
    application* app;
} global_state;

extern global_state* g_state;