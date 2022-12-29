#pragma once

#include "application.h"

typedef struct global_state {
    application* app;
    u32 entity_count;
} global_state;

extern global_state* g_state;