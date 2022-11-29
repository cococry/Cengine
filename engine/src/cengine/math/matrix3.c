#include "matrix3.h"

#include <stdlib.h>

#include "../core/logging.h"

matrix3 matrix3_create(vector3 row1, vector3 row2, vector3 row3) {
    matrix3 ret;
    ret.row1 = row1;
    ret.row2 = row2;
    ret.row3 = row3;
    return ret;
}

matrix3 matrix3_identity() {
    matrix3 ret = matrix3_create(
        vector3_create(1.0f, 0.0f, 0.0f),
        vector3_create(0.0f, 1.0f, 0.0f),
        vector3_create(0.0f, 0.0f, 1.0f));
    return ret;
}

float* matrix3_value_ptr(matrix3 m) {
    float* ret = malloc(sizeof(matrix3));

    ret[0] = m.row1.x;
    ret[1] = m.row1.y;
    ret[2] = m.row1.z;

    ret[3] = m.row2.x;
    ret[4] = m.row2.y;
    ret[5] = m.row2.z;

    ret[6] = m.row3.x;
    ret[7] = m.row3.y;
    ret[8] = m.row3.z;

    return ret;
}

matrix3 matrix3_create_by_value(float val) {
    matrix3 ret = matrix3_create(
        vector3_create(val, val, val),
        vector3_create(val, val, val),
        vector3_create(val, val, val));
    return ret;
}

bool8 matrix3_compare(matrix3 m1, matrix3 m2) {
    return vector3_compare(m1.row1, m2.row1) &&
           vector3_compare(m1.row2, m2.row2) &&
           vector3_compare(m1.row3, m2.row3);
}

matrix3 matrix3_scaler_addition(matrix3 m, float k) {
    matrix3 ret = matrix3_create(m.row1, m.row2, m.row3);
    vector3_add_scaler(&ret.row1, k);
    vector3_add_scaler(&ret.row2, k);
    vector3_add_scaler(&ret.row3, k);
    return ret;
}

void matrix3_add_scaler(matrix3* m, float k) {
    vector3_add_scaler(&m->row1, k);
    vector3_add_scaler(&m->row2, k);
    vector3_add_scaler(&m->row3, k);
}

matrix3 matrix3_scaler_subtraction(matrix3 m, float k) {
    matrix3 ret = matrix3_create(m.row1, m.row2, m.row3);
    vector3_sub_scaler(&ret.row1, k);
    vector3_sub_scaler(&ret.row2, k);
    vector3_sub_scaler(&ret.row3, k);
    return ret;
}

void matrix3_sub_scaler(matrix3* m, float k) {
    vector3_sub_scaler(&m->row1, k);
    vector3_sub_scaler(&m->row2, k);
    vector3_sub_scaler(&m->row3, k);
}

matrix3 matrix3_scaler_mul(matrix3 m, float k) {
    matrix3 ret = matrix3_create(m.row1, m.row2, m.row3);
    vector3_mult_scaler(&ret.row1, k);
    vector3_mult_scaler(&ret.row2, k);
    vector3_mult_scaler(&ret.row3, k);
    return ret;
}

void matrix3_mul_by_scaler(matrix3* m, float k) {
    vector3_mult_scaler(&m->row1, k);
    vector3_mult_scaler(&m->row2, k);
    vector3_mult_scaler(&m->row3, k);
}

matrix3 matrix3_div_scaler(matrix3 m, float k) {
    matrix3 ret = matrix3_create(m.row1, m.row2, m.row3);
    vector3_div_scaler(&ret.row1, k);
    vector3_div_scaler(&ret.row2, k);
    vector3_div_scaler(&ret.row3, k);
    return ret;
}

void matrix3_div_by_scaler(matrix3* m, float k) {
    vector3_div_scaler(&m->row1, k);
    vector3_div_scaler(&m->row2, k);
    vector3_div_scaler(&m->row3, k);
}

matrix3 matrix3_addation(matrix3 m1, matrix3 m2) {
    matrix3 ret = matrix3_create(m1.row1, m1.row2, m1.row3);
    vector3_addv(&ret.row1, m2.row1);
    vector3_addv(&ret.row2, m2.row2);
    vector3_addv(&ret.row3, m2.row3);
    return ret;
}

void matrix3_addm(matrix3* source, matrix3 m) {
    vector3_addv(&source->row1, m.row1);
    vector3_addv(&source->row2, m.row2);
    vector3_addv(&source->row3, m.row3);
}

