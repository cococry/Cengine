#pragma once

#include "../core/defines.h"

typedef struct vector3 {
    float x, y, z;
} vector3;

#define VECTOR3_ELEMENT_COUNT 3

vector3 vector3_create(float x, float y, float z);

float* vector3_value_ptr(vector3 vec);

void vector3_addv(vector3* source, vector3 add);

vector3 vector3_additition(vector3 v1, vector3 v2);

void vector3_subv(vector3* source, vector3 sub);

vector3 vector3_subtraction(vector3 v1, vector3 v2);

void vector3_multv(vector3* source, vector3 mult);

vector3 vector3_multiply(vector3 v1, vector3 v2);

void vector3_divdv(vector3* source, vector3 div);

vector3 vector3_divide(vector3 v1, vector3 v2);

void vector3_add_scaler(vector3* source, float k);

vector3 vector3_scaler_addition(vector3 v, float k);

void vector3_sub_scaler(vector3* source, float k);

vector3 vector3_scaler_subtraction(vector3 v, float k);

void vector3_mult_scaler(vector3* source, float k);

vector3 vector3_scaler_multiplication(vector3 v, float k);

void vector3_div_scaler(vector3* source, float k);

vector3 vector3_scaler_division(vector3 v, float k);

float vector3_dot(vector3 v1, vector3 v2);

vector3 vector3_cross(vector3 v1, vector3 v2);

float vector3_magnitude(vector3 v);

float vector3_magnitude_squared(vector3 v);

bool8 vector3_perpendicular(vector3 v1, vector3 v2);

bool8 vector3_compare(vector3 v1, vector3 v2);

vector3 vector3_normalized(vector3 v);

void vector3_normalize(vector3* v);

float vector3_distance(vector3 v1, vector3 v2);

void vector3_log(vector3 v);

void vector3_log_new_line(vector3 v);