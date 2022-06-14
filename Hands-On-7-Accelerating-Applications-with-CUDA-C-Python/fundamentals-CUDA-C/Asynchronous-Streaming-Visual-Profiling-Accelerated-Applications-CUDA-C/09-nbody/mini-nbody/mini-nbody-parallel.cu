/*
@(#)File:           $mini-nbody-parallel.cu$
@(#)Version:        $Parallel version N-Body Simulation$
@(#)Last changed:   $Date: 20220319 11:13:00 $
@(#)Purpose:        Fast N-Body Simulation
@(#)Author:         Murilo Boratto - muriloboratto@gmail.com
@(#)Usage:         
 (*) Hotocompile:   nvcc mini-nbody-parallel.cu -o mini-nbody-parallel -Xcompiler -fopenmp
 (*) Hotoexecute:   ./mini-nbody-parallel <size nBodies>
@(#)Comment:  
    (*) [1] https://developer.nvidia.com/gpugems/gpugems3/part-v-physics-simulation/chapter-31-fast-n-body-simulation-cuda            
    (*) [2] https://github.com/harrism/mini-nbody/blob/master/nbody.c
*/

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#define BLOCK_SIZE 256
#define SOFTENING 1e-9f

typedef struct { float x, y, z, vx, vy, vz; } Body;

void randomizeBodies(float *data, int n) {
  for (int i = 0; i < n; i++) {
    data[i] = 2.0f * (rand() / (float)RAND_MAX) - 1.0f;
  }
}

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
  
  int nBodies = 30000;
  
  if (argc > 1) 
    nBodies = atoi(argv[1]);
  
  const float dt = 0.01f; // time step
  const int nIters = 10;  // simulation iterations
  int bytes = nBodies*sizeof(Body);
  float *buf = (float*)malloc(bytes);
  Body *p = (Body*)buf;

  randomizeBodies(buf, 6*nBodies); // Init pos / vel data

  float *d_buf;
  cudaMalloc(&d_buf, bytes);
  Body *d_p = (Body*)d_buf;
  int nBlocks = (nBodies + BLOCK_SIZE - 1) / BLOCK_SIZE;
  double totalTime = 0.0; 

  const double t3 = omp_get_wtime();

  for (int iter = 1; iter <= nIters; iter++) {
    const double t1 = omp_get_wtime();

    cudaMemcpy(d_buf, buf, bytes, cudaMemcpyHostToDevice);
          bodyForce<<<nBlocks, BLOCK_SIZE>>>(d_p, dt, nBodies); // compute interbody forces
    cudaMemcpy(buf, d_buf, bytes, cudaMemcpyDeviceToHost);

    for (int i = 0 ; i < nBodies; i++) { // integrate position
      p[i].x += p[i].vx*dt;
      p[i].y += p[i].vy*dt;
      p[i].z += p[i].vz*dt;
    }

    const double t2 = omp_get_wtime();
    totalTime += (t2-t1); 
    //printf("Iteration %d: %.3f seconds\n", iter, totalTime);
  }
  const double t4 = omp_get_wtime();

  double avgTime = (t4-t3) / (double)(nIters-1); 
  float billionsOfOpsPerSecond = 1e-9 * nBodies * nBodies / avgTime;
  printf("\n Size (Bodies) = %d\n", nBodies);
  printf("%0.3f Billion Interactions / second\n", billionsOfOpsPerSecond);
  printf("%0.3f second\n", avgTime);
  
  free(buf);
  cudaFree(d_buf);

  return 0;
}