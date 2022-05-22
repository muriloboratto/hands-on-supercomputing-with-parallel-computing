/*
File:           paralell-tridiagonal-matrix-handson3.c
Date:           20220419
Purpose:        Parallelize the tridiagonal matrix using MPI
Author:         Murilo Boratto < muriloboratto 'at' fieb.org.br >
Usage:
HowToCompile:   mpicc paralell-tridiagonal-matrix-handson3.c -o tridiagonal -fopenmp
HowToExecute:   mpirun -np 4 ./tridiagonal

*/

#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

#define ORDER 4

void printMatrix (int m[][ORDER]) {
	int i, j;
	
	for (i = 0; i < ORDER; i++) {
		printf ("| ");
		for (j = 0; j < ORDER; j++) {
			printf ("%3d ", m[i][j]);
		}
		printf ("|\n");
	}
	printf ("\n");
}

int main (int argc, char **argv){

	int numberOfProcessors, id, to, tag=1000;
	int constants[] = {100, 200, 300};
	int i, j;
	int origin[ORDER][ORDER];
	int matrix[ORDER][ORDER];
		
	MPI_Init(&argc,&argv);
	MPI_Comm_size(MPI_COMM_WORLD,&numberOfProcessors);
	MPI_Comm_rank(MPI_COMM_WORLD, &id);
	MPI_Status status;
		
	switch(id)
	{	
		case 0:	/* master */	
			for (i = 0; i < ORDER; i++) {
				for (j = 0; j < ORDER; j++) {
					if (i == j)
						origin[i][j] = i + j + 1;
					else if (i == (j + 1)) {
						origin[i][j] = i + j + 1;
						origin[j][i] = origin[i][j];
					}
					else
						origin[i][j] = 0;
				}
			}
			
			printMatrix(origin);
			
			for (to = 1; to < numberOfProcessors; to++) {
				MPI_Send(&origin, ORDER * ORDER, MPI_INT, to, tag, MPI_COMM_WORLD);				
				MPI_Recv(&origin, ORDER * ORDER, MPI_INT, to, tag, MPI_COMM_WORLD, &status);	
			}		
			printMatrix(origin);
			break;

		default:/* worker process */
			MPI_Recv(&matrix, ORDER * ORDER, MPI_INT, 0, tag, MPI_COMM_WORLD, &status);
			if (id == 1) { // main diagonal
				for (i = 0; i < ORDER; i++) 
					matrix[i][i] += constants[0];
			}
			else if (id == 2) { //superdiagonal
				for (i = 0; (i + 1) < ORDER; i++) 
					matrix[i][i + 1] += constants[2];
				
			}
			else { //subdiagonal
				for (i = 0; (i + 1) < ORDER; i++) 
					matrix[i+1][i] += constants[1];
				
			}
						
			MPI_Send(&matrix, ORDER * ORDER, MPI_INT, 0, tag, MPI_COMM_WORLD); //result

	}
	
	MPI_Finalize();
	return 0;
}
