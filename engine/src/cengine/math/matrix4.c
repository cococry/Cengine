#include "matrix4.h"

#include <stdlib.h>

#include "../core/logging.h"

matrix4 matrix4_create(vector4 row1, vector4 row2, vector4 row3, vector4 row4) {
    matrix4 ret;
    ret.row1 = row1;
    ret.row2 = row2;
    ret.row3 = row3;
    ret.row4 = row4;
    return ret;
}

matrix4 matrix4_identity() {
    matrix4 ret = matrix4_create(
        vector4_create(1.0f, 0.0f, 0.0f, 0.0f),
        vector4_create(0.0f, 1.0f, 0.0f, 0.0f),
        vector4_create(0.0f, 0.0f, 1.0f, 0.0f),
        vector4_create(0.0f, 0.0f, 0.0f, 1.0f));
    return ret;
}

float* matrix4_value_ptr(matrix4 m) {
    float* ret = malloc(sizeof(matrix4));

    ret[0] = m.row1.x;
    ret[1] = m.row1.y;
    ret[2] = m.row1.z;
    ret[3] = m.row1.w;

    ret[4] = m.row2.x;
    ret[5] = m.row2.y;
    ret[6] = m.row2.z;
    ret[7] = m.row2.w;

    ret[8] = m.row3.x;
    ret[9] = m.row3.y;
    ret[10] = m.row3.z;
    ret[11] = m.row3.w;

    ret[12] = m.row4.x;
    ret[13] = m.row4.y;
    ret[14] = m.row4.z;
    ret[15] = m.row4.w;

    return ret;
}

matrix4 matrix4_create_by_value(float val) {
    matrix4 ret = matrix4_create(
        vector4_create(val, val, val, val),
        vector4_create(val, val, val, val),
        vector4_create(val, val, val, val),
        vector4_create(val, val, val, val));
    return ret;
}

bool8 matrix4_compare(matrix4 m1, matrix4 m2) {
    return vector4_compare(m1.row1, m2.row1) &&
           vector4_compare(m1.row2, m2.row2) &&
           vector4_compare(m1.row3, m2.row3) &&
           vector4_compare(m1.row4, m2.row4);
}

matrix4 matrix4_scaler_addition(matrix4 m, float k) {
    matrix4 ret = matrix4_create(m.row1, m.row2, m.row3, m.row4);
    vector4_add_scaler(&ret.row1, k);
    vector4_add_scaler(&ret.row2, k);
    vector4_add_scaler(&ret.row3, k);
    vector4_add_scaler(&ret.row4, k);
    return ret;
}

void matrix4_add_scaler(matrix4* m, float k) {
    vector4_add_scaler(&m->row1, k);
    vector4_add_scaler(&m->row2, k);
    vector4_add_scaler(&m->row3, k);
    vector4_add_scaler(&m->row4, k);
}

matrix4 matrix4_scaler_subtraction(matrix4 m, float k) {
    matrix4 ret = matrix4_create(m.row1, m.row2, m.row3, m.row4);
    vector4_sub_scaler(&ret.row1, k);
    vector4_sub_scaler(&ret.row2, k);
    vector4_sub_scaler(&ret.row3, k);
    vector4_sub_scaler(&ret.row4, k);
    return ret;
}

void matrix4_sub_scaler(matrix4* m, float k) {
    vector4_sub_scaler(&m->row1, k);
    vector4_sub_scaler(&m->row2, k);
    vector4_sub_scaler(&m->row3, k);
    vector4_sub_scaler(&m->row4, k);
}

matrix4 matrix4_scaler_mul(matrix4 m, float k) {
    matrix4 ret = matrix4_create(m.row1, m.row2, m.row3, m.row4);
    vector4_mult_scaler(&ret.row1, k);
    vector4_mult_scaler(&ret.row2, k);
    vector4_mult_scaler(&ret.row3, k);
    vector4_mult_scaler(&ret.row4, k);
    return ret;
}

void matrix4_mul_by_scaler(matrix4* m, float k) {
    vector4_mult_scaler(&m->row1, k);
    vector4_mult_scaler(&m->row2, k);
    vector4_mult_scaler(&m->row3, k);
    vector4_mult_scaler(&m->row4, k);
}

