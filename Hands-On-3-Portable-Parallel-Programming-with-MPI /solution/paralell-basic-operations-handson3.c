/*
File:           paralell-basic-operations-handson3.c
Date:           20220419
Purpose:        Parallelize the basic operations using MPI
Author:         Murilo Boratto < muriloboratto 'at' fieb.org.br >
Usage:
HowToCompile:   mpicc paralell-basic-operations-handson3.c -o basic -fopenmp
HowToExecute:   mpirun -np 4 ./basic

*/

#include <stdio.h>
#include <mpi.h>
#define SIZE 10

int main (int argc, char **argv){
	
	/*Variables*/
	int  array[SIZE];
	char operations[] = {'+', '-', '*'};
	char operationsRec;
	int  numberOfProcessors, id, to, from, tag = 1000;
	int  i, result, value=0;
	
	/*Iniatilize MPI*/
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &numberOfProcessors);
	MPI_Comm_rank(MPI_COMM_WORLD, &id);
	MPI_Status status;
	
	switch(id)
	{	
		case 0:	/*Master*/

			for(i = 0; i < SIZE; i++){
			  array[i] = i + 1;
			  printf("%d\t", i, array[i]);
			}
			printf("\n");

			for(to = 1; to < numberOfProcessors; to++) {
				MPI_Send(&array, SIZE, MPI_INT, to, tag, MPI_COMM_WORLD);
				MPI_Send(&operations[to-1], 1, MPI_CHAR, to, tag, MPI_COMM_WORLD);	
			}
			for (to = 1; to < numberOfProcessors; to++) {
				MPI_Recv(&result, 1, MPI_INT, to, tag, MPI_COMM_WORLD, &status);
				MPI_Recv(&operationsRec, 1, MPI_CHAR, to, tag, MPI_COMM_WORLD, &status);
				printf ("(%c) = %d\n", operationsRec, result);
			}			
			break;

		default: /*Workers*/	
			MPI_Recv(&array, SIZE, MPI_INT, 0, tag, MPI_COMM_WORLD, &status);
			MPI_Recv(&operationsRec, 1, MPI_CHAR, 0, tag, MPI_COMM_WORLD, &status);
			
			switch (operationsRec) {
				case '+':	
					value = 0;
					for(i = 0; i < SIZE; i++)
						value += array[i];
					break;

				case '-':
					value = 0;
					for(i = 0; i < SIZE; i++)
						value -= array[i];
					break;
				
				case '*':
					value = 1;
					for(i = 0; i < SIZE; i++)
						value *= array[i];
					break;
			}
			
			/*Return to the Master*/
			MPI_Send(&value, 1, MPI_INT, 0, tag, MPI_COMM_WORLD);
			MPI_Send(&operationsRec, 1, MPI_CHAR, 0, tag, MPI_COMM_WORLD);
	}

	return 0;
}
