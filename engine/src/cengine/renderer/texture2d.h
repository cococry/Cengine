#pragma once

#include "render_defines.h"
#include "../core/defines.h"

typedef struct texture2d {
    render_id id;
    u32 width, height;
    u32 number_of_channels;
    const char* filepath;
    u32 data_format, internal_format;
} texture2d;

texture2d* texture2d_create(const char* filepath);

texture2d* texture2d_create_by_size(u32 width, u32 height);

void texture2d_bind(texture2d* texture);

void texture2d_bind_unit(texture2d* texture, u32 slot);

void texture2d_active_slot(texture2d* texture, u32 slot);

void texture2d_unbind(texture2d* texture);

void texture2d_delete(texture2d* texture);

void texture2d_set_data(texture2d* texture, void* data, u32 data_size);

bool8 texture2d_compare(texture2d* texture1, texture2d* texture2);