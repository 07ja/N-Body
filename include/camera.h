#ifndef CAMERA_H
#define CAMERA_H

#include <SDL3/SDL.h>
#include "linmath.h"

typedef struct {
    vec3 eye;
    vec3 viewDirection;
    vec3 upVector;
    float pitch;
    float yaw;
    float moveSpeed;
    float mouseSensitivity;
} Camera;

Camera camera_init(void);
void camera_mouse(Camera* cam, float dx, float dy);
void camera_keys(Camera* cam, const bool* keystate);
void camera_update(Camera* cam);

#endif