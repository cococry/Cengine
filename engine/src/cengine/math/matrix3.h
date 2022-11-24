#pragma once

#include "vector3.h"

typedef struct matrix3 {
    vector3 row1, row2, row3;
} matrix3;

matrix3 matrix3_create(vector3 row1, vector3 row2, vector3 row3);

matrix3 matrix3_identity();

float* matrix3_value_ptr(matrix3 m);

matrix3 matrix3_create_by_value(float val);

bool8 matrix3_compare(matrix3 m1, matrix3 m2);

matrix3 matrix3_scaler_addition(matrix3 m, float k);

void matrix3_add_scaler(matrix3* m, float k);

matrix3 matrix3_scaler_subtraction(matrix3 m, float k);

void matrix3_sub_scaler(matrix3* m, float k);

matrix3 matrix3_scaler_mul(matrix3 m, float k);

void matrix3_mul_by_scaler(matrix3* m, float k);

matrix3 matrix3_div_scaler(matrix3 m, float k);

void matrix3_div_by_scaler(matrix3* m, float k);

matrix3 matrix3_addation(matrix3 m1, matrix3 m2);

void matrix3_addm(matrix3* source, matrix3 m);

matrix3 matrix3_subtraction(matrix3 m1, matrix3 m2);

void matrix3_subm(matrix3* source, matrix3 m);

vector3 matrix3_get_column(matrix3 m, u32 index);

vector3 matrix3_get_row(matrix3 m, u32 index);

matrix3 matrix3_multiply(matrix3 m1, matrix3 m2);

void matrix3_mulm(matrix3* source, matrix3 m);

matrix3 matrix3_divide(matrix3 m1, matrix3 m2);

void matrix3_divm(matrix3* source, matrix3 m);

matrix3 matrix3_transpose(matrix3 m);

void matrix3_log(matrix3 m);