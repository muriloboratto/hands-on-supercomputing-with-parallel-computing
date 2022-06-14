#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "timer.h"
#include "files.h"

#define SOFTENING 1e-9f

typedef struct { float x, y, z, vx, vy, vz; } Body;

__global__
void bodyForce(Body *p, float dt, int n) {
  int i = blockDim.x * blockIdx.x + threadIdx.x;
  if (i < n) {
    float Fx = 0.0f; float Fy = 0.0f; float Fz = 0.0f;

    for (int j = 0; j < n; j++) {
      float dx = p[j].x - p[i].x;
      float dy = p[j].y - p[i].y;
      float dz = p[j].z - p[i].z;
      float distSqr = dx*dx + dy*dy + dz*dz + SOFTENING;
      float invDist = rsqrtf(distSqr);
      float invDist3 = invDist * invDist * invDist;

      Fx += dx * invDist3; Fy += dy * invDist3; Fz += dz * invDist3;
    }

    p[i].vx += dt*Fx; p[i].vy += dt*Fy; p[i].vz += dt*Fz;
  }
}


int main(const int argc, const char** argv) {

  int nBodies = 2<<11;
  if (argc > 1) nBodies = 2<<atoi(argv[1]);

  const char * initialized_values;
  const char * solution_values;

  if (nBodies == 2<<11) {
    initialized_values = "09-nbody/files/initialized_4096";
    solution_values = "09-nbody/files/solution_4096";
  } else { // nBodies == 2<<15
    initialized_values = "09-nbody/files/initialized_65536";
    solution_values = "09-nbody/files/solution_65536";
  }

  if (argc > 2) initialized_values = argv[2];
  if (argc > 3) solution_values = argv[3];

  const float dt = 0.01f; // Time step
  const int nIters = 10;  // Simulation iterations

  int bytes = nBodies * sizeof(Body);
  float *buf;

  buf = (float *)malloc(bytes);
  Body *p = (Body*)buf;

  read_values_from_file(initialized_values, buf, bytes);

  double totalTime = 0.0;

// CHANGE ===========================

  int size_in_bytes = sizeof(Body) * nBodies;
 
  float *device_buf;
  cudaMalloc((void**) &device_buf, size_in_bytes);
  
  Body *d_p = (Body*)device_buf;
    
  int deviceId;
  cudaGetDevice(&deviceId);
  cudaDeviceProp props;
  cudaGetDeviceProperties(&props, deviceId);
  int multiProcessorCount = props.multiProcessorCount;
   
  size_t threadsPerBlock = 256;
  size_t numberOfBlocks = 32 * multiProcessorCount;

// CHANGE ===========================


  for (int iter = 0; iter < nIters; iter++) {
    StartTimer();

    cudaMemcpy(device_buf, buf, size_in_bytes, cudaMemcpyHostToDevice);
       bodyForce<<< numberOfBlocks, threadsPerBlock>>>(d_p, dt, nBodies); // compute interbody forces
    cudaMemcpy(buf, device_buf, size_in_bytes, cudaMemcpyDeviceToHost);
  
    for (int i = 0 ; i < nBodies; i++) { // integrate position
      p[i].x += p[i].vx*dt;
      p[i].y += p[i].vy*dt;
      p[i].z += p[i].vz*dt;
    }

    const double tElapsed = GetTimer() / 1000.0;
    totalTime += tElapsed;
  }

  double avgTime = totalTime / (double)(nIters);
  float billionsOfOpsPerSecond = 1e-9 * nBodies * nBodies / avgTime;
  write_values_to_file(solution_values, buf, bytes);

  printf("%0.3f Billion Interactions / second\n", billionsOfOpsPerSecond);

  free(buf);
  cudaFree(device_buf);
}
