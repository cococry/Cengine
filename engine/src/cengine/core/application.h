#pragma once

#include "defines.h"
#include "../platform/platform.h"

typedef struct application_state {
    bool8 running;
} application_state;

typedef struct application {
    application_state state;
    window* wnd;
} application;

application* application_create(window_properties props);

void application_run(application* app);

void application_shutdown(application* app);

void application_stop(application* app);