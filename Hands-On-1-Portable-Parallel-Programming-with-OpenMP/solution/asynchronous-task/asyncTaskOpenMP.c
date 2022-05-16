/*
File:           asyncTaskOpenMP.c
Version:        Solution
Purpose:        Implement the asynchronous tasks using API OpenMP
Author:         Murilo Boratto  <muriloboratto 'at' fieb.org.br>
Usage:
HowToCompile:   gcc asyncTaskOpenMP.c -o asyncTaskOpenMP -fopenmp
HowToExecute:   ./asyncTaskOpenMP    <size>   <blockSize>
                ./asyncTaskOpenMP      10           2
*/

#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#define SIZE_MATRIX 10

int main (int argc, char **argv){

  int size = atoi(argv[1]);
  int blockSize = atoi(argv[2]);
  int matrix[SIZE_MATRIX][SIZE_MATRIX];
  int k[5] = {10, 20, 30, 40, 50};

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

    if (id == 0)
    {
      for (int row = 0; row < size; row++)
        for (int column = 0; column < (id+1) * blockSize; column++)
          matrix[row][column] *= k[id];
    }
    
    if (id == 1)
    {
      for (int row = 0; row < size; row++)
        for (int column = blockSize; column < 2 * blockSize; column++)
          matrix[row][column] *= k[id];
    }
    
    if (id == 2)
    {
      for (int row = 0; row < size; row++)
        for (int col = 2 * blockSize; col < 3 * blockSize; col++)
          matrix[row][col] *= k[id];
    }
    
    if (id == 3)
    {
      for (int row = 0; row < size; row++)
        for (int col = 3 * blockSize; col < 4 * blockSize; col++)
          matrix[row][col] *= k[id];
    }

    if (id == 4)
    {
      for (int row = 0; row < size; row++)
        for (int col = 4 * blockSize; col < 5 * blockSize; col++)
          matrix[row][col] *= k[id];
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
