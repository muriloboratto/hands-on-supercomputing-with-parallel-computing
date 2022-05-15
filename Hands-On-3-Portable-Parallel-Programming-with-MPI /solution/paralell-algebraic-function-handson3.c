/*
File:           paralell-algebraic-function-handson3.c
Date:           20220419
Purpose:        Parallelize the tridiagonal matrix using MPI
Author:         Murilo Boratto < muriloboratto 'at' fieb.org.br >
Usage:
HowToCompile:   mpicc paralell-algebraic-function-handson3.c -o function -fopenmp
HowToExecute:   mpirun -np 4 ./function

*/

#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

#define SIZE 10

int main (int argc, char **argv){

	double variableX, variableXRec, parts[4], fx=0, coefficient, result, value=0;
	char op[] = {'+', '-', '*'}, c;
	int numberOfProcessors, id, to, tag = 1000;		
	
	MPI_Init(&argc,&argv);
	MPI_Comm_size(MPI_COMM_WORLD,&numeroProcessadores);
	MPI_Comm_rank(MPI_COMM_WORLD, &id);
	MPI_Status status;
	
	switch(id)
	{	
		case 0: //master			
			
			parts[0]  = atoi(argv[1]); //a
            parts[1]  = atoi(argv[2]); //b
            parts[2]  = atoi(argv[3]); //c
            variableX = atoi(argv[4]); //x

			for(to = 1; to < numberOfProcessors; to++) { //coefficients
				MPI_Send(&parts[2-(to-1)], 1, MPI_DOUBLE, to, tag, MPI_COMM_WORLD);
				MPI_Send(&parts, 1, MPI_DOUBLE, to, tag, MPI_COMM_WORLD);
			}
			
			for(to = 1; to < numberOfProcessors; to++) {
				MPI_Recv(&result, 1, MPI_DOUBLE, to, tag, MPI_COMM_WORLD, &status);
				fx += result;
			}			
			printf ("f(%f) = %f\n", parts, fx);			
			break;

		default: //workers
			MPI_Recv(&coefficient, SIZE, MPI_DOUBLE, 0, tag, MPI_COMM_WORLD, &status);
			MPI_Recv(&variableXRec, 1, MPI_DOUBLE, 0, tag, MPI_COMM_WORLD, &status);

			value = coefficient;

			for(int i = 0; i < id; i++) 
				value *= variableXRec;
				
			MPI_Send(&value,1,MPI_DOUBLE,0,tag,MPI_COMM_WORLD); //Return the result

	}
	
	return 0;
}
