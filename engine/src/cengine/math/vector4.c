#include "vector4.h"

#include <stdlib.h>
#include <math.h>
#include <stdio.h>

vector4 vector4_create(float x, float y, float z, float w) {
    vector4 ret;
    ret.x = x;
    ret.y = y;
    ret.z = z;
    ret.w = w;
    return ret;
}

float* vector4_value_ptr(vector4 vec) {
    float* ret = malloc(sizeof(float) * VECTOR4_ELEMENT_COUNT);
    ret[0] = vec.x;
    ret[1] = vec.y;
    ret[2] = vec.z;
    ret[3] = vec.w;
    return ret;
}

void vector4_addv(vector4* source, vector4 add) {
    source->x += add.x;
    source->y += add.y;
    source->z += add.z;
    source->w += add.w;
}

vector4 vector4_additition(vector4 v1, vector4 v2) {
    vector4 ret = vector4_create(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z, v1.w + v2.w);
    return ret;
}

void vector4_subv(vector4* source, vector4 sub) {
    source->x -= sub.x;
    source->y -= sub.y;
    source->z -= sub.z;
    source->w -= sub.w;
}
vector4 vector4_subtraction(vector4 v1, vector4 v2) {
    vector4 ret = vector4_create(v1.x - v2.x, v1.y - v2.y, v1.z - v2.z, v1.w - v2.w);
    return ret;
}

void vector4_multv(vector4* source, vector4 mult) {
    source->x *= mult.x;
    source->y *= mult.y;
    source->z *= mult.z;
    source->w *= mult.w;
}

vector4 vector4_multiply(vector4 v1, vector4 v2) {
    vector4 ret = vector4_create(v1.x * v2.x, v1.y * v2.y, v1.z * v2.z, v1.w * v2.w);
    return ret;
}

void vector4_divdv(vector4* source, vector4 div) {
    source->x /= div.x;
    source->y /= div.y;
    source->z /= div.z;
    source->w /= div.w;
}

vector4 vector4_divide(vector4 v1, vector4 v2) {
    vector4 ret = vector4_create(v1.x / v2.x, v1.y / v2.y, v1.z / v2.z, v1.w / v2.w);
    return ret;
}

void vector4_add_scaler(vector4* source, float k) {
    source->x += k;
    source->y += k;
    source->z += k;
    source->w += k;
}

vector4 vector4_scaler_addition(vector4 v, float k) {
    vector4 ret = vector4_create(v.x + k, v.y + k, v.z + k, v.w + k);
    return ret;
}

void vector4_sub_scaler(vector4* source, float k) {
    source->x -= k;
    source->y -= k;
    source->z -= k;
    source->w -= k;
}

vector4 vector4_scaler_subtraction(vector4 v, float k) {
    vector4 ret = vector4_create(v.x - k, v.y - k, v.z - k, v.w - k);
    return ret;
}

void vector4_mult_scaler(vector4* source, float k) {
    source->x *= k;
    source->y *= k;
    source->z *= k;
    source->w *= k;
}

vector4 vector4_scaler_multiplication(vector4 v, float k) {
    vector4 ret = vector4_create(v.x * k, v.y * k, v.z * k, v.w * k);
    return ret;
}

void vector4_div_scaler(vector4* source, float k) {
    source->x /= k;
    source->y /= k;
    source->z /= k;
    source->w /= k;
}

vector4 vector4_scaler_division(vector4 v, float k) {
    vector4 ret = vector4_create(v.x / k, v.y / k, v.z / k, v.w / k);
    return ret;
}

float vector4_dot(vector4 v1, vector4 v2) {
    float ret = 0;
    ret += (v1.x * v2.x) + (v1.y * v2.y) + (v1.z * v2.z) + (v1.w * v2.w);
    return ret;
}

float vector4_magnitude(vector4 v) {
    return (float)sqrt(vector4_magnitude_squared(v));
}

float vector4_magnitude_squared(vector4 v) {
    float ret = 0.0f;

    ret += v.x * v.x;
    ret += v.y * v.y;
    ret += v.z * v.z;
    ret += v.w * v.w;

    return ret;
}

bool8 vector4_perpendicular(vector4 v1, vector4 v2) {
    return vector4_dot(v1, v2) == 0;
}

bool8 vector4_compare(vector4 v1, vector4 v2) {
    return v1.x == v2.x && v1.y == v2.y && v1.z == v2.z && v1.w == v2.w;
}

vector4 vector4_normalized(vector4 v) {
    vector4 ret = vector4_create(v.x, v.y, v.z, v.w);
    ret.x /= vector4_magnitude(v);
    ret.y /= vector4_magnitude(v);
    ret.z /= vector4_magnitude(v);
    ret.w /= vector4_magnitude(v);
    return ret;
}

void vector4_normalize(vector4* v) {
    v->x /= vector4_magnitude(*v);
    v->y /= vector4_magnitude(*v);
    v->z /= vector4_magnitude(*v);
    v->w /= vector4_magnitude(*v);
}

float vector4_distance(vector4 v1, vector4 v2) {
    vector4 dist = vector4_subtraction(v1, v2);

    return vector4_magnitude(dist);
}

void vector4_log(vector4 v) {
    printf("x: %f, y: %f, z: %f, w: %f", v.x, v.y, v.z, v.w);
}

void vector4_log_new_line(vector4 v) {
    printf("x: %f, y: %f, z: %f, w: %f\n", v.x, v.y, v.z, v.w);
}