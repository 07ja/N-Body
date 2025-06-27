#ifndef NBODY_H
#define NBODY_H

#include <OpenCL/opencl.h>
#include "linmath.h"

#define NUM_BODIES 10000

typedef struct {
    cl_context clContext;
    cl_device_id clDevice;
    cl_command_queue queue;
    cl_program program;
    cl_kernel kernel;
    cl_mem clPositions;
    cl_mem clVelocity;
    cl_mem clMass;
} Compute;

typedef struct {
    vec3* points;
    vec3* colors;
    vec3* velocity;
    float* mass;
} Bodies;

Bodies bodies_init(void);
Compute compute_init(GLuint glPositionVBO, vec3* velocity, float* mass);
void sphere_points(vec3* points, int bodies);
void sphere_color(vec3* colors, int bodies);
void point_velocities(vec3* points, vec3* velocities, int bodies);
void point_masses(float* masses);
void update_positions(Compute* comp);
void compute_destroy(Compute* compute);
void bodies_destroy(Bodies* bodies);

#endif