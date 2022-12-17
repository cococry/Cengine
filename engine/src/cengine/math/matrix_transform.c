#include "matrix_transform.h"

#include <math.h>

matrix4 translate_mv(matrix4 m, vector3 v) {
    matrix4 ret = m;
    ret.row4 = vector4_additition(vector4_additition(
                                      vector4_scaler_multiplication(m.row1, v.x),
                                      vector4_scaler_multiplication(m.row2, v.y)),
                                  vector4_additition(vector4_scaler_multiplication(m.row3, v.z),
                                                     m.row4));

    return ret;
}

matrix4 rotate_mv_angle(matrix4 m, float angle, vector3 v) {
    const float a = angle;
    const float c = (float)cos(a);
    const float s = (float)sin(a);

    vector3 axis = vector3_normalized(v);

    vector3 tmp = vector3_create(1 - c, 1 - c, 1 - c);

    vector3_multv(&tmp, axis);

    matrix4 rotate = matrix4_create_by_value(0.0f);

    rotate.row1 = vector4_create(
        c + tmp.x * axis.x,
        tmp.x * axis.y + s * axis.z,
        tmp.x * axis.z - s * axis.y,
        0.0f);

    rotate.row2 = vector4_create(
        tmp.y * axis.x - s * axis.z,
        c + tmp.y * axis.y,
        tmp.y * axis.z + s * axis.x,
        0.0f);
    rotate.row3 = vector4_create(
        tmp.z * axis.x + s * axis.y,
        tmp.z * axis.y - s * axis.x,
        c + tmp.z * axis.z,
        0.0f);

    vector4 row1 = vector4_additition(
        vector4_additition(
            vector4_scaler_multiplication(
                matrix4_get_column(m, 0),
                rotate.row1.x),
            vector4_scaler_multiplication(
                matrix4_get_column(m, 1),
                rotate.row1.y)),

        vector4_scaler_multiplication(
            matrix4_get_column(m, 2),
            rotate.row1.z));
    vector4 row2 = vector4_additition(
        vector4_additition(
            vector4_scaler_multiplication(
                matrix4_get_column(m, 0),
                rotate.row2.x),
            vector4_scaler_multiplication(
                matrix4_get_column(m, 1),
                rotate.row2.y)),
        vector4_scaler_multiplication(matrix4_get_column(m, 2), rotate.row2.z));
    vector4 row3 = vector4_additition(
        vector4_additition(
            vector4_scaler_multiplication(
                matrix4_get_column(m, 0),
                rotate.row3.x),
            vector4_scaler_multiplication(
                matrix4_get_column(m, 1),
                rotate.row3.y)),
        vector4_scaler_multiplication(matrix4_get_column(m, 2), rotate.row3.z));

    vector4 row4 = matrix4_get_column(m, 3);

    matrix4 ret = matrix4_create_by_value(0.0f);
    ret.row1 = row1;
    ret.row2 = row2;
    ret.row3 = row3;
    ret.row4 = row4;

    return ret;
}

matrix4 scale_mv(matrix4 m, vector3 v) {
    matrix4 ret;
    ret.row1 = vector4_scaler_multiplication(m.row1, v.x);
    ret.row2 = vector4_scaler_multiplication(m.row2, v.y);
    ret.row3 = vector4_scaler_multiplication(m.row3, v.z);
    ret.row4 = m.row4;
    return ret;
}

matrix4 look_at_matrix(vector3 eye, vector3 center, vector3 up) {
    const vector3 f = vector3_normalized(vector3_subtraction(center, eye));
    const vector3 s = vector3_normalized(vector3_cross(f, up));
    const vector3 u = vector3_cross(s, f);

    matrix4 ret = matrix4_identity();
    ret.row1.x = s.x;
    ret.row2.x = s.y;
    ret.row3.x = s.z;
    ret.row1.y = u.x;
    ret.row2.y = u.y;
    ret.row3.y = u.z;
    ret.row1.z = -f.x;
    ret.row2.z = -f.y;
    ret.row3.z = -f.z;
    ret.row4.x = -vector3_dot(s, eye);
    ret.row4.y = -vector3_dot(u, eye);
    ret.row4.z = vector3_dot(f, eye);

    return ret;
}

matrix4 perspective_matrix(float fov, float aspect, float near, float far) {
    const float tan_half_fov = (float)tan(fov / 2);

    matrix4 ret = matrix4_create_by_value(0.0f);
    ret.row1.x = 1 / (aspect * tan_half_fov);
    ret.row2.y = 1 / tan_half_fov;
    ret.row3.z = -(far + near) / (far - near);
    ret.row3.w = -1;
    ret.row4.z = -(2 * far * near) / (far - near);

    return ret;
}

matrix4 orthographic_matrix(float left, float right, float bottom, float top) {
    matrix4 ret = matrix4_identity();
    ret.row1.x = 2 / (right - left);
    ret.row2.y = 2 / (top - bottom);
    ret.row3.z = -1;
    ret.row4.x = -(right + left) / (right - left);
    ret.row4.y = -(top + bottom) / (top - bottom);

    return ret;
}