#ifndef RENDER_H
#define RENDER_H

#include <glad/glad.h>
#include "app.h"
#include "camera.h"

typedef struct {
    GLuint vao;
    GLuint vboPositions;
    GLuint vboColors;
    GLuint ibo;
    float offset;
    float rotation;
    float scale;
} Render;

Render render_init(vec3* points, vec3* colors);
void shader_pipeline(Application* app);
GLuint create_shader_program(const char* vs, const char* fs);
GLuint CompileShader(GLenum type, const char* source);
void render_frame(Application* app, Camera* cam, Render* mesh, Bodies* bodies);

#endif