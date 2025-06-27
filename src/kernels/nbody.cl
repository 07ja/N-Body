__kernel void update_positions(__global float4* positions, __global float4* velocities, __global float* masses) {
    int i = get_global_id(0);

    float4 p = positions[i];
    float4 v = velocities[i];

    float4 acc = (float4)0.0f;
    float dt = 0.00025f;
    float soft = 1.25f;

    for (int j = 0; j < get_global_size(0); j++) {
        if (i == j) continue;

        float4 pj = positions[j];
        float mj = masses[j];

        float4 radius = pj - p;
        float distSqr = dot(radius.xyz, radius.xyz) + (soft * soft);

        float invDist = native_rsqrt(distSqr);
        float invDist3 = invDist * invDist * invDist;

        acc += radius * (mj * invDist3);
    }

    v += acc * dt;
    p += v * dt;

    positions[i] = p;
    velocities[i] = v;
}
