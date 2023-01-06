#include "ecs.h"

#include <stdlib.h>
#include <stdarg.h>
#include <string.h>

#include "array_stack.h"
#include "../core/logging.h"

#define INITIAL_COMPONENT_CAP 32

typedef struct component_storage {
    u32 type_count;
    u32 cap;
    size_t size;
    size_t* data_size_array;
    size_t* data_offset_array;
    void* data;
} component_storage;

typedef struct entity_storage {
    u32* mask_array;
    u32* flag_array;
    u32 count, cap;
} entity_storage;

typedef struct ecs_state {
    component_storage comp_storage;
    entity_storage entty_storage;
    ecs_query_result query_result;
    array_stack* entity_pool;
} ecs_state;

static ecs_state s_state = {0};

void ecs_init(u32 component_count, ...) {
    u32 i;
    va_list ap;
    size_t sizes[32];
    size_t offsets[32];
    size_t size = 0;

    va_start(ap, component_count);
    for (i = 0; i < component_count; i++) {
        sizes[i] = va_arg(ap, size_t);
        offsets[i] = size;
        size += sizes[i];
    }
    va_end(ap);

    s_state.entity_pool = array_stack_create(sizeof(u32));

    s_state.comp_storage.type_count = component_count;
    s_state.comp_storage.cap = INITIAL_COMPONENT_CAP;
    s_state.comp_storage.data = malloc(INITIAL_COMPONENT_CAP * size);
    s_state.comp_storage.data_size_array = malloc(component_count * sizeof(size_t));
    s_state.comp_storage.data_offset_array = malloc(component_count * sizeof(size_t));
    s_state.comp_storage.size = size;
    memcpy(s_state.comp_storage.data_size_array, sizes, component_count * sizeof(size_t));
    memcpy(s_state.comp_storage.data_offset_array, offsets, component_count * sizeof(size_t));

    s_state.entty_storage.count = 0;
    s_state.entty_storage.cap = INITIAL_COMPONENT_CAP;
    s_state.entty_storage.mask_array = malloc(INITIAL_COMPONENT_CAP * sizeof(u32));
    s_state.entty_storage.flag_array = malloc(INITIAL_COMPONENT_CAP * sizeof(u32));
}

entity entity_create() {
    entity entty;
    u32 id;
    if (s_state.entity_pool->count > 0) {
        id = *(u32*)array_stack_pop(s_state.entity_pool);
    } else {
        id = s_state.entty_storage.count++;
        if (s_state.entty_storage.cap == id) {
            u32* new_flag_array = realloc(s_state.entty_storage.flag_array, s_state.entty_storage.cap * 2 * sizeof(u32));
            u32* new_mask_array = realloc(s_state.entty_storage.mask_array, s_state.entty_storage.cap * 2 * sizeof(u32));
            void* new_data = realloc(s_state.comp_storage.data, s_state.comp_storage.cap * 2 * s_state.comp_storage.size);
            u32* new_query_result_list = realloc(s_state.query_result.list, s_state.entty_storage.cap * 2 * sizeof(u32));
            ASSERT_MSG(new_flag_array != nullptr && new_mask_array != nullptr && new_data != nullptr && new_query_result_list != nullptr,
                       "Failed to reallocate memory.");
            s_state.entty_storage.flag_array = new_flag_array;
            s_state.entty_storage.mask_array = new_mask_array;
            s_state.query_result.list = new_query_result_list;
            s_state.entty_storage.cap *= 2;

            s_state.comp_storage.data = new_data;
            s_state.comp_storage.cap *= 2;
        }
    }
    if (s_state.query_result.list == nullptr) {
        u32* new_query_result_list = realloc(s_state.query_result.list, s_state.entty_storage.cap * 2 * sizeof(u32));
        s_state.query_result.list = new_query_result_list;
    }
    s_state.entty_storage.mask_array[id] = 0;
    s_state.entty_storage.flag_array[id] = ENTITY_FLAG_ALIVE;
    entty.id = id;
    return entty;
}

void* ecs_get_component(u32 entity_id, u32 component_id) {
    return (u8*)s_state.comp_storage.data + (entity_id * s_state.comp_storage.size + s_state.comp_storage.data_offset_array[component_id]);
}

void ecs_add_component(u32 entity_id, u32 component_id, void* data) {
    size_t size = s_state.comp_storage.data_size_array[component_id];
    void* ptr = ecs_get_component(entity_id, component_id);
    s_state.entty_storage.mask_array[entity_id] |= (1 << component_id);
    memcpy(ptr, data, size);
}

void ecs_remove_component(u32 entity_id, u32 component_id) {
    s_state.entty_storage.mask_array[entity_id] &= ~(1 << component_id);
}

u32 ecs_has_component(u32 entity_id, u32 component_id) {
    return (s_state.entty_storage.mask_array[entity_id] & (1 << component_id)) != 0;
}

void entity_destroy(u32 entity_id) {
    if (0 != (s_state.entty_storage.flag_array[entity_id] & ENTITY_FLAG_ALIVE)) {
        s_state.entty_storage.flag_array[entity_id] &= ~ENTITY_FLAG_ALIVE;
        s_state.entty_storage.mask_array[entity_id] = 0;
        array_stack_push(s_state.entity_pool, &entity_id);
    }
}

ecs_query_result* ecs_query(u32 component_count, ...) {
    va_list ap;
    u32 i, mask = 0;

    va_start(ap, component_count);
    for (i = 0; i < component_count; ++i) {
        mask |= (1 << va_arg(ap, u32));
    }
    va_end(ap);

    for (i = 0; i < s_state.entty_storage.count; ++i) {
        if (0 != (s_state.entty_storage.flag_array[i] & ENTITY_FLAG_ALIVE) && mask == (s_state.entty_storage.mask_array[i] & mask)) {
            s_state.query_result.list[s_state.query_result.count++] = i;
        }
    }
    return &s_state.query_result;
}

void esc_end_query() {
    s_state.query_result.count = 0;
}