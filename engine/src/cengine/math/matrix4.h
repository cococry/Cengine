#pragma once

#include "vector4.h"

typedef struct matrix4 {
    vector4 row1, row2, row3, row4;
} matrix4;

matrix4 matrix4_create(vector4 row1, vector4 row2, vector4 row3, vector4 row4);

matrix4 matrix4_identity();

float* matrix4_value_ptr(matrix4 m);

matrix4 matrix4_create_by_value(float val);

bool8 matrix4_compare(matrix4 m1, matrix4 m2);

matrix4 matrix4_scaler_addition(matrix4 m, float k);

void matrix4_add_scaler(matrix4* m, float k);

matrix4 matrix4_scaler_subtraction(matrix4 m, float k);

void matrix4_sub_scaler(matrix4* m, float k);

matrix4 matrix4_scaler_mul(matrix4 m, float k);

void matrix4_mul_by_scaler(matrix4* m, float k);

matrix4 matrix4_div_scaler(matrix4 m, float k);

void matrix4_div_by_scaler(matrix4* m, float k);

matrix4 matrix4_addation(matrix4 m1, matrix4 m2);

void matrix4_addm(matrix4* source, matrix4 m);

matrix4 matrix4_subtraction(matrix4 m1, matrix4 m2);

void matrix4_subm(matrix4* source, matrix4 m);

vector4 matrix4_get_column(matrix4 m, u32 index);

vector4 matrix4_get_row(matrix4 m, u32 index);

matrix4 matrix4_multiply(matrix4 m1, matrix4 m2);

void matrix4_multm(matrix4* source, matrix4 m);

matrix4 matrix4_divide(matrix4 m1, matrix4 m2);

void matrix4_divm(matrix4* source, matrix4 m);

matrix4 matrix4_transpose(matrix4 m);

void matrix4_log(matrix4 m);

matrix4 matrix4_inverse(matrix4 m);

vector4 vector4_multiply_matrix4(matrix4 m, vector4 v);