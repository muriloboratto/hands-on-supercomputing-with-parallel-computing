/*
@(#)File:           $sequential-tridiagonal-matrix-handson3.c$
@(#)Last changed:   $Date: 20220407 11:35:00 $
@(#)Purpose:        Add a constant value in a tridiagonal matrix in C
@(#)Author:         muriloboratto 'at' fieb.org.br
@(#)Usage:
@(*) Hotocompile:   gcc sequential-tridiagonal-matrix-handson3.c -o obj
@(*) Hotoexecute:   ./obj
*/

#include <stdio.h>
#define ORDER 4

void printMatrix (int m[][ORDER]) {
  int i, j;
  for(i = 0; i < ORDER; i++) {
    printf ("| ");
    for (j = 0; j < ORDER; j++) {
      printf ("%3d ", m[i][j]);
    }
    printf ("|\n");
  }
  printf ("\n");
}

int main (int argc, char **argv){

  int k[3] = {100, 200, 300};
  int matrix[ORDER][ORDER];
  int i, j;

  for(i = 0; i < ORDER; i++) {
    for(j = 0; j < ORDER; j++) {
      if( i == j )
        matrix[i][j] = i + j +1;
      else if(i == (j + 1)) {
        matrix[i][j] = i +  j + 1;
        matrix[j][i] = matrix[i][j];
      } else
           matrix[i][j] = 0;
     }
  }

  printMatrix(matrix);

  for(i = 0; i < ORDER; i++){
       matrix[i][i]     += k[0];  //main diagonal
     matrix[i + 1][i] += k[1];    //subdiagonal
     matrix[i][i + 1] += k[2];    //superdiagonal
  }

  printMatrix(matrix);

  return 0;
}
