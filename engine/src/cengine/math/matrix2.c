#include "matrix2.h"

#include <stdlib.h>

#include "../core/logging.h"

matrix2 matrix2_create(vector2 row1, vector2 row2) {
    matrix2 ret;
    ret.row1 = row1;
    ret.row2 = row2;
    return ret;
}

matrix2 matrix2_identity() {
    matrix2 ret = matrix2_create(
        vector2_create(1.0f, 0.0f),
        vector2_create(0.0f, 1.0f));
    return ret;
}

float* matrix2_value_ptr(matrix2 m) {
    float* ret = malloc(sizeof(matrix2));

    ret[0] = m.row1.x;
    ret[1] = m.row1.y;

    ret[2] = m.row2.x;
    ret[3] = m.row2.y;

    return ret;
}

matrix2 matrix2_create_by_value(float val) {
    matrix2 ret = matrix2_create(
        vector2_create(val, val),
        vector2_create(val, val));
    return ret;
}

bool8 matrix2_compare(matrix2 m1, matrix2 m2) {
    return vector2_compare(m1.row1, m2.row1) &&
           vector2_compare(m1.row2, m2.row2);
}

matrix2 matrix2_scaler_addition(matrix2 m, float k) {
    matrix2 ret = matrix2_create(m.row1, m.row2);
    vector2_add_scaler(&ret.row1, k);
    vector2_add_scaler(&ret.row2, k);
    return ret;
}

void matrix2_add_scaler(matrix2* m, float k) {
    vector2_add_scaler(&m->row1, k);
    vector2_add_scaler(&m->row2, k);
}

matrix2 matrix2_scaler_subtraction(matrix2 m, float k) {
    matrix2 ret = matrix2_create(m.row1, m.row2);
    vector2_sub_scaler(&ret.row1, k);
    vector2_sub_scaler(&ret.row2, k);
    return ret;
}

void matrix2_sub_scaler(matrix2* m, float k) {
    vector2_sub_scaler(&m->row1, k);
    vector2_sub_scaler(&m->row2, k);
}

matrix2 matrix2_scaler_mul(matrix2 m, float k) {
    matrix2 ret = matrix2_create(m.row1, m.row2);
    vector2_mult_scaler(&ret.row1, k);
    vector2_mult_scaler(&ret.row2, k);
    return ret;
}

void matrix2_mul_by_scaler(matrix2* m, float k) {
    vector2_mult_scaler(&m->row1, k);
    vector2_mult_scaler(&m->row2, k);
}

matrix2 matrix2_div_scaler(matrix2 m, float k) {
    matrix2 ret = matrix2_create(m.row1, m.row2);
    vector2_div_scaler(&ret.row1, k);
    vector2_div_scaler(&ret.row2, k);
    return ret;
}

void matrix2_div_by_scaler(matrix2* m, float k) {
    vector2_div_scaler(&m->row1, k);
    vector2_div_scaler(&m->row2, k);
}

matrix2 matrix2_addation(matrix2 m1, matrix2 m2) {
    matrix2 ret = matrix2_create(m1.row1, m1.row2);
    vector2_addv(&ret.row1, m2.row1);
    vector2_addv(&ret.row2, m2.row2);
    return ret;
}

void matrix2_addm(matrix2* source, matrix2 m) {
    vector2_addv(&source->row1, m.row1);
    vector2_addv(&source->row2, m.row2);
}

matrix2 matrix2_subtraction(matrix2 m1, matrix2 m2) {
    matrix2 ret = matrix2_create(m1.row1, m1.row2);
    vector2_subv(&ret.row1, m2.row1);
    vector2_subv(&ret.row2, m2.row2);
    return ret;
}

vector2 matrix2_get_column(matrix2 m, u32 index) {
    ASSERT_MSG(index < VECTOR2_ELEMENT_COUNT, "Matrix column index out of bounds.");

    float* matrix_values = matrix2_value_ptr(m);
    u32 x_index = 0 + index;
    u32 y_index = VECTOR2_ELEMENT_COUNT + index;
    vector2 ret = vector2_create(matrix_values[x_index], matrix_values[y_index]);
    return ret;
}

vector2 matrix2_get_row(matrix2 m, u32 index) {
    ASSERT_MSG(index < VECTOR2_ELEMENT_COUNT, "Matrix column index out of bounds.");

    float* matrix_values = matrix2_value_ptr(m);
    u32 x_index = 0 + (index * VECTOR2_ELEMENT_COUNT);
    u32 y_index = 1 + (index * VECTOR2_ELEMENT_COUNT);
    vector2 ret = vector2_create(matrix_values[x_index], matrix_values[y_index]);
    return ret;
}

void matrix2_subm(matrix2* source, matrix2 m) {
    vector2_subv(&source->row1, m.row1);
    vector2_subv(&source->row2, m.row2);
}

matrix2 matrix2_multiply(matrix2 m1, matrix2 m2) {
    matrix2 ret = matrix2_create(m1.row1, m1.row2);
    vector2_multv(&ret.row1, m2.row1);
    vector2_multv(&ret.row2, m2.row2);
    return ret;
}

void matrix2_mulm(matrix2* source, matrix2 m) {
    vector2_multv(&source->row1, m.row1);
    vector2_multv(&source->row2, m.row2);
}

matrix2 matrix2_divide(matrix2 m1, matrix2 m2) {
    matrix2 ret = matrix2_create(m1.row1, m1.row2);
    vector2_divdv(&ret.row1, m2.row1);
    vector2_divdv(&ret.row2, m2.row2);
    return ret;
}

void matrix2_divm(matrix2* source, matrix2 m) {
    vector2_divdv(&source->row1, m.row1);
    vector2_divdv(&source->row2, m.row2);
}

matrix2 matrix2_transpose(matrix2 m) {
    matrix2 ret = matrix2_create_by_value(0.0f);
    ret.row1 = matrix2_get_column(m, 0);
    ret.row2 = matrix2_get_column(m, 1);
    return ret;
}

void matrix2_log(matrix2 m) {
    printf("| ");
    vector2_log(m.row1);
    printf(" |\n");
    printf("| ");
    vector2_log(m.row2);
    printf(" |\n");
}