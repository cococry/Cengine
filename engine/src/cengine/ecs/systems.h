#pragma once

#include "ecs.h"
#include "components.h"

void scene_render_system();

void start_moving_system();

void end_moving_system();

camera_component* get_render_camera();