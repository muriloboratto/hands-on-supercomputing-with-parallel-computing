/*
File:           asyncTaskOpenMPOptimization.c
Version:        Solution
Purpose:        Implement the asynchronous tasks using API OpenMP with Algorithm Optimization
Author:         Murilo Boratto  <muriloboratto 'at' fieb.org.br>
Usage:
HowToCompile:   gcc asyncTaskOpenMPOptimization.c -o asyncTaskOpenMPOptimization -fopenmp
HowToExecute:   ./asyncTaskOpenMPOptimization    <size>   <blockSize>
                ./asyncTaskOpenMPOptimization      10           2
*/

#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#define SIZE_MATRIX 10

int main (int argc, char **argv){

  int size = atoi(argv[1]);
  int blockSize = atoi(argv[2]);
  int matrix[SIZE_MATRIX][SIZE_MATRIX];
  int k[5] = {1, 2, 3, 4, 5};

  for (int i = 0; i < size; i++)
  {
    for (int j = 0; j < size; j++)
    {
      matrix[i][j] = 5;
      printf("%d\t", matrix[i][j]);
    }
    printf("\n");
  }

  printf("\n\n");

  omp_set_num_threads(5);

  #pragma omp parallel
  {
    int id = omp_get_thread_num();

    if(id)
    {
      for (int row = 0; row < size; row++)
        for (int column = id * blockSize; column < ( id + 1 ) * blockSize; column++)
          matrix[row][column] *= k[id];
    }
     
  }

  for (int i = 0; i < size; i++)
  {
    for (int j = 0; j < size; j++)
      printf("%d\t", matrix[i][j]);
    printf("\n");
  }

  return 0;

}