matrix4 matrix4_div_scaler(matrix4 m, float k) {
    matrix4 ret = matrix4_create(m.row1, m.row2, m.row3, m.row4);
    vector4_div_scaler(&ret.row1, k);
    vector4_div_scaler(&ret.row2, k);
    vector4_div_scaler(&ret.row3, k);
    vector4_div_scaler(&ret.row4, k);
    return ret;
}

void matrix4_div_by_scaler(matrix4* m, float k) {
    vector4_div_scaler(&m->row1, k);
    vector4_div_scaler(&m->row2, k);
    vector4_div_scaler(&m->row3, k);
    vector4_div_scaler(&m->row4, k);
}

matrix4 matrix4_addation(matrix4 m1, matrix4 m2) {
    matrix4 ret = matrix4_create(m1.row1, m1.row2, m1.row3, m1.row4);
    vector4_addv(&ret.row1, m2.row1);
    vector4_addv(&ret.row2, m2.row2);
    vector4_addv(&ret.row3, m2.row3);
    vector4_addv(&ret.row4, m2.row4);
    return ret;
}

void matrix4_addm(matrix4* source, matrix4 m) {
    vector4_addv(&source->row1, m.row1);
    vector4_addv(&source->row2, m.row2);
    vector4_addv(&source->row3, m.row3);
    vector4_addv(&source->row4, m.row4);
}

matrix4 matrix4_subtraction(matrix4 m1, matrix4 m2) {
    matrix4 ret = matrix4_create(m1.row1, m1.row2, m1.row3, m1.row4);
    vector4_subv(&ret.row1, m2.row1);
    vector4_subv(&ret.row2, m2.row2);
    vector4_subv(&ret.row3, m2.row3);
    vector4_subv(&ret.row4, m2.row4);
    return ret;
}

vector4 matrix4_get_column(matrix4 m, u32 index) {
    u32 index_r = index--;
    ASSERT_MSG(index_r <= VECTOR4_ELEMENT_COUNT, "Matrix column index out of bounds.");

    float* matrix_values = matrix4_value_ptr(m);
    u32 x_index = 0 + index_r;
    u32 y_index = VECTOR4_ELEMENT_COUNT + index_r;
    u32 z_index = VECTOR4_ELEMENT_COUNT * 2 + index_r;
    u32 w_index = VECTOR4_ELEMENT_COUNT * 3 + index_r;
    vector4 ret = vector4_create(matrix_values[x_index], matrix_values[y_index],
                                 matrix_values[z_index], matrix_values[w_index]);
    return ret;
}

vector4 matrix4_get_row(matrix4 m, u32 index) {
    u32 index_r = index--;
    ASSERT_MSG(index_r <= VECTOR4_ELEMENT_COUNT, "Matrix column index out of bounds.");

    float* matrix_values = matrix4_value_ptr(m);
    u32 x_index = 0 + (index_r * VECTOR4_ELEMENT_COUNT);
    u32 y_index = 1 + (index_r * VECTOR4_ELEMENT_COUNT);
    u32 z_index = 2 + (index_r * VECTOR4_ELEMENT_COUNT);
    u32 w_index = 3 + (index_r * VECTOR4_ELEMENT_COUNT);
    vector4 ret = vector4_create(matrix_values[x_index], matrix_values[y_index],
                                 matrix_values[z_index], matrix_values[w_index]);
    return ret;
}

void matrix4_subm(matrix4* source, matrix4 m) {
    vector4_subv(&source->row1, m.row1);
    vector4_subv(&source->row2, m.row2);
    vector4_subv(&source->row3, m.row3);
    vector4_subv(&source->row4, m.row4);
}

