#include "phys_def.h"

AABB aabb_create(vector2 half_size, vector2 position) {
    AABB ret;
    ret.position = position;
    ret.half_size = half_size;
    return ret;
}

vector2 aabb_get_min(AABB aabb) {
    return vector2_subtraction(aabb.position, aabb.half_size);
}

vector2 aabb_get_max(AABB aabb) {
    return vector2_additition(aabb.position, aabb.half_size);
}

bool8 aabb_collides_with_point(AABB aabb, vector2 point) {
    bool8 x_collision = point.x >= aabb_get_min(aabb).x && point.x <= aabb_get_max(aabb).x;
    bool8 y_collision = point.y >= aabb_get_min(aabb).y && point.y <= aabb_get_max(aabb).y;
    return x_collision && y_collision;
}

bool8 aabb_collides_with_aabb(AABB a, AABB b) {
    bool8 x_collision = aabb_get_min(a).x <= aabb_get_max(b).x &&
                        aabb_get_max(a).x >= aabb_get_min(b).x;
    bool8 y_collision = aabb_get_min(a).y <= aabb_get_max(b).y &&
                        aabb_get_max(a).y >= aabb_get_min(b).y;
    return x_collision && y_collision;
}