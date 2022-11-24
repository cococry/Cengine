#pragma once

#include "vector.h"
#include "matrix.h"

matrix4 translate_mv(matrix4 m, vector3 v);

matrix4 rotate_mv_angle(matrix4 m, float angle, vector3 v);

matrix4 scale_mv(matrix4 m, vector3 v);

matrix4 look_at_matrix(vector3 eye, vector3 center, vector3 up);

matrix4 perspective_matrix(float fov, float aspect, float near, float far);

matrix4 orthographic_matrix(float left, float right, float bottom, float top);