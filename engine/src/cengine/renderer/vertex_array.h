#pragma once

#include "render_buffers.h"
typedef struct vertex_array {
    render_id id;
    vertex_buffer* vertex_buffers;
    index_buffer index_buffer;
    u32 vertex_buffer_count;
    u32 _vertex_attribute_offset;
} vertex_array;

vertex_array vertex_array_create();

void vertex_array_bind(vertex_array* va);

void vertex_array_unbind(vertex_array* va);

void vertex_array_delete(vertex_array* va);

void vertex_array_add_vertex_buffer(vertex_array* va, vertex_buffer vb);

void vertex_array_set_index_buffer(vertex_array* va, index_buffer ib);
