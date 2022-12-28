#include "vector2.h"

#include <stdlib.h>
#include <math.h>
#include <stdio.h>

vector2 vector2_create(float x, float y) {
    vector2 ret;
    ret.x = x;
    ret.y = y;
    return ret;
}

float* vector2_value_ptr(vector2 vec) {
    float* ret = malloc(sizeof(float) * VECTOR2_ELEMENT_COUNT);
    ret[0] = vec.x;
    ret[1] = vec.y;
    return ret;
}

void vector2_addv(vector2* source, vector2 add) {
    source->x += add.x;
    source->y += add.y;
}

vector2 vector2_additition(vector2 v1, vector2 v2) {
    vector2 ret = vector2_create(v1.x + v2.x, v1.y + v2.y);
    return ret;
}

void vector2_subv(vector2* source, vector2 sub) {
    source->x -= sub.x;
    source->y -= sub.y;
}
vector2 vector2_subtraction(vector2 v1, vector2 v2) {
    vector2 ret = vector2_create(v1.x - v2.x, v1.y - v2.y);
    return ret;
}

void vector2_multv(vector2* source, vector2 mult) {
    source->x *= mult.x;
    source->y *= mult.y;
}

vector2 vector2_multiply(vector2 v1, vector2 v2) {
    vector2 ret = vector2_create(v1.x * v2.x, v1.y * v2.y);
    return ret;
}

void vector2_divdv(vector2* source, vector2 div) {
    source->x /= div.x;
    source->y /= div.y;
}

vector2 vector2_divide(vector2 v1, vector2 v2) {
    vector2 ret = vector2_create(v1.x / v2.x, v1.y / v2.y);
    return ret;
}

void vector2_add_scaler(vector2* source, float k) {
    source->x += k;
    source->y += k;
}

vector2 vector2_scaler_addition(vector2 v, float k) {
    vector2 ret = vector2_create(v.x + k, v.y + k);
    return ret;
}

void vector2_sub_scaler(vector2* source, float k) {
    source->x -= k;
    source->y -= k;
}

vector2 vector2_scaler_subtraction(vector2 v, float k) {
    vector2 ret = vector2_create(v.x - k, v.y - k);
    return ret;
}

void vector2_mult_scaler(vector2* source, float k) {
    source->x *= k;
    source->y *= k;
}

vector2 vector2_scaler_multiplication(vector2 v, float k) {
    vector2 ret = vector2_create(v.x * k, v.y * k);
    return ret;
}

void vector2_div_scaler(vector2* source, float k) {
    source->x /= k;
    source->y /= k;
}

vector2 vector2_scaler_division(vector2 v, float k) {
    vector2 ret = vector2_create(v.x / k, v.y / k);
    return ret;
}

float vector2_dot(vector2 v1, vector2 v2) {
    float ret = 0;
    ret += (v1.x * v2.x) + (v1.y * v2.y);
    return ret;
}

float vector2_magnitude(vector2 v) {
    return (float)sqrt(vector2_magnitude_squared(v));
}

float vector2_magnitude_squared(vector2 v) {
    float ret = 0.0f;

    ret += v.x * v.x;
    ret += v.y * v.y;

    return ret;
}

bool8 vector2_perpendicular(vector2 v1, vector2 v2) {
    return vector2_dot(v1, v2) == 0;
}

bool8 vector2_compare(vector2 v1, vector2 v2) {
    return v1.x == v2.x && v1.y == v2.y;
}

vector2 vector2_normalized(vector2 v) {
    vector2 ret = vector2_create(v.x, v.y);
    ret.x /= vector2_magnitude(v);
    ret.y /= vector2_magnitude(v);
    return ret;
}

void vector2_normalize(vector2* v) {
    v->x /= vector2_magnitude(*v);
    v->y /= vector2_magnitude(*v);
}

float vector2_distance(vector2 v1, vector2 v2) {
    vector2 dist = vector2_subtraction(v1, v2);

    return vector2_magnitude(dist);
}

void vector2_log(vector2 v) {
    printf("x: %f, y: %f", v.x, v.y);
}

void vector2_log_new_line(vector2 v) {
    printf("x: %f, y: %f\n", v.x, v.y);
}

void vector2_set_value_at_index(vector2* v, u32 i, u32 val) {
    if (i == 0) {
        v->x = val;
        return;
    }
    v->y = val;
}