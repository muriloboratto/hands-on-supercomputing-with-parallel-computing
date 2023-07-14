/*
File:           asyncTaskOpenMP.c
Purpose:        Implement the asynchronous tasks using API OpenMP
Author:         Murilo Boratto  <muriloboratto 'at' fieb.org.br>
Usage:
HowToCompile:   gcc asyncTaskOpenMP.c -o asyncTaskOpenMP -fopenmp
HowToExecute:   ./asyncTaskOpenMP    <size>   <blockSize>
Example:        ./asyncTaskOpenMP      10           2
*/

#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#define SIZE_MATRIX 10

int main(int argc, char **argv)
{
  int n = atoi(argv[1]);
  int block_size = atoi(argv[2]);
  int matrix[SIZE_MATRIX][SIZE_MATRIX], k1 = 10, k2 = 20;
  int i, j, row, column;

  for(i = 0; i < n; i++)
  {
    for(j = 0; j < n; j++)
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
      for(row = 0; row < n; row++)
        for(column = 0; column < block_size; column++)
          matrix[row][column] *= k1;
    }

    if(id == 1)
    {
      for(row = 0; row < n; row++)
        for(column = block_size; column < 2 * block_size; column++)
          matrix[row][column] *= k2;
    }
  
  }/*pragma omp*/

  for(i = 0; i < n; i++)
  {
    for(j = 0; j < n; j++)
      printf("%d\t", matrix[i][j]);
    printf("\n");
  }

  return 0;
}
