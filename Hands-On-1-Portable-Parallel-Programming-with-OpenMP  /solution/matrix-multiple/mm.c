/*
@(#)File:           $mm.c$
@(#)Last changed:   $Date: 20220303 11:35:00 $
@(#)Purpose:        Matrix Multiply Algorithm in API OpenMP
@(#)Author:         muriloboratto 'at' fieb.org.br
@(#)Usage:         
@(*) Hotocompile:   gcc mm.c -o mm -fopenmp
@(*) Hotoexecute:   OMP_NUM_THREADS=<threads> ./mm <size>
                    OMP_NUM_THREADS=4         ./mm  100
*/

#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

void initializeMatrix(int *A, int n){
  int i,j;
 
  for(i = 0; i < n; i++)
    for(j = 0; j < n; j++)
      A[i * n + j] = rand()%(10-1)*1;
 
}

void printMatrix(int *A, int n){
  int i,j;

  for(i = 0; i < n; i++){
    for(j = 0; j < n; j++)
      printf("%d\t", A[i * n + j]);
    
    printf("\n");
  }

 printf("\n");

}

int main (int argc, char **argv){

int n = atoi(argv[1]);  
int i, j, k;
double t1, t2;

int  *A = (int *) malloc (sizeof(int)*n*n);
int  *B = (int *) malloc (sizeof(int)*n*n);
int  *C = (int *) malloc (sizeof(int)*n*n);

initializeMatrix(A,n);
initializeMatrix(B,n);

t1 = omp_get_wtime();

#pragma omp parallel for private(i, j, k)
  for(i = 0; i < n; i++) 
    for(j = 0; j < n; j++)
       for(k = 0; k < n; k++) 
          C[i*n+j]+=A[i*n+k]*B[k*n+j];

t2 = omp_get_wtime();

printf("%d\t%f\n",n, t2-t1);

// printMatrix(A,n);
// printMatrix(B,n);
// printMatrix(C,n);

return 0;

}

