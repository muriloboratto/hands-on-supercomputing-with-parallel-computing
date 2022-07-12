/****************************************************************************80
*  Code: 
*   saxpy-cudaMallocManaged.cu
*
*  Purpose:
*   Implements in C using CUDA the simple SAXPY stands for Single-Precision.
*   Using the unified memory (cudaMallocManaged).
*
*  Modified:
*   May 08 2022 17:28 
*
*  Author:
*    Murilo Boratto  < muriloboratto 'at' fieb.org.br >
*
*  HowtoCompile:
*    nvcc saxpy-cudaMallocManaged.cu -o saxpy 
*   
*  HowtoExecute:
*    ./saxpy-cudaMallocManaged <size> 
*    ./saxpy-cudaMallocManaged  10
*
*******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <cuda.h>


__global__ void saxpy(int n,  float *x, float *y){

 int i = threadIdx.x;

 if(i < n)
   y[i] = x[i] + y[i];

}


void printVector(float *vector, int n){

for (int i=0; i < n ; ++i)
 printf("%1.0f\t", vector[i]);

 printf("\n\n");

}


void generateVector(float *vector, int n){

for (int i=0; i < n ; ++i)
 vector[i] = i + 1;

}


int main(int argc, char *argv[]){

  int n = atoi(argv[1]);   
  float *x,*y;

  cudaMallocManaged(&x, sizeof(float) * n);
  cudaMallocManaged(&y, sizeof(float) * n);
 
  generateVector(x, n);
  printVector(x, n);

  generateVector(y, n);
  printVector(y, n);
  
	saxpy <<< 1, n >>> (n, x, y);
  
  cudaDeviceSynchronize();

  printVector(y, n);
 
  cudaFree(x);
  cudaFree(y);

  return 0;

}

