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

int main(int argc, char **argv){

  int size = atoi(argv[1]);
  int blockSize = atoi(argv[2]);
  int matrix[SIZE_MATRIX][SIZE_MATRIX];
  int i, j, row, column;

  int k[5] = {10, 20, 30, 40, 50};

  for(i = 0; i < size; i++)
  {
    for(j = 0; j < size; j++)
    {
      matrix[i][j] = 5;
      printf("%d\t", matrix[i][j]);
    }
    printf("\n");
  }

  printf("\n\n");

  omp_set_num_threads(5);

  #pragma omp parallel private(row, column)
  {
    int id = omp_get_thread_num();

    if(id == 0)
    {
      for(row = 0; row < size; row++)
        for(column = 0; column < (id+1) * blockSize; column++)
          matrix[row][column] *= k[id];
    }
    
    if(id == 1)
    {
      for(row = 0; row < size; row++)
        for(column = blockSize; column < 2 * blockSize; column++)
          matrix[row][column] *= k[id];
    }
    
    if(id == 2)
    {
      for(row = 0; row < size; row++)
        for(column = 2 * blockSize; column < 3 * blockSize; column++)
          matrix[row][column] *= k[id];
    }
    
    if(id == 3)
    {
      for(row = 0; row < size; row++)
        for(column = 3 * blockSize; column < 4 * blockSize; column++)
          matrix[row][column] *= k[id];
    }

    if(id == 4)
    {
      for(row = 0; row < size; row++)
        for(column = 4 * blockSize; column < 5 * blockSize; column++)
          matrix[row][column] *= k[id];
    }
    
  }

  for(i = 0; i < size; i++)
  {
    for(j = 0; j < size; j++)
      printf("%d\t", matrix[i][j]);
    printf("\n");
  }

  return 0;

}
