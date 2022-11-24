#pragma once

#include "vector2.h"

typedef struct matrix2 {
    vector2 row1, row2;
} matrix2;

matrix2 matrix2_create(vector2 row1, vector2 row2);

matrix2 matrix2_identity();

float* matrix2_value_ptr(matrix2 m);

matrix2 matrix2_create_by_value(float val);

bool8 matrix2_compare(matrix2 m1, matrix2 m2);

matrix2 matrix2_scaler_addition(matrix2 m, float k);

void matrix2_add_scaler(matrix2* m, float k);

matrix2 matrix2_scaler_subtraction(matrix2 m, float k);

void matrix2_sub_scaler(matrix2* m, float k);

matrix2 matrix2_scaler_mul(matrix2 m, float k);

void matrix2_mul_by_scaler(matrix2* m, float k);

matrix2 matrix2_div_scaler(matrix2 m, float k);

void matrix2_div_by_scaler(matrix2* m, float k);

matrix2 matrix2_addation(matrix2 m1, matrix2 m2);

void matrix2_addm(matrix2* source, matrix2 m);

matrix2 matrix2_subtraction(matrix2 m1, matrix2 m2);

void matrix2_subm(matrix2* source, matrix2 m);

vector2 matrix2_get_column(matrix2 m, u32 index);

vector2 matrix2_get_row(matrix2 m, u32 index);

matrix2 matrix2_multiply(matrix2 m1, matrix2 m2);

void matrix2_mulm(matrix2* source, matrix2 m);

matrix2 matrix2_divide(matrix2 m1, matrix2 m2);

void matrix2_divm(matrix2* source, matrix2 m);

matrix2 matrix2_transpose(matrix2 m);

void matrix2_log(matrix2 m);