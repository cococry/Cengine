#include "render_command.h"

#include "../platform/opengl/loader/glad.h"

void render_command_clear_buffers(u32 buffer_flags) {
    glClear(buffer_flags);
}

void render_command_clear_color(float r, float g, float b, float a) {
    glClearColor(r, g, b, a);
}