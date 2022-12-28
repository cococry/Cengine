#include "phys_def.h"

#include <math.h>
#include <stdlib.h>

void aab_min_max(vector2* min, vector2* max, AABB aabb) {
    vector2_addv(min, aabb.position);
    vector2_subv(min, aabb.half_size);
    vector2_addv(max, aabb.position);
    vector2_addv(max, aabb.half_size);
}

void physics_object_set_linear_velocity(physics_object* obj, vector2 velocity) {
    obj->velocity = velocity;
}

void physics_object_add_linear_impulse(physics_object* obj, vector2 impulse) {
    vector2_addv(&obj->velocity, impulse);
}

bool8 physics_point_intersects_aabb(vector2 point, AABB aabb) {
    vector2 min = vector2_create(0.0f, 0.0f);
    vector2 max = vector2_create(0.0f, 0.0f);
    aab_min_max(&min, &max, aabb);
    return point.x >= min.x &&
           point.x <= max.x &&
           point.y >= min.y &&
           point.y <= max.y;
}

bool8 physics_aabb_intersects_aabb(AABB a, AABB b) {
    vector2 min = vector2_create(0.0f, 0.0f);
    vector2 max = vector2_create(0.0f, 0.0f);
    aab_min_max(&min, &max, aabb_minkowski_difference(a, b));

    return (min.x <= 0 && max.x >= 0 && min.y <= 0 && max.y >= 0);
}

AABB aabb_minkowski_difference(AABB a, AABB b) {
    AABB result;
    vector2_subv(&result.position, a.position);
    vector2_addv(&result.position, b.position);
    vector2_addv(&result.half_size, a.half_size);
    vector2_addv(&result.half_size, b.half_size);
    return result;
}

void aabb_penetration_vector(vector2* r, AABB aabb) {
    vector2 min = vector2_create(0.0f, 0.0f);
    vector2 max = vector2_create(0.0f, 0.0f);
    aab_min_max(&min, &max, aabb);

    float min_dist = fabsf(min.x);
    r->x = min.x;
    r->y = 0;

    if (fabsf(max.x) < min_dist) {
        min_dist = fabsf(max.x);
        r->x = max.x;
    }
    if (fabsf(min.y) < min_dist) {
        min_dist = fabsf(min.y);
        r->x = 0;
        r->y = min.y;
    }
    if (fabsf(max.y) < min_dist) {
        r->x = 0;
        r->y = max.y;
    }
}

ray_cast_hit ray_intersect_aabb(vector2 position, vector2 magnitude, AABB aabb) {
    ray_cast_hit hit;
    hit.is_hit = false;
    hit.position = vector2_create(0.0f, 0.0f);
    hit.time = 0.0f;
    vector2 min = vector2_create(0.0f, 0.0f);
    vector2 max = vector2_create(0.0f, 0.0f);
    aab_min_max(&min, &max, aabb);

    float last_entry = -INFINITY;
    float first_exit = INFINITY;

    float* magnitude_vals = vector2_value_ptr(magnitude);
    float* position_vals = vector2_value_ptr(position);
    float* min_vals = vector2_value_ptr(min);
    float* max_vals = vector2_value_ptr(max);
    for (u8 i = 0; i < 2; i++) {
        if (magnitude_vals[i] != 0) {
            float t1 = (min_vals[i] - position_vals[i]) / magnitude_vals[i];
            float t2 = (max_vals[i] - position_vals[i]) / magnitude_vals[i];

            last_entry = fmaxf(last_entry, fminf(t1, t2));
            first_exit = fminf(first_exit, fmaxf(t1, t2));
        } else if (position_vals[i] <= min_vals[i] || position_vals[i] >= max_vals[i]) {
            return hit;
        }
    }
    free(magnitude_vals);
    free(position_vals);
    free(min_vals);
    free(max_vals);

    if (first_exit > last_entry && first_exit > 0 && last_entry < 1) {
        hit.position.x = position.x + magnitude.x * last_entry;
        hit.position.y = position.y + magnitude.y * last_entry;

        hit.is_hit = true;
        hit.time = last_entry;
    }
    return hit;
}