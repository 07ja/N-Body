#ifndef LINMATH_H
#define LINMATH_H

// vvvvvvvvvvvvvvv Structs vvvvvvvvvvvvvvv
typedef struct {
    float m[4][4];
} mat4;

typedef struct {
    float x, y, z;
} vec3;

// vvvvvvvvvvvvvvv mat4 Functions vvvvvvvvvvvvvvv
mat4 mat4_identity(void);
mat4 mat4_perspective(float fovyRadians, float aspect, float near, float far);
mat4 mat4_rotate(mat4 m, float radians, vec3 axis);
mat4 mat4_scale(mat4 m, vec3 v);
mat4 mat4_translate(mat4 m, vec3 v);
mat4 mat4_view(vec3 eye, vec3 center, vec3 up);


// vvvvvvvvvvvvvvv vec3 Functions vvvvvvvvvvvvvvv
vec3 vec3_add(vec3 a, vec3 b);
vec3 vec3_cross(vec3 a, vec3 b);
float vec3_dot(vec3 a, vec3 b);
float vec3_length(vec3 v);
vec3 vec3_norm(vec3 v);
vec3 vec3_scale(vec3 v, float s);
vec3 vec3_sub(vec3 a, vec3 b);


#endif