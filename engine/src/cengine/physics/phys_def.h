#pragma once

#include "../math/vector2.h"

typedef struct AABB {
    vector2 half_size, position;
} AABB;

AABB aabb_create(vector2 half_size, vector2 position);

vector2 aabb_get_min(AABB aabb);

vector2 aabb_get_max(AABB aabb);

bool8 aabb_collides_with_point(AABB aabb, vector2 point);

bool8 aabb_collides_with_aabb(AABB a, AABB b);