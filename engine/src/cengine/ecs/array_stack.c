#include "array_stack.h"
#include <stdlib.h>
#include <string.h>

#include "../core/logging.h"

array_stack* array_stack_create(u32 size) {
    array_stack* stack = malloc(sizeof(array_stack));
    stack->size = size;
    stack->data = malloc(size);
    stack->cap = 1;
    stack->count = 0;
    return stack;
}

void array_stack_destroy(array_stack* stack) {
    free(stack->data);
    free(stack);
}

void* array_stack_pop(array_stack* stack) {
    u8* ret = (u8*)stack->data;
    ret += (--stack->count) * stack->size;
    return ret;
}

void array_stack_push(array_stack* stack, void* data) {
    u8* ptr;
    if (stack->count == stack->cap) {
        void* data_realloc = realloc(stack->data, stack->cap * 2 * stack->size);
        if (data_realloc == nullptr) {
            ASSERT_MSG(false, "Failed to reallocate memory.");
        } else {
            stack->data = data_realloc;
            stack->cap *= 2;
        }
    }

    ptr = (u8*)stack->data;
    ptr += (stack->count++) * stack->size;
    memcpy(ptr, data, stack->size);
}