#include "vertex_array.h"

#include "../platform/opengl/gl_functions.h"

#include "../core/logging.h"

#include <stdlib.h>

#define MAX_VERTEX_BUFFERS 32
vertex_array vertex_array_create() {
    vertex_array ret;
    ret.vertex_buffers = malloc(sizeof(vertex_buffer) * MAX_VERTEX_BUFFERS);
    ret.vertex_buffer_count = 0;
    ret._vertex_attribute_offset = 0;
    glCreateVertexArrays(1, &ret.id);
    glBindBuffer(GL_ARRAY_BUFFER, ret.id);
    return ret;
}

void vertex_array_bind(vertex_array* va) {
    glBindVertexArray(va->id);
}

void vertex_array_unbind(vertex_array* va) {
    glBindVertexArray(0);
}

void vertex_array_delete(vertex_array* va) {
    glDeleteVertexArrays(1, &va->id);

    for (u32 i = 0; i < va->vertex_buffer_count; i++) {
        vertex_buffer_delete(&va->vertex_buffers[i]);
    }
    index_buffer_delete(&va->index_buffer);
}

void vertex_array_add_vertex_buffer(vertex_array* va, vertex_buffer vb) {
    glBindVertexArray(va->id);
    vertex_buffer_bind(&vb);

    u32 stride = 0;
    for (u32 i = 0; i < vb.layout_attribute_count; i++) {
        stride += vb.layout_attributes[i]._element_count * sizeof(float);
    }
    for (u32 i = 0; i < vb.layout_attribute_count; i++) {
        glVertexAttribPointer(i, vb.layout_attributes[i]._element_count,
                              GL_FLOAT, GL_FALSE, stride, (void*)(intptr_t)va->_vertex_attribute_offset);
        glEnableVertexAttribArray(i);
        va->_vertex_attribute_offset += vb.layout_attributes[i]._element_count * sizeof(float);
    }

    va->vertex_buffers[va->vertex_buffer_count++] = vb;
}

void vertex_array_set_index_buffer(vertex_array* va, index_buffer ib) {
    glBindVertexArray(va->id);
    index_buffer_bind(&ib);

    va->index_buffer = ib;
}
