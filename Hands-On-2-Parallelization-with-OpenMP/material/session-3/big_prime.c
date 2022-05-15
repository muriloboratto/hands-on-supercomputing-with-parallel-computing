/*
File:           hands-on-2/session-3/big_prime.c
Version:        1
Purpose:        Parallelize finding the biggest 128 bit prime number using openMP
Author:         Murilo Boratto - muriloboratto@uneb.br
Usage:
HowToCompile:   gcc big_prime.c -o big -fopenmp -lm
HowToExecute:   OMP_NUM_THREADS=${num_threads} ./big
Example:        OMP_NUM_THREADS=4 ./count
*/

#include <stdio.h>
#include <math.h>
#include <limits.h>

typedef unsigned long long big_integer;
#define BIGGEST_INTEGER ULLONG_MAX

int is_prime(big_integer n)
{
  int result;
  big_integer sq_root;

  result = (n % 2 != 0 || n == 2);

  if (result)
  {
    sq_root = sqrt(n);

    for (big_integer i = 3; result && i <= sq_root; i += 2)
      result = n % i != 0;
  }

  return result;
}

int main()
{
  big_integer n;

  for (n = BIGGEST_INTEGER; !is_prime(n); n -= 2)
  {
  }

  printf("%llu\n", n);

  return 0;
}