matrix3 matrix3_subtraction(matrix3 m1, matrix3 m2) {
    matrix3 ret = matrix3_create(m1.row1, m1.row2, m1.row3);
    vector3_subv(&ret.row1, m2.row1);
    vector3_subv(&ret.row2, m2.row2);
    vector3_subv(&ret.row3, m2.row3);
    return ret;
}

vector3 matrix3_get_column(matrix3 m, u32 index) {
    ASSERT_MSG(index < VECTOR3_ELEMENT_COUNT, "Matrix column index out of bounds.");

    float* matrix_values = matrix3_value_ptr(m);
    u32 x_index = 0 + index;
    u32 y_index = VECTOR3_ELEMENT_COUNT + index;
    u32 z_index = VECTOR3_ELEMENT_COUNT * 2 + index;
    vector3 ret = vector3_create(matrix_values[x_index], matrix_values[y_index],
                                 matrix_values[z_index]);
    return ret;
}

vector3 matrix3_get_row(matrix3 m, u32 index) {
    ASSERT_MSG(index < VECTOR3_ELEMENT_COUNT, "Matrix column index out of bounds.");

    float* matrix_values = matrix3_value_ptr(m);
    u32 x_index = 0 + (index * VECTOR3_ELEMENT_COUNT);
    u32 y_index = 1 + (index * VECTOR3_ELEMENT_COUNT);
    u32 z_index = 2 + (index * VECTOR3_ELEMENT_COUNT);
    vector3 ret = vector3_create(matrix_values[x_index], matrix_values[y_index],
                                 matrix_values[z_index]);
    return ret;
}

void matrix3_subm(matrix3* source, matrix3 m) {
    vector3_subv(&source->row1, m.row1);
    vector3_subv(&source->row2, m.row2);
    vector3_subv(&source->row3, m.row3);
}

matrix3 matrix3_multiply(matrix3 m1, matrix3 m2) {
    matrix3 ret;

    vector3 m2_col1 = matrix3_get_column(m2, 0);
    vector3 m2_col2 = matrix3_get_column(m2, 1);
    vector3 m2_col3 = matrix3_get_column(m2, 2);

    ret.row1.x = m1.row1.x * m2_col1.x + m1.row1.y * m2_col1.y + m1.row1.z * m2_col1.z;
    ret.row1.y = m1.row1.x * m2_col2.x + m1.row1.y * m2_col2.y + m1.row1.z * m2_col2.z;
    ret.row1.z = m1.row1.x * m2_col3.x + m1.row1.y * m2_col3.y + m1.row1.z * m2_col3.z;

    ret.row2.x = m1.row2.x * m2_col1.x + m1.row2.y * m2_col1.y + m1.row2.z * m2_col1.z;
    ret.row2.y = m1.row2.x * m2_col2.x + m1.row2.y * m2_col2.y + m1.row2.z * m2_col2.z;
    ret.row2.z = m1.row2.x * m2_col3.x + m1.row2.y * m2_col3.y + m1.row2.z * m2_col3.z;

    ret.row3.x = m1.row3.x * m2_col1.x + m1.row3.y * m2_col1.y + m1.row3.z * m2_col1.z;
    ret.row3.y = m1.row3.x * m2_col2.x + m1.row3.y * m2_col2.y + m1.row3.z * m2_col2.z;
    ret.row3.z = m1.row3.x * m2_col3.x + m1.row3.y * m2_col3.y + m1.row3.z * m2_col3.z;

    return ret;
}

void matrix3_mulm(matrix3* source, matrix3 m) {
    matrix3 res = matrix3_multiply(*source, m);
    source = &res;
}

matrix3 matrix3_divide(matrix3 m1, matrix3 m2) {
    matrix3 ret = matrix3_create(m1.row1, m1.row2, m1.row3);
    vector3_divdv(&ret.row1, m2.row1);
    vector3_divdv(&ret.row2, m2.row2);
    vector3_divdv(&ret.row3, m2.row3);
    return ret;
}

void matrix3_divm(matrix3* source, matrix3 m) {
    vector3_divdv(&source->row1, m.row1);
    vector3_divdv(&source->row2, m.row2);
    vector3_divdv(&source->row3, m.row3);
}

matrix3 matrix3_transpose(matrix3 m) {
    matrix3 ret = matrix3_create_by_value(0.0f);
    ret.row1 = matrix3_get_column(m, 0);
    ret.row2 = matrix3_get_column(m, 1);
    ret.row3 = matrix3_get_column(m, 2);
    return ret;
}

void matrix3_log(matrix3 m) {
    printf("| ");
    vector3_log(m.row1);
    printf(" |\n");
    printf("| ");
    vector3_log(m.row2);
    printf(" |\n");
    printf("| ");
    vector3_log(m.row3);
    printf(" |\n");
}