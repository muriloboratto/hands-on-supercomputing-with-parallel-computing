/*
File:           hello-openmp+mpi.c
Purpose:        Hello World using OpenMP+MPI
Author :        Murilo Boratto <muriloboratto 'at' fieb.org.br>
Usage:
HowToCompile:   mpicc hello-mpi+openmp.c -o hello-mpi+openmp -fopenmp
HowToExecute:   mpirun -np <numberOfProcesses> ./hello-mpi+openmp
                mpirun -np 4 ./hello-mpi+openmp
*/

#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

int main( int argc, char *argv[])
{
    int nthreads,nprocs,idpro,idthr;
    int  namelen;
    char processor_name[MPI_MAX_PROCESSOR_NAME];

    MPI_Init(&argc,&argv);
    MPI_Comm_size(MPI_COMM_WORLD,&nprocs);
    MPI_Comm_rank(MPI_COMM_WORLD,&idpro);
    MPI_Get_processor_name(processor_name,&namelen);

    #pragma omp parallel private(idthr) firstprivate(idpro,processor_name)
    {
        idthr = omp_get_thread_num();
        printf("Hello World  thread %d, From %d processor %s\n",idthr,idpro,processor_name);
        if (idthr == 0) 
        {
            nthreads = omp_get_num_threads();
            printf("Master processor %d, number of threads %d, number of processor %d\n",idpro,
                                                                                         nthreads,
                                                                                         nprocs);
        }

    }

    MPI_Finalize();
}
