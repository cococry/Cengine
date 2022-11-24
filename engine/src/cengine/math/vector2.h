#pragma once

#include "../core/defines.h"

typedef struct vector2 {
    float x, y;
} vector2;

#define VECTOR2_ELEMENT_COUNT 2

vector2 vector2_create(float x, float y);

float* vector2_value_ptr(vector2 vec);

void vector2_addv(vector2* source, vector2 add);

vector2 vector2_additition(vector2 v1, vector2 v2);

void vector2_subv(vector2* source, vector2 sub);

vector2 vector2_subtraction(vector2 v1, vector2 v2);

void vector2_multv(vector2* source, vector2 mult);

vector2 vector2_multiply(vector2 v1, vector2 v2);

void vector2_divdv(vector2* source, vector2 div);

vector2 vector2_divide(vector2 v1, vector2 v2);

void vector2_add_scaler(vector2* source, float k);

vector2 vector2_scaler_addition(vector2 v, float k);

void vector2_sub_scaler(vector2* source, float k);

vector2 vector2_scaler_subtraction(vector2 v, float k);

void vector2_mult_scaler(vector2* source, float k);

vector2 vector2_scaler_multiplication(vector2 v, float k);

void vector2_div_scaler(vector2* source, float k);

vector2 vector2_scaler_division(vector2 v, float k);

float vector2_dot(vector2 v1, vector2 v2);

float vector2_magnitude(vector2 v);

float vector2_magnitude_squared(vector2 v);

bool8 vector2_perpendicular(vector2 v1, vector2 v2);

bool8 vector2_compare(vector2 v1, vector2 v2);

vector2 vector2_normalized(vector2 v);

void vector2_normalize(vector2* v);

float vector2_distance(vector2 v1, vector2 v2);

void vector2_log(vector2 v);

void vector2_log_new_line(vector2 v);