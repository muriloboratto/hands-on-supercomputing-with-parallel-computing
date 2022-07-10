/*
File:           lu_mpi.c
Purpose:        Parallelize LU decomposition using MPI
Author:         Murilo Boratto - muriloboratto 'at' fieb.org.br
Usage:
HowToCompile:   mpicc lu_mpi.c -o lu_mpi -O3  
HowToExecute:   mpirun -np ${number_of_processes} ./lu ${problem_size}
Example:        mpirun -np 4 ./lu_mpi 8
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mpi.h>

#define MANAGER_RANK 0
#define VERBOSE 1

int mpi_rank, mpi_size;

/* generates a A*X=B problem in which all values for X are 1 */
void generate_ab_problem(int size, double **a_matrix, double *b_array)
{
    int r, c;
    for (r = 0; r < size; r++)
    {
        int sum = 0;
        for (c = 0; c < size; c++)
        {
            int coefficient = -5 + (rand() % 11);
            sum += coefficient;
            a_matrix[r][c] = coefficient;
        }
        b_array[r] = sum;
    }
}

double **alloc_matrix(int size)
{
    double **matrix = (double **)calloc(size, sizeof(double *));
    double *matrix_values = (double *)calloc(size * size, sizeof(double));
    int i;

    for (i = 0; i < size; i++)
        matrix[i] = (matrix_values + size * i);

    return matrix;
}

double *alloc_array(int size)
{
    return (double *)calloc(size, sizeof(double));
}

void print_matrix(double **matrix, int size)
{
    int r, c;
    for (r = 0; r < size; r++)
    {
        for (c = 0; c < size; c++)
            printf("%.2f\t", matrix[r][c]);
        printf("\n");
    }
}

void print_array(double *array, int size)
{
    int i;
    for (i = 0; i < size; i++)
        printf("%.2f\t", array[i]);
    printf("\n");
}

/* performs LU decomposition in place */
void lu_decompose(double **matrix, int size)
{
    int r, c;

    for (c = 0; c < size; c++)
    {
        for (r = c + 1; r < size; r++)
        {
            int iteration_owner = r % mpi_size;
            if (mpi_rank == iteration_owner)
            {
                double factor = matrix[r][c] / matrix[c][c];
                matrix[r][c] = factor;

                int cc;
                for (cc = c + 1; cc < size; cc++)
                    matrix[r][cc] -= matrix[c][cc] * factor;
            }

            MPI_Bcast(&matrix[r][c], size - c, MPI_DOUBLE, iteration_owner, MPI_COMM_WORLD);
        }
    }
}

/* solves L*Y=B for Y using progressive elimination */
void solve_lyb(double **l_matrix, int size, double *y_array, double *b_array)
{
    int r, c;

    y_array[0] = b_array[0];
    for (r = 1; r < size; r++)
    {
        for (c = mpi_rank; c < r; c += mpi_size)
            y_array[r] -= l_matrix[r][c] * y_array[c];

        MPI_Allreduce(&y_array[r], &y_array[r], 1, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);

        y_array[r] += b_array[r];
    }
}

/* solves the U*X=Y for X using regressive substitution */
void solve_uxy(double **u_matrix, int size, double *x_array, double *y_array)
{
    int r, c;

    x_array[size - 1] = y_array[size - 1] / u_matrix[size - 1][size - 1];
    for (r = size - 2; r >= 0; r--)
    {
        for (c = size - 1 - mpi_rank; c > r; c -= mpi_size)
            x_array[r] -= u_matrix[r][c] * x_array[c];

        MPI_Allreduce(&x_array[r], &x_array[r], 1, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);

        x_array[r] += y_array[r];
        x_array[r] /= u_matrix[r][r];
    }
}

int main(int argc, char **argv)
{
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &mpi_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &mpi_size);

    int size = atoi(argv[1]);

    double **A = alloc_matrix(size);
    double *B = alloc_array(size);
    double *Y = alloc_array(size);
    double *X = alloc_array(size);

    if (mpi_rank == MANAGER_RANK)
        generate_ab_problem(size, A, B);

    MPI_Bcast(A[0], size * size, MPI_DOUBLE, MANAGER_RANK, MPI_COMM_WORLD);
    MPI_Bcast(B, size, MPI_DOUBLE, MANAGER_RANK, MPI_COMM_WORLD);

    double start = MPI_Wtime();

    lu_decompose(A, size);
    solve_lyb(A, size, Y, B);
    solve_uxy(A, size, X, Y);

    double end = MPI_Wtime();

#if VERBOSE
    if (mpi_rank == MANAGER_RANK)
    {
        printf("A\n");
        print_matrix(A, size);
        printf("B\n");
        print_array(B, size);
        printf("L\\U\n");
        print_matrix(A, size);
        printf("Y\n");
        print_array(Y, size);
        printf("X\n");
        print_array(X, size);
    }
#endif

    if (mpi_rank == MANAGER_RANK)
        printf("%d %f\n", size, end - start);

    MPI_Finalize();
}