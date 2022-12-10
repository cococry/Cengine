#pragma once

#include "defines.h"
#include "../platform/platform.h"
#include "../renderer/vertex_array.h"
#include "../renderer/shader.h"
#include "../renderer/texture2d.h"

#include "../math/vector.h"

typedef struct application_state {
    bool8 running;
    float delta_time;
} application_state;

typedef struct application {
    application_state state;
    window* wnd;
    shader_program* shader;
} application;

application* application_create(window_properties props);

void application_run(application* app);

void application_shutdown(application* app);

void application_stop(application* app);