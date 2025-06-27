#include <glad/glad.h>
#include <SDL3/SDL.h>
#include <stdio.h>
#include "app.h"


Application app_init(const char* title, int width, int height) {
    Application app = {.quit = 0, .toggle = 0, .restart = 0, .rotationLock = 0, .windowWidth = 960, .windowHeight = 540,
                       .window = NULL, .glContext = NULL, .pipeline = 0, };

    // Initialize Video
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        fprintf(stderr, "Error initializing SDL: %s\n", SDL_GetError());
    }

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

    // Create Window
    app.window = SDL_CreateWindow(title, app.windowWidth, app.windowHeight, SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL);
    if (!app.window) {
        fprintf(stderr, "Error creating SDL window: %s\n", SDL_GetError());
    }

    // Attach Mouse to Screen
    SDL_CaptureMouse(1);
    SDL_SetWindowRelativeMouseMode(app.window, 1);

    // Creating OpenGL Context
    app.glContext = SDL_GL_CreateContext(app.window);
    if (!app.glContext) {
        fprintf(stderr, "Error creating openGL context: %s\n", SDL_GetError());
    }

    // Initialize Glad
    if (!gladLoadGL()) {
        fprintf(stderr, "Failed to initialize GLAD\n");
    }

    printf("Vendor: %s\n", glGetString(GL_VENDOR));
    printf("Renderer: %s\n", glGetString(GL_RENDERER));
    printf("Version: %s\n", glGetString(GL_VERSION));
    printf("Shading Language: %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));

    return app;
}

void app_input(Application* app, Camera* cam) {
    static bool mouse = true;

    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        // [X TOP LEFT] - Quit
        if (event.type == SDL_EVENT_QUIT) app->quit = 1;

        // [MOUSE] - Camera Movement
        else if (event.type == SDL_EVENT_MOUSE_MOTION) {
            if (mouse) {
                mouse = false;
                return; 
            }
            camera_mouse(cam, event.motion.xrel, event.motion.yrel);
        }
    }

    // [ESC] - Quit
    const bool* keystate = SDL_GetKeyboardState(NULL);
    if (keystate[SDL_SCANCODE_ESCAPE]) app->quit = 1;
    
    // [T] - Toggle Simulation
    static int togglePrev = 0;
    int toggleCurr = keystate[SDL_SCANCODE_T];

    if (toggleCurr && !togglePrev) {
        app->toggle = !app->toggle;
        app->rotationLock = 1;
    }
    togglePrev = toggleCurr;

    // [R] - Restart
    static int restartPrev = 0;
    int restartCurr = keystate[SDL_SCANCODE_R];

    if (restartCurr && !restartPrev) {
        app->restart = 1;
    }
    restartPrev = restartCurr;
    
    // [W], [A], [S], [D] - Camera Movement
    camera_keys(cam, keystate);
}

int app_quit(Application* app) {
    return app->quit;
}

void app_destroy(Application* app) {
    SDL_DestroyWindow(app->window);
    SDL_Quit();
}