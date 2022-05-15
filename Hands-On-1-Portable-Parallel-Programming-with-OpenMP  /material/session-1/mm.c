/*
File:           hands-on-1/session-1/mm.c
Version:        1
Purpose:        Parallelize matrix multiplication using openMP
Author:         Murilo Boratto - muriloboratto@uneb.br
Usage:
HowToCompile:   gcc -O3 -o mm -fopenmp mm.c
HowToExecute:   OMP_NUM_THREADS=${num_threads} ./mm ${matrix_size}
Example:        OMP_NUM_THREADS=4 ./sudoku-task 10
*/

/*
@(#)File:           $mm.c$
@(#)Last changed:   $Date: 20220303 11:35:00 $
@(#)Purpose:        Matrix Multiply Sequential Algorithm in C
@(#)Author:         muriloboratto 'at' fieb.org.br
@(#)Usage:         
@(*) Hotocompile:   gcc mm.c -o mm -fopenmp
@(*) Hotoexecute:   ./mm <size>
                    ./mm  100
*/


#include <stdio.h>
#include <stdlib.h>

void initialize_matrix(int *matrix, int size)
{
  for (int i = 0; i < size; i++)
    for (int j = 0; j < size; j++)
      matrix[i * size + j] = rand() % (10 - 1) * 1;
}

void print_matrix(int *matrix, int size)
{
  for (int i = 0; i < size; i++)
  {
    for (int j = 0; j < size; j++)
      printf("%d\t", matrix[i * size + j]);
    printf("\n");
  }
  printf("\n");
}

int main(int argc, char **argv)
{
  int size = atoi(argv[1]);

  int *A = (int *)malloc(sizeof(int) * size * size);
  int *B = (int *)malloc(sizeof(int) * size * size);
  int *C = (int *)malloc(sizeof(int) * size * size);

  initialize_matrix(A, size);
  initialize_matrix(B, size);

  for (int i = 0; i < size; i++)
    for (int j = 0; j < size; j++)
      for (int k = 0; k < size; k++)
        C[i * size + j] += A[i * size + k] * B[k * size + j];

  print_matrix(A, size);
  print_matrix(B, size);
  print_matrix(C, size);

  return 0;
}
