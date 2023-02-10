#include "font_renderer.h"
#include "../core/global_state.h"

#include "../platform/opengl/gl_functions.h"
#include "../math/vector2.h"
#include "../core/logging.h"
#include "ui_core.h"

#include <string.h>
#include <stdlib.h>

font_struct font_renderer_load_file(const char* filepath) {
    font_struct ret;

    ret.data = malloc(sizeof(stbtt_bakedchar) * 96);

    u8* ttfBuffer = (u8*)malloc(1 << 20);
    u8 tmpBitmap[512 * 512];

    fread(ttfBuffer, 1, 1 << 20, fopen(filepath, "rb"));
    stbtt_BakeFontBitmap(ttfBuffer, 0, 32.0f, tmpBitmap, 512, 512, 32, 96, ret.data);
    glGenTextures(1, &ret.font_texture_id);
    glBindTexture(GL_TEXTURE_2D, ret.font_texture_id);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA, 512, 512, 0, GL_ALPHA, GL_UNSIGNED_BYTE, tmpBitmap);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);

    glCreateVertexArrays(1, &ret.vao);
    glBindVertexArray(ret.vao);

    glCreateBuffers(1, &ret.vbo);
    glBindBuffer(GL_ARRAY_BUFFER, ret.vbo);
    ret.vertex_buffer_capacity = 20;
    ret.vertex_buffer_data = malloc(sizeof(font_vertex) * ret.vertex_buffer_capacity * 6);
    glBufferData(GL_ARRAY_BUFFER, sizeof(font_vertex) * 6 * ret.vertex_buffer_capacity, nullptr, GL_DYNAMIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(font_vertex), nullptr);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(font_vertex), (const void*)offsetof(font_vertex, texcoord));
    glBindVertexArray(0);

    free(ttfBuffer);
    return ret;
}a

void font_renderer_delete_font(font_struct* font) {
    if (font->data)
        free(font->data);
}

void render_text(const char* text, vector2 position, font_struct font) {
    glBindVertexArray(font.vao);
    glBindBuffer(GL_ARRAY_BUFFER, font.vbo);
    shader_program_bind(ui_get_state().font_shader);
    shader_program_upload_mat4(ui_get_state().font_shader, "u_proj", orthographic_matrix(0.0f, g_state->app->wnd->props.width, 0.0f, g_state->app->wnd->props.height));

    u32 lenght = strlen(text);
    if (font.vertex_buffer_capacity < lenght) {
        font.vertex_buffer_capacity = lenght;
        glBufferData(GL_ARRAY_BUFFER, sizeof(font_vertex) * 6 * font.vertex_buffer_capacity, 0, GL_DYNAMIC_DRAW);
        free(font.vertex_buffer_data);
        font.vertex_buffer_data = malloc(sizeof(font_vertex) * 6 * font.vertex_buffer_capacity);
    }

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, font.font_texture_id);
    glUniform1i(glGetUniformLocation(ui_get_state().font_shader->id, "u_texture"), 0);

    font_vertex* vertex_data = font.vertex_buffer_data;
    u32 num_vertices = 0;
    while (*text) {
        if (*text >= 32 && *text < 128) {
            stbtt_aligned_quad q;
            stbtt_GetBakedQuad(font.data, 512, 512, *text - 32, &position.x, &position.y, &q, 1);

            vertex_data[0].position = vector2_create(q.x0, q.y0);
            vertex_data[0].texcoord = vector2_create(q.s0, q.t0);
            vertex_data[1].position = vector2_create(q.x1, q.y0);
            vertex_data[1].texcoord = vector2_create(q.s1, q.t0);
            vertex_data[2].position = vector2_create(q.x1, q.y1);
            vertex_data[2].texcoord = vector2_create(q.s1, q.t1);
            vertex_data[3].position = vector2_create(q.x0, q.y1);
            vertex_data[3].texcoord = vector2_create(q.s0, q.t1);
            vertex_data[4].position = vector2_create(q.x0, q.y0);
            vertex_data[4].texcoord = vector2_create(q.s0, q.t0);
            vertex_data[5].position = vector2_create(q.x1, q.y1);
            vertex_data[5].texcoord = vector2_create(q.s1, q.t1);

            vertex_data += 6;
            num_vertices += 6;
        }
        ++text;
    }

    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(font_vertex) * num_vertices, font.vertex_buffer_data);
    glDrawArrays(GL_TRIANGLES, 0, num_vertices);

    shader_program_unbind(ui_get_state().font_shader);
}