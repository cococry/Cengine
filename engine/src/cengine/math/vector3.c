#include "vector3.h"

#include <stdlib.h>
#include <math.h>
#include <stdio.h>

vector3 vector3_create(float x, float y, float z) {
    vector3 ret;
    ret.x = x;
    ret.y = y;
    ret.z = z;
    return ret;
}

float* vector3_value_ptr(vector3 vec) {
    float* ret = malloc(sizeof(float) * VECTOR3_ELEMENT_COUNT);
    ret[0] = vec.x;
    ret[1] = vec.y;
    ret[2] = vec.z;
    return ret;
}

void vector3_addv(vector3* source, vector3 add) {
    source->x += add.x;
    source->y += add.y;
    source->z += add.z;
}

vector3 vector3_additition(vector3 v1, vector3 v2) {
    vector3 ret = vector3_create(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z);
    return ret;
}

void vector3_subv(vector3* source, vector3 sub) {
    source->x -= sub.x;
    source->y -= sub.y;
    source->z -= sub.z;
}
vector3 vector3_subtraction(vector3 v1, vector3 v2) {
    vector3 ret = vector3_create(v1.x - v2.x, v1.y - v2.y, v1.z - v2.z);
    return ret;
}

void vector3_multv(vector3* source, vector3 mult) {
    source->x *= mult.x;
    source->y *= mult.y;
    source->z *= mult.z;
}

vector3 vector3_multiply(vector3 v1, vector3 v2) {
    vector3 ret = vector3_create(v1.x * v2.x, v1.y * v2.y, v1.z * v2.z);
    return ret;
}

void vector3_divdv(vector3* source, vector3 div) {
    source->x /= div.x;
    source->y /= div.y;
    source->z /= div.z;
}

vector3 vector3_divide(vector3 v1, vector3 v2) {
    vector3 ret = vector3_create(v1.x / v2.x, v1.y / v2.y, v1.z / v2.z);
    return ret;
}

void vector3_add_scaler(vector3* source, float k) {
    source->x += k;
    source->y += k;
    source->z += k;
}

vector3 vector3_scaler_addition(vector3 v, float k) {
    vector3 ret = vector3_create(v.x + k, v.y + k, v.z + k);
    return ret;
}

void vector3_sub_scaler(vector3* source, float k) {
    source->x -= k;
    source->y -= k;
    source->z -= k;
}

vector3 vector3_scaler_subtraction(vector3 v, float k) {
    vector3 ret = vector3_create(v.x - k, v.y - k, v.z - k);
    return ret;
}

void vector3_mult_scaler(vector3* source, float k) {
    source->x *= k;
    source->y *= k;
    source->z *= k;
}

vector3 vector3_scaler_multiplication(vector3 v, float k) {
    vector3 ret = vector3_create(v.x * k, v.y * k, v.z * k);
    return ret;
}

void vector3_div_scaler(vector3* source, float k) {
    source->x /= k;
    source->y /= k;
    source->z /= k;
}

vector3 vector3_scaler_division(vector3 v, float k) {
    vector3 ret = vector3_create(v.x / k, v.y / k, v.z / k);
    return ret;
}

float vector3_dot(vector3 v1, vector3 v2) {
    float ret = 0;
    ret += (v1.x * v2.x) + (v1.y * v2.y) + (v1.z * v2.z);
    return ret;
}

vector3 vector3_cross(vector3 v1, vector3 v2) {
    vector3 ret = vector3_create(0.0f, 0.0f, 0.0f);

    ret.x = (v1.y * v2.z) - (v1.z * v2.y);
    ret.y = -1 * ((v1.x * v2.z) - (v1.z * v2.x));
    ret.z = (v1.x * v2.y) - (v1.y * v2.x);

    return ret;
}

float vector3_magnitude(vector3 v) {
    return (float)sqrt(vector3_magnitude_squared(v));
}

float vector3_magnitude_squared(vector3 v) {
    float ret = 0.0f;

    ret += v.x * v.x;
    ret += v.y * v.y;
    ret += v.z * v.z;

    return ret;
}

bool8 vector3_perpendicular(vector3 v1, vector3 v2) {
    return vector3_dot(v1, v2) == 0;
}

bool8 vector3_compare(vector3 v1, vector3 v2) {
    return v1.x == v2.x && v1.y == v1.y && v1.z == v2.z;
}
vector3 vector3_normalized(vector3 v) {
    vector3 ret = vector3_create(v.x, v.y, v.z);
    ret.x /= vector3_magnitude(v);
    ret.y /= vector3_magnitude(v);
    ret.z /= vector3_magnitude(v);
    return ret;
}

void vector3_normalize(vector3* v) {
    v->x /= vector3_magnitude(*v);
    v->y /= vector3_magnitude(*v);
    v->z /= vector3_magnitude(*v);
}

float vector3_distance(vector3 v1, vector3 v2) {
    vector3 dist = vector3_subtraction(v1, v2);

    return vector3_magnitude(dist);
}

void vector3_log(vector3 v) {
    printf("x: %f, y: %f, z: %f", v.x, v.y, v.z);
}

void vector3_log_new_line(vector3 v) {
    printf("x: %f, y: %f, z: %f\n2", v.x, v.y, v.z);
}