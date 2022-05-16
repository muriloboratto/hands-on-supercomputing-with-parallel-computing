/*
File:           big_prime.c
Version:        Solution
Purpose:        Parallelize finding the biggest 128 bit prime number using openMP
Author:         Murilo Boratto - muriloboratto@uneb.br
Usage:
HowToCompile:   gcc big_prime.c -o big_prime -fopenmp -lm
HowToExecute:   OMP_NUM_THREADS=${num_threads} ./big_prime
Example:        OMP_NUM_THREADS=4              ./big_prime
*/

#include <stdio.h>
#include <math.h>
#include <limits.h>
#include <omp.h>

typedef unsigned long long big_integer;
#define BIGGEST_INTEGER ULLONG_MAX
#define PARALLEL_BOUNDARY ULLONG_MAX / 100000000000

int is_prime(big_integer n)
{
  int result;

  result = (n % 2 != 0 || n == 2);

  if (!result)
    return 0;

  big_integer i;
  big_integer sq_root = sqrt(n);
  big_integer boundary = sq_root < PARALLEL_BOUNDARY ? sq_root : PARALLEL_BOUNDARY;

  for (i = 3; result && i <= boundary; i += 2)
    result = n % i != 0;

  if (!result)
    return 0;

#pragma omp parallel firstprivate(i) reduction(&:result)
  {
    i += 2 * omp_get_thread_num();
    big_integer increment = 2 * omp_get_num_threads();

    while (result && i <= sq_root)
    {
      result = n % i != 0;
      i += increment;
    }
  }

  return result;
}

int main(int argc, char **argv)
{
  big_integer n;

  double start = omp_get_wtime();
  for (n = BIGGEST_INTEGER; !is_prime(n); n -= 2)
  {
  }
  double end = omp_get_wtime();

  printf("%f %llu\n", end - start, n);

  return 0;
}
