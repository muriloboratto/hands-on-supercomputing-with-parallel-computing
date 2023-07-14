/*
File:           hands-on-5/lu.c
Version:        1
Purpose:        Parallelize LU decomposition using MPI
Author:         Murilo Boratto - muriloboratto@uneb.br
Usage:
HowToCompile:   gcc -o lu lu.c
HowToExecute:   ./lu ${problem_size}
Example:        ./lu 8
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* generates a A*X=B problem in which all value for X are 1 */
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
            double factor = matrix[r][c] / matrix[c][c];
            matrix[r][c] = factor;

            int cc;
            for (cc = c + 1; cc < size; cc++)
                matrix[r][cc] -= matrix[c][cc] * factor;
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
        y_array[r] = b_array[r];
        for (c = 0; c < r; c++)
            y_array[r] -= l_matrix[r][c] * y_array[c];
    }
}

/* solves the U*X=Y for X using regressive substitution */
void solve_uxy(double **u_matrix, int size, double *x_array, double *y_array)
{
    int r, c;

    x_array[size - 1] = y_array[size - 1] / u_matrix[size - 1][size - 1];
    for (r = size - 2; r >= 0; r--)
    {
        for (c = size - 1; c > r; c--)
            x_array[r] -= u_matrix[r][c] * x_array[c];

        x_array[r] += y_array[r];
        x_array[r] /= u_matrix[r][r];
    }
}

int main(int argc, char **argv)
{
    int size = atoi(argv[1]);

    double **A = alloc_matrix(size);
    double *B = alloc_array(size);
    double *Y = alloc_array(size);
    double *X = alloc_array(size);

    generate_ab_problem(size, A, B);

    printf("A\n");
    print_matrix(A, size);
    printf("B\n");
    print_array(B, size);

    lu_decompose(A, size); // A now contains both L and U
    solve_lyb(A, size, Y, B);
    solve_uxy(A, size, X, Y);

    printf("L\\U\n");
    print_matrix(A, size);
    printf("Y\n");
    print_array(Y, size);
    printf("X\n");
    print_array(X, size);

    return 0;
}