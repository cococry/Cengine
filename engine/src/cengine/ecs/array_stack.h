#pragma once

#include "../core/defines.h"

typedef struct array_stack {
    u32 size, count, cap;
    void* data;
} array_stack;

array_stack* array_stack_create(u32 size);

void array_stack_destroy(array_stack* stack);

void* array_stack_pop(array_stack* stack);

void array_stack_push(array_stack* stack, void* data);