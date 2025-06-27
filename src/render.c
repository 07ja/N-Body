#include <glad/glad.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "app.h"
#include "util.h"
#include "nbody.h"
#include "render.h"
#include "linmath.h"


Render render_init(vec3* points, vec3* colors) {
    glEnable(GL_PROGRAM_POINT_SIZE);

    Render mesh = {.vao = 0, .vboPositions = 0, .vboColors = 0, .ibo = 0,
                   .offset = -3.0f, .rotation = 0.0f, .scale = 1.0f};

    glGenVertexArrays(1, &mesh.vao);
    glBindVertexArray(mesh.vao);

    // Position VBO
    glGenBuffers(1, &mesh.vboPositions);
    glBindBuffer(GL_ARRAY_BUFFER, mesh.vboPositions);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vec3) * NUM_BODIES, points, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vec3), (void*)0);

    // Color VBO
    glGenBuffers(1, &mesh.vboColors);
    glBindBuffer(GL_ARRAY_BUFFER, mesh.vboColors);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vec3) * NUM_BODIES, colors, GL_STATIC_DRAW);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(vec3), (void*)0);

    glBindVertexArray(0);

    return mesh;
}

void shader_pipeline(Application* app) {
    char* vs = load_file("./src/shaders/vertex_shader.glsl");
    char* fs = load_file("./src/shaders/fragment_shader.glsl");

    if (!vs || !fs) {
        fprintf(stderr, "Failed to load one or both shader files.\n");
        free(vs);
        free(fs);
        return;
    }

    app->pipeline = create_shader_program(vs, fs);

    free(vs);
    free(fs);
}

GLuint create_shader_program(const char* vs, const char* fs) {
    GLuint programObject = glCreateProgram();

    GLuint myVertexShader   = CompileShader(GL_VERTEX_SHADER, vs);
    GLuint myFragmentShader = CompileShader(GL_FRAGMENT_SHADER, fs);

    glAttachShader(programObject, myVertexShader);
    glAttachShader(programObject, myFragmentShader);
    glLinkProgram(programObject);

    glValidateProgram(programObject);
    return programObject;
}

GLuint CompileShader(GLenum type, const char* source) {
    GLuint shaderObject = 0;
    if (type == GL_VERTEX_SHADER) {
        shaderObject = glCreateShader(GL_VERTEX_SHADER);
    }
    else if (type == GL_FRAGMENT_SHADER){
        shaderObject = glCreateShader(GL_FRAGMENT_SHADER);
    }
    glShaderSource(shaderObject, 1, &source, NULL);
    glCompileShader(shaderObject);
    return shaderObject;
}

void render_frame(Application* app, Camera* cam, Render* mesh, Bodies* bodies) {
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);

    glViewport(0, 0, app->windowWidth, app->windowHeight);
    
    // Background Color
    glClearColor(0.15f, 0.13f, 0.16f, 1.0f);  // Dark Purple
    // glClearColor(0.15f, 0.15f, 0.22f, 1.0f);  // Navy Blue
    // glClearColor(0.17f, 0.20f, 0.18f, 1.0f);  // Dark Green
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

    glUseProgram(app->pipeline);

    if (!app->rotationLock) {
        mesh->rotation += 0.001f;
    }

    // Translation into world space
    vec3 offset = {0.0f, 0.0f, mesh->offset};
    mat4 model = mat4_translate(mat4_identity(), offset);

    // Rotation
    vec3 axis = {0.0f, 1.0f, 0.5f};
    model = mat4_rotate(model, mesh->rotation * M_PI / 180.0f, axis);

    // Scale
    vec3 scale = {1.0f, mesh->scale * 1.0f, 1.0f};
    model = mat4_scale(model, scale);

    // Tranlation
    GLint locationTranslate = glGetUniformLocation(app->pipeline, "uModelMatrix");
    if (locationTranslate >= 0) {
        glUniformMatrix4fv(locationTranslate, 1, false, &model.m[0][0]);
    }
    else {
        printf("Could not find translate location, double check vertex and fragment shaders\n");
        exit(EXIT_FAILURE);
    }

    // Camera
    float yawRad = cam->yaw * (M_PI / 180.0f);
    float pitchRad = cam->pitch * (M_PI / 180.0f);

    cam->viewDirection.x = cosf(yawRad) * cosf(pitchRad);
    cam->viewDirection.y = sinf(pitchRad);
    cam->viewDirection.z = sinf(yawRad) * cosf(pitchRad);

    float length = sqrtf(cam->viewDirection.x * cam->viewDirection.x + 
                         cam->viewDirection.y * cam->viewDirection.y + 
                         cam->viewDirection.z * cam->viewDirection.z);
    cam->viewDirection.x /= length;
    cam->viewDirection.y /= length;
    cam->viewDirection.z /= length;

    // View
    mat4 view = mat4_view(cam->eye,vec3_add(cam->eye, cam->viewDirection), cam->upVector);
    GLint locationView = glGetUniformLocation(app->pipeline, "uViewMatrix");
    if (locationView >= 0) {
        glUniformMatrix4fv(locationView, 1, false, &view.m[0][0]);
    }
    else {
        printf("Could not find view location, double check vertex and fragment shaders\n");
        exit(EXIT_FAILURE);
    }

    // Projection in Perspective
    float aspect = (float)app->windowWidth / (float)app->windowHeight;
    mat4 perspective = mat4_perspective(45.0f * M_PI / 180.0f, aspect, 0.1f, 100.0f);
    GLint locationProjection = glGetUniformLocation(app->pipeline, "uProjection");
    if (locationProjection >= 0) {
        glUniformMatrix4fv(locationProjection, 1, false, &perspective.m[0][0]);
    }
    else {
        printf("Could not find projection location, double check vertex and fragment shaders\n");
        exit(EXIT_FAILURE);
    }
    
    // Draw
    glBindVertexArray(mesh->vao);
    glDrawArrays(GL_POINTS, 0, NUM_BODIES);
    glUseProgram(0);

    SDL_GL_SwapWindow(app->window);
}
