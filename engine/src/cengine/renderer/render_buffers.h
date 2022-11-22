#pragma once

#include "../core/defines.h"
#include "render_defines.h"

typedef enum vertex_layout_attrib_type {
    vertex_layout_attrib_type_float = 0,
    vertex_layout_attrib_type_int,
    vertex_layout_attrib_type_vector2f,
    vertex_layout_attrib_type_vector3f,
    vertex_layout_attrib_type_vector4f
} vertex_layout_attrib_type;

typedef struct vertex_layout_attribute {
    vertex_layout_attrib_type type;
    u32 _element_count;
} vertex_layout_attribute;

vertex_layout_attribute vertex_layout_attribute_create(vertex_layout_attrib_type type);

typedef struct vertex_buffer {
    render_id id;
    vertex_layout_attribute* layout_attributes;
    u32 layout_attribute_count, layout_attribute_capacity;
} vertex_buffer;

typedef struct index_buffer {
    u32 index_count;
    render_id id;
} index_buffer;

vertex_buffer* vertex_buffer_create(float* data, u32 size, u32 draw_mode, u32 vertex_attrib_count);

void vertex_buffer_bind(vertex_buffer* buffer);

void vertex_buffer_unbind(vertex_buffer* buffer);

void vertex_buffer_delete(vertex_buffer* buffer);

void vertex_buffer_add_layout_attribute(vertex_buffer* buffer, vertex_layout_attribute attribute);

index_buffer* index_buffer_create(u32* data, u32 count, u32 draw_mode);

void index_buffer_bind(index_buffer* buffer);

void index_buffer_unbind(index_buffer* buffer);

void index_buffer_delete(index_buffer* buffer);