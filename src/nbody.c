#include <OpenCL/opencl.h>
#include <OpenGL/OpenGL.h> 
#include <math.h>
#include "util.h"
#include "nbody.h"
#include "render.h"
#include "linmath.h"


Compute compute_init(GLuint glPositionVBO, vec3* velocity, float* mass) {
    Compute comp = {0};
    cl_int err;

    // Platform
    cl_platform_id platform;
    err = clGetPlatformIDs(1, &platform, NULL);

    // Device
    cl_device_id device;
    err = clGetDeviceIDs(platform, CL_DEVICE_TYPE_GPU, 1, &device, NULL);

    // Context with OpenGL sharing
    cl_context_properties props[] = {CL_CONTEXT_PROPERTY_USE_CGL_SHAREGROUP_APPLE,
                                     (cl_context_properties)CGLGetShareGroup(CGLGetCurrentContext()), 0};
    cl_context context = clCreateContext(props, 1, &device, NULL, NULL, &err);

    // Queue
    cl_command_queue queue = clCreateCommandQueue(context, device, 0, &err);

    // Create shared OpenGL buffers
    cl_mem clPositions = clCreateFromGLBuffer(context, CL_MEM_READ_WRITE, glPositionVBO, &err);
    cl_mem clVelocity = clCreateBuffer(context, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR,
                                       sizeof(vec3) * NUM_BODIES, velocity, &err);
    cl_mem clMass = clCreateBuffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR,
                                   sizeof(float) * NUM_BODIES, mass, &err);

    // Load and build kernel
    char* source = load_file("src/kernels/nbody.cl");
    cl_program program = clCreateProgramWithSource(context, 1, (const char**)&source, NULL, &err);
    err = clBuildProgram(program, 1, &device, NULL, NULL, NULL);

    cl_kernel kernel = clCreateKernel(program, "update_positions", &err);
    free(source);

    comp.clContext = context;
    comp.clDevice = device;
    comp.queue = queue;
    comp.program = program;
    comp.kernel = kernel;
    comp.clPositions = clPositions;
    comp.clVelocity = clVelocity;
    comp.clMass = clMass;

    return comp;
}

Bodies bodies_init(void) {
    srand((unsigned int)time(NULL));

    Bodies bodies;
    bodies.points = malloc(sizeof(vec3) * NUM_BODIES);
    bodies.colors = malloc(sizeof(vec3) * NUM_BODIES);
    bodies.velocity = malloc(sizeof(vec3) * NUM_BODIES);
    bodies.mass = malloc(sizeof(float) * NUM_BODIES);

    sphere_points(bodies.points, NUM_BODIES);
    sphere_color(bodies.colors, NUM_BODIES);
    point_velocities(bodies.points, bodies.velocity, NUM_BODIES);
    point_masses(bodies.mass);

    return bodies;
}

void sphere_points(vec3* points, int bodies) {
    for (int i = 0; i < bodies; i++) {
        float n1 = (float)rand() / (float)RAND_MAX;
        float n2 = (float)rand() / (float)RAND_MAX;
        float phi = acosf(1.0f - 2.0f * n1);
        float theta = 2.0f * M_PI * n2;

        // radius = 1
        //                 x                      y                      z
        points[i] = (vec3){sinf(phi)*cosf(theta), sinf(phi)*sinf(theta), cosf(phi)};
    }
}

void sphere_color(vec3* colors, int bodies) {
    for (int i = 0; i < bodies; i++) {
        //                     r                         g                         b
        // colors[i] = (vec3){(float)rand() / RAND_MAX, (float)rand() / RAND_MAX, (float)rand() / RAND_MAX,};
        // colors[i] = (vec3){0.0f, 0.0f, 0.0f,};  // Black
        colors[i] = (vec3){0.75f, 0.75f, 0.75f,};  // Light Grey
        // colors[i] = (vec3){1.0f, 0.52f, 0.13f,};   // Orange
        // colors[i] = (vec3){0.76f, 0.15f, 0.0f,};   // Red
    }
}

void point_velocities(vec3* points, vec3* velocities, int bodies) {
    vec3 y_axis = {0.0f, 1.0f, 0.0f};
    float spin_speed = 1.5f;

    for (int i = 0; i < bodies; i++) {
        vec3 tangent = vec3_cross(y_axis, points[i]);
        tangent = vec3_norm(tangent);
        velocities[i] = vec3_scale(tangent, spin_speed);
    }
}

void point_masses(float* masses) {
    for (int i = 0; i < NUM_BODIES; i++) {
        masses[i] = 0.001f;
    }

    for (int i = 0; i < 25; i++) {
        int idx = rand() % NUM_BODIES;
        masses[idx] = 10.0f;
    }
}

void update_positions(Compute* comp) {
    clEnqueueAcquireGLObjects(comp->queue, 1, &comp->clPositions, 0, NULL, NULL);

    // Pass Positions, Velocities, and Masses to Kernel
    clSetKernelArg(comp->kernel, 0, sizeof(cl_mem), &comp->clPositions);
    clSetKernelArg(comp->kernel, 1, sizeof(cl_mem), &comp->clVelocity);
    clSetKernelArg(comp->kernel, 2, sizeof(cl_mem), &comp->clMass);

    size_t global_size = NUM_BODIES;
    clEnqueueNDRangeKernel(comp->queue, comp->kernel, 1, NULL, &global_size, NULL, 0, NULL, NULL);

    clEnqueueReleaseGLObjects(comp->queue, 1, &comp->clPositions, 0, NULL, NULL);

    clFinish(comp->queue);
}

void compute_destroy(Compute* comp) {
    clReleaseMemObject(comp->clPositions);
    clReleaseMemObject(comp->clVelocity);
    clReleaseMemObject(comp->clMass);

    clReleaseKernel(comp->kernel);
    clReleaseProgram(comp->program);
    clReleaseCommandQueue(comp->queue);
    clReleaseContext(comp->clContext);
}

void bodies_destroy(Bodies* bodies) {
    free(bodies->points);
    free(bodies->colors);
    free(bodies->velocity);
    free(bodies->mass);
}