matrix4 matrix4_multiply(matrix4 m1, matrix4 m2) {
    matrix4 ret;

    vector4 m2_col1 = matrix4_get_column(m2, 0);
    vector4 m2_col2 = matrix4_get_column(m2, 1);
    vector4 m2_col3 = matrix4_get_column(m2, 2);
    vector4 m2_col4 = matrix4_get_column(m2, 3);

    ret.row1.x = m1.row1.x * m2_col1.x + m1.row1.y * m2_col1.y + m1.row1.z * m2_col1.z + m1.row1.w * m2_col1.w;
    ret.row1.y = m1.row1.x * m2_col2.x + m1.row1.y * m2_col2.y + m1.row1.z * m2_col2.z + m1.row1.w * m2_col2.w;
    ret.row1.z = m1.row1.x * m2_col3.x + m1.row1.y * m2_col3.y + m1.row1.z * m2_col3.z + m1.row1.w * m2_col3.w;
    ret.row1.w = m1.row1.x * m2_col4.x + m1.row1.y * m2_col4.y + m1.row1.z * m2_col4.z + m1.row1.w * m2_col4.w;

    ret.row2.x = m1.row2.x * m2_col1.x + m1.row2.y * m2_col1.y + m1.row2.z * m2_col1.z + m1.row2.w * m2_col1.w;
    ret.row2.y = m1.row2.x * m2_col2.x + m1.row2.y * m2_col2.y + m1.row2.z * m2_col2.z + m1.row2.w * m2_col2.w;
    ret.row2.z = m1.row2.x * m2_col3.x + m1.row2.y * m2_col3.y + m1.row2.z * m2_col3.z + m1.row2.w * m2_col3.w;
    ret.row2.w = m1.row2.x * m2_col4.x + m1.row2.y * m2_col4.y + m1.row2.z * m2_col4.z + m1.row2.w * m2_col4.w;

    ret.row3.x = m1.row3.x * m2_col1.x + m1.row3.y * m2_col1.y + m1.row3.z * m2_col1.z + m1.row3.w * m2_col1.w;
    ret.row3.y = m1.row3.x * m2_col2.x + m1.row3.y * m2_col2.y + m1.row3.z * m2_col2.z + m1.row3.w * m2_col2.w;
    ret.row3.z = m1.row3.x * m2_col3.x + m1.row3.y * m2_col3.y + m1.row3.z * m2_col3.z + m1.row3.w * m2_col3.w;
    ret.row3.w = m1.row3.x * m2_col4.x + m1.row3.y * m2_col4.y + m1.row3.z * m2_col4.z + m1.row3.w * m2_col4.w;

    ret.row4.x = m1.row4.x * m2_col1.x + m1.row4.y * m2_col1.y + m1.row4.z * m2_col1.z + m1.row4.w * m2_col1.w;
    ret.row4.y = m1.row4.x * m2_col2.x + m1.row4.y * m2_col2.y + m1.row4.z * m2_col2.z + m1.row4.w * m2_col2.w;
    ret.row4.z = m1.row4.x * m2_col3.x + m1.row4.y * m2_col3.y + m1.row4.z * m2_col3.z + m1.row4.w * m2_col3.w;
    ret.row4.w = m1.row4.x * m2_col4.x + m1.row4.y * m2_col4.y + m1.row4.z * m2_col4.z + m1.row4.w * m2_col4.w;

    return ret;
}

void matrix4_multm(matrix4* source, matrix4 m) {
    matrix4 res = matrix4_multiply(*source, m);
    source = &res;
}

matrix4 matrix4_divide(matrix4 m1, matrix4 m2) {
    matrix4 ret = matrix4_create(m1.row1, m1.row2, m1.row3, m1.row4);
    vector4_divdv(&ret.row1, m2.row1);
    vector4_divdv(&ret.row2, m2.row2);
    vector4_divdv(&ret.row3, m2.row3);
    vector4_divdv(&ret.row4, m2.row4);
    return ret;
}

void matrix4_divm(matrix4* source, matrix4 m) {
    vector4_divdv(&source->row1, m.row1);
    vector4_divdv(&source->row2, m.row2);
    vector4_divdv(&source->row3, m.row3);
    vector4_divdv(&source->row4, m.row4);
}

matrix4 matrix4_transpose(matrix4 m) {
    matrix4 ret = matrix4_create_by_value(0.0f);
    ret.row1 = matrix4_get_column(m, 1);
    ret.row2 = matrix4_get_column(m, 2);
    ret.row3 = matrix4_get_column(m, 3);
    ret.row4 = matrix4_get_column(m, 4);
    return ret;
}

void matrix4_log(matrix4 m) {
    printf("| ");
    vector4_log(m.row1);
    printf(" |\n");
    printf("| ");
    vector4_log(m.row2);
    printf(" |\n");
    printf("| ");
    vector4_log(m.row3);
    printf(" |\n");
    printf("| ");
    vector4_log(m.row4);
    printf(" |\n");
}