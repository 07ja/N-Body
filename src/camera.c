#include <math.h>
#include "camera.h"


Camera camera_init(void) {
    Camera cam = {.eye = {0.0f, 0.0f, 0.0f}, .viewDirection = {0.0f, 0.0f, -1.0f}, .upVector = {0.0f, 1.0f, 0.0f},
                  .pitch = 0.0f, .yaw = -90.0f,
                  .moveSpeed = 0.0015f, .mouseSensitivity = 0.25f, };
    return cam;
}

// [MOUSE] - Camera Movement
void camera_mouse(Camera* cam, float dx, float dy) {
    cam->yaw += dx * cam->mouseSensitivity;
    cam->pitch -= dy * cam->mouseSensitivity;

    if (cam->pitch > 89.0f) cam->pitch = 89.0f;
    if (cam->pitch < -89.0f) cam->pitch = -89.0f;
}

// [W], [A], [S], [D] - Camera Movement
void camera_keys(Camera* cam, const bool* keystate) {
    if (keystate[SDL_SCANCODE_W]) {
        cam->eye.x += cam->viewDirection.x * cam->moveSpeed;
        cam->eye.z += cam->viewDirection.z * cam->moveSpeed;
    }
    if (keystate[SDL_SCANCODE_S]) {
        cam->eye.x -= cam->viewDirection.x * cam->moveSpeed;
        cam->eye.z -= cam->viewDirection.z * cam->moveSpeed;
    }
    
    vec3 right = vec3_norm(vec3_cross(cam->viewDirection, cam->upVector));
    if (keystate[SDL_SCANCODE_D]) {
        cam->eye.x += right.x * cam->moveSpeed;
        cam->eye.z += right.z * cam->moveSpeed;
    }
    if (keystate[SDL_SCANCODE_A]) {
        cam->eye.x -= right.x * cam->moveSpeed;
        cam->eye.z -= right.z * cam->moveSpeed;
    }
}

void camera_update(Camera* cam) {
    float yawRad = cam->yaw * (M_PI / 180.0f);
    float pitchRad = cam->pitch * (M_PI / 180.0f);

    cam->viewDirection.x = cosf(yawRad) * cosf(pitchRad);
    cam->viewDirection.y = sinf(pitchRad);
    cam->viewDirection.z = sinf(yawRad) * cosf(pitchRad);
    cam->viewDirection = vec3_norm(cam->viewDirection);
}