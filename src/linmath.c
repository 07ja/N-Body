#include <math.h>
#include "linmath.h"


// vvvvvvvvvvvvvvv mat4 Functions vvvvvvvvvvvvvvv
mat4 mat4_identity(void) {
    mat4 result = {0};
    result.m[0][0] = 1.0f;
    result.m[1][1] = 1.0f;
    result.m[2][2] = 1.0f;
    result.m[3][3] = 1.0f;
    return result;
}

mat4 mat4_perspective(float fovyRadians, float aspect, float near, float far) {
    float f = 1.0f / tanf(fovyRadians / 2.0f);
    float nf = 1.0f / (near - far);

    mat4 result = {0};
    result.m[0][0] = f / aspect;
    result.m[1][1] = f;
    result.m[2][2] = (far + near) * nf;
    result.m[2][3] = -1.0f;
    result.m[3][2] = (2.0f * far * near) * nf;

    return result;
}

mat4 mat4_rotate(mat4 m, float radians, vec3 axis) {
    float c = cosf(radians);
    float s = sinf(radians);
    float one_minus_c = 1.0f - c;

    float length = sqrtf(axis.x * axis.x + axis.y * axis.y + axis.z * axis.z);
    if (length == 0.0f) {
        return m; 
    }

    float x = axis.x / length;
    float y = axis.y / length;
    float z = axis.z / length;

    mat4 rot = {0};
    rot.m[0][0] = c + x * x * one_minus_c;
    rot.m[0][1] = y * x * one_minus_c + z * s;
    rot.m[0][2] = z * x * one_minus_c - y * s;

    rot.m[1][0] = x * y * one_minus_c - z * s;
    rot.m[1][1] = c + y * y * one_minus_c;
    rot.m[1][2] = z * y * one_minus_c + x * s;

    rot.m[2][0] = x * z * one_minus_c + y * s;
    rot.m[2][1] = y * z * one_minus_c - x * s;
    rot.m[2][2] = c + z * z * one_minus_c;

    rot.m[3][3] = 1.0f;

    mat4 result = {0};
    for (int col = 0; col < 4; ++col) {
        for (int row = 0; row < 4; ++row) {
            for (int i = 0; i < 4; ++i) {
                result.m[col][row] += m.m[i][row] * rot.m[col][i];
            }
        }
    }

    return result;
}

mat4 mat4_scale(mat4 m, vec3 v) {
    mat4 result = {0};
    for (int col = 0; col < 4; ++col) {
        result.m[col][0] = m.m[col][0] * v.x;
        result.m[col][1] = m.m[col][1] * v.y;
        result.m[col][2] = m.m[col][2] * v.z;
        result.m[col][3] = m.m[col][3];
    }
    return result;
}

mat4 mat4_translate(mat4 m, vec3 v) {
    m.m[3][0] += v.x;
    m.m[3][1] += v.y;
    m.m[3][2] += v.z;
    return m;
}

mat4 mat4_view(vec3 eye, vec3 center, vec3 up) {
    vec3 f = vec3_norm(vec3_sub(center, eye));
    vec3 s = vec3_norm(vec3_cross(f, up));
    vec3 u = vec3_cross(s, f);

    mat4 result = mat4_identity();

    result.m[0][0] = s.x;
    result.m[1][0] = s.y;
    result.m[2][0] = s.z;

    result.m[0][1] = u.x;
    result.m[1][1] = u.y;
    result.m[2][1] = u.z;

    result.m[0][2] = -f.x;
    result.m[1][2] = -f.y;
    result.m[2][2] = -f.z;

    result.m[3][0] = -vec3_dot(s, eye);
    result.m[3][1] = -vec3_dot(u, eye);
    result.m[3][2] =  vec3_dot(f, eye);

    return result;
}


// vvvvvvvvvvvvvvv vec3 Functions vvvvvvvvvvvvvvv
vec3 vec3_add(vec3 a, vec3 b) {
    return (vec3){a.x + b.x, a.y + b.y, a.z + b.z};
}

vec3 vec3_cross(vec3 a, vec3 b) {
    return (vec3){
        a.y * b.z - a.z * b.y,
        a.z * b.x - a.x * b.z,
        a.x * b.y - a.y * b.x
    };
}

float vec3_dot(vec3 a, vec3 b) {
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

float vec3_len(vec3 v) {
    return sqrtf(v.x * v.x + v.y * v.y + v.z * v.z);
}

vec3 vec3_norm(vec3 v) {
    float len = sqrtf(v.x * v.x + v.y * v.y + v.z * v.z);
    return (vec3){ v.x / len, v.y / len, v.z / len };
}

vec3 vec3_scale(vec3 v, float s) {
    return (vec3){v.x * s, v.y * s, v.z * s};
}

vec3 vec3_sub(vec3 a, vec3 b) {
    return (vec3){ a.x - b.x, a.y - b.y, a.z - b.z };
}