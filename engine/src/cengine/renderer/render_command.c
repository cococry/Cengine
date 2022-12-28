#include "render_command.h"

#include <Windows.h>
#include <windowsx.h>
#include <gl/GL.h>
#include <gl/GLU.h>

void render_command_clear_buffers(u32 buffer_flags) {
    glClear(buffer_flags);
}

void render_command_clear_color(float r, float g, float b, float a) {
    glClearColor(r, g, b, a);
}

void render_command_draw_indexed(vertex_array* va) {
    vertex_array_bind(va);
    glDrawElements(GL_TRIANGLES, va->index_buffer.index_count, GL_UNSIGNED_INT, nullptr);
}

void render_command_draw_indexed_va_check(vertex_array* va, u32 index_count) {
    u32 count = index_count != 0 ? index_count : va->index_buffer.index_count;
    vertex_array_bind(va);
    glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr);
}

void render_command_resize_viewport(u32 width, u32 height) {
    glViewport(0, 0, width, height);
}