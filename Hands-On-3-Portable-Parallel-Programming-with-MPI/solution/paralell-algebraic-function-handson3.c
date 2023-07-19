/*
File:           paralell-algebraic-function-handson3.c
Date:           20220419
Purpose:        Parallelize the tridiagonal matrix using MPI
Author:         Murilo Boratto < muriloboratto 'at' fieb.org.br >
Usage:
HowToCompile:   mpicc paralell-algebraic-function-handson3.c -o function
HowToExecute:   mpirun -np 4 ./function 1 2 3 4 1000

*/

#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

#define SIZE 10

int main (int argc, char **argv){

	double variableX, variableXRec, parts[4], fx=0, coefficient, result, value=0;
	int numberOfProcessors, id, to, tag = 1000;		
	
	MPI_Init(&argc,&argv);
	MPI_Comm_size(MPI_COMM_WORLD,&numberOfProcessors);
	MPI_Comm_rank(MPI_COMM_WORLD, &id);
	MPI_Status status;
	
	switch(id)
	{	
		case 0: //master			
			
			parts[3]  = atof(argv[1]); //a
            parts[2]  = atof(argv[2]); //b
            parts[1]  = atof(argv[3]); //c
            parts[0]  = atof(argv[4]); //d
            variableX = atof(argv[5]); //x

			for(to = 1; to < numberOfProcessors; to++) { //coefficients
				MPI_Send(&parts[to], 1, MPI_DOUBLE, to, tag, MPI_COMM_WORLD);
				MPI_Send(&variableX, 1, MPI_DOUBLE, to, tag, MPI_COMM_WORLD);
			}
			
			for(to = 1; to < numberOfProcessors; to++) {
				MPI_Recv(&result, 1, MPI_DOUBLE, to, tag, MPI_COMM_WORLD, &status);
				fx += result;
			}			
			fx += parts[0];
			printf ("f(%f) = %f\n", variableX, fx);
			break;

		default: //workers
			MPI_Recv(&coefficient, SIZE, MPI_DOUBLE, 0, tag, MPI_COMM_WORLD, &status);
			MPI_Recv(&variableXRec, 1, MPI_DOUBLE, 0, tag, MPI_COMM_WORLD, &status);

			value = coefficient;
			
			int i;
			for(i = 0; i < id; i++) 
				value *= variableXRec;
				
			MPI_Send(&value,1,MPI_DOUBLE,0,tag,MPI_COMM_WORLD); //Return the result

	}

	MPI_Finalize();
	
	return 0;
}
