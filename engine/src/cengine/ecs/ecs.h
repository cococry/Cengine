#pragma once

#include "../math/vector.h"

#define ENTITY_FLAG_ALIVE 1

typedef struct entity {
    u32 id;
} entity;

typedef struct ecs_query_result {
    u32 count, cap;
    u32* list;
} ecs_query_result;

void ecs_init(u32 component_count, ...);

entity entity_create();

void* ecs_get_component(u32 entity_id, u32 component_id);

void ecs_add_component(u32 entity_id, u32 component_id, void* data);

void ecs_remove_component(u32 entity_id, u32 component_id);

u32 ecs_has_component(u32 entity_id, u32 component_id);

void entity_destroy(u32 entity_id);

ecs_query_result* ecs_query(u32 component_count, ...);

void esc_end_query();