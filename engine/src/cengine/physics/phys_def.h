#pragma once

#include "../math/vector2.h"

typedef enum physics_object_type {
    physics_object_type_dynamic = 0,
    physics_object_type_static
} physics_object_type;

typedef struct AABB {
    vector2 position;
    vector2 half_size;
} AABB;
typedef struct physics_object {
    AABB aabb;
    vector2 velocity, acceleration;
    physics_object_type type;
} physics_object;

typedef struct ray_cast_hit {
    bool8 is_hit;
    float time;
    vector2 position;
} ray_cast_hit;

void physics_object_set_linear_velocity(physics_object* obj, vector2 velocity);

void physics_object_add_linear_impulse(physics_object* obj, vector2 impulse);

bool8 physics_point_intersects_aabb(vector2 point, AABB aabb);

bool8 physics_aabb_intersects_aabb(AABB a, AABB b);

AABB aabb_minkowski_difference(AABB a, AABB b);

void aabb_penetration_vector(vector2* r, AABB aabb);

void aab_min_max(vector2* min, vector2* max, AABB aabb);

ray_cast_hit ray_intersect_aabb(vector2 position, vector2 magnitude, AABB aabb);