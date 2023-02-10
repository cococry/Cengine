#include "../core/defines.h"
#include "../renderer/render_defines.h"
#include "../math/vector4.h"
#include "../math/vector2.h"
#include "../../../vendor/stb_truetype/stb_truetype.h"

typedef struct font_vertex {
    vector2 position, texcoord;
} font_vertex;

typedef struct font_struct {
    stbtt_bakedchar* data;
    render_id font_texture_id, vao, vbo;
    font_vertex* vertex_buffer_data;
    u32 vertex_buffer_capacity;
} font_struct;

font_struct font_renderer_load_file(const char* filepath);

void font_renderer_delete_font(font_struct* font);

void render_text(const char* text, vector2 position, font_struct font);