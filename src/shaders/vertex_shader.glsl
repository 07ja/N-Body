#version 410 core

layout(location=0) in vec3 points;
layout(location=1) in vec3 vertexColor;

out vec3 v_vertexColors;

uniform mat4 uModelMatrix;
uniform mat4 uViewMatrix;
uniform mat4 uProjection;

void main() {
    gl_Position = uProjection * uViewMatrix * uModelMatrix * vec4(points, 1.0f);
    gl_PointSize = 1.0;
    v_vertexColors = vertexColor;
}
