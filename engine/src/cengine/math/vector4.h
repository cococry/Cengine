#pragma once

#include "../core/defines.h"

typedef struct vector4 {
    float x, y, z, w;
} vector4;

#define VECTOR4_ELEMENT_COUNT 4

vector4 vector4_create(float x, float y, float z, float w);

float* vector4_value_ptr(vector4 vec);

void vector4_addv(vector4* source, vector4 add);

vector4 vector4_additition(vector4 v1, vector4 v2);

void vector4_subv(vector4* source, vector4 sub);

vector4 vector4_subtraction(vector4 v1, vector4 v2);

void vector4_multv(vector4* source, vector4 mult);

vector4 vector4_multiply(vector4 v1, vector4 v2);

void vector4_divdv(vector4* source, vector4 div);

vector4 vector4_divide(vector4 v1, vector4 v2);

void vector4_add_scaler(vector4* source, float k);

vector4 vector4_scaler_addition(vector4 v, float k);

void vector4_sub_scaler(vector4* source, float k);

vector4 vector4_scaler_subtraction(vector4 v, float k);

void vector4_mult_scaler(vector4* source, float k);

vector4 vector4_scaler_multiplication(vector4 v, float k);

void vector4_div_scaler(vector4* source, float k);

vector4 vector4_scaler_division(vector4 v, float k);

float vector4_dot(vector4 v1, vector4 v2);

float vector4_magnitude(vector4 v);

float vector4_magnitude_squared(vector4 v);

bool8 vector4_perpendicular(vector4 v1, vector4 v2);

bool8 vector4_compare(vector4 v1, vector4 v2);

vector4 vector4_normalized(vector4 v);

void vector4_normalize(vector4* v);

float vector4_distance(vector4 v1, vector4 v2);

void vector4_log(vector4 v);

void vector4_log_new_line(vector4 v);