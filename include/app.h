#ifndef APP_H
#define APP_H

#include <glad/glad.h>
#include <SDL3/SDL.h>
#include "camera.h"

typedef struct {
    int quit, toggle, restart, rotationLock;
    int windowWidth, windowHeight;
    SDL_Window* window;
    SDL_GLContext glContext;
    GLuint pipeline;
} Application;

Application app_init(const char* title, int width, int height);
void app_input(Application* app, Camera* cam);
int app_quit(Application* app);
void app_destroy(Application* app);

#endif