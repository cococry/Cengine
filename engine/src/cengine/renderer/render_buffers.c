#include "render_buffers.h"

#include "../platform/opengl/gl_functions.h"
#include "../core/logging.h"

#include <stdlib.h>

vertex_layout_attribute vertex_layout_attribute_create(vertex_layout_attrib_type type) {
    vertex_layout_attribute ret;
    ret.type = type;
    switch (type) {
        case vertex_layout_attrib_type_float:
            ret._element_count = 1;
            break;
        case vertex_layout_attrib_type_int:
            ret._element_count = 1;
            break;
        case vertex_layout_attrib_type_vector2f:
            ret._element_count = 2;
            break;
        case vertex_layout_attrib_type_vector3f:
            ret._element_count = 3;
            break;
        case vertex_layout_attrib_type_vector4f:
            ret._element_count = 4;
            break;
        default:
            break;
    }
    return ret;
}
vertex_buffer vertex_buffer_create(const void* data, u32 size, u32 draw_mode, u32 vertex_attrib_count) {
    vertex_buffer ret;
    ret.layout_attributes = malloc(sizeof(vertex_layout_attribute) * vertex_attrib_count);
    ret.layout_attribute_capacity = vertex_attrib_count;
    ret.layout_attribute_count = 0;
    glCreateBuffers(1, &ret.id);
    glBindBuffer(GL_ARRAY_BUFFER, ret.id);
    glBufferData(GL_ARRAY_BUFFER, size, data, draw_mode);

    return ret;
}
vertex_buffer vertex_buffer_create_empty(u32 size, u32 vertex_attrib_count) {
    vertex_buffer ret;
    ret.layout_attributes = malloc(sizeof(vertex_layout_attribute) * vertex_attrib_count);
    ret.layout_attribute_capacity = vertex_attrib_count;
    ret.layout_attribute_count = 0;
    glCreateBuffers(1, &ret.id);
    glBindBuffer(GL_ARRAY_BUFFER, ret.id);
    glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);

    return ret;
}

void vertex_buffer_bind(vertex_buffer* buffer) {
    glBindBuffer(GL_ARRAY_BUFFER, buffer->id);
}

void vertex_buffer_unbind(vertex_buffer* buffer) {
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void vertex_buffer_delete(vertex_buffer* buffer) {
    glDeleteBuffers(1, &buffer->id);

    free(buffer->layout_attributes);
}

void vertex_buffer_add_layout_attribute(vertex_buffer* buffer, vertex_layout_attribute attribute) {
    ASSERT_MSG(buffer->layout_attribute_count < buffer->layout_attribute_capacity, "Vertex layout attribute index out of bounds.");

    buffer->layout_attributes[buffer->layout_attribute_count++] = attribute;
}

void vertex_buffer_set_data(vertex_buffer* buffer, const void* data, u32 size, u32 offset) {
    glBindBuffer(GL_ARRAY_BUFFER, buffer->id);
    glBufferSubData(GL_ARRAY_BUFFER, offset, size, data);
}

index_buffer index_buffer_create(u32* data, u32 count, u32 draw_mode) {
    index_buffer ret;
    ret.index_count = count;
    glCreateBuffers(1, &ret.id);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ret.id);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(u32), data, draw_mode);
    return ret;
}

void index_buffer_bind(index_buffer* buffer) {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer->id);
}

void index_buffer_unbind(index_buffer* buffer) {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void index_buffer_delete(index_buffer* buffer) {
    glDeleteBuffers(1, &buffer->id);
}