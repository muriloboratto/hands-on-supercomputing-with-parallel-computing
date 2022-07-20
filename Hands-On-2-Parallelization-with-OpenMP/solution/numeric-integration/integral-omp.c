/*
File:           integral.c
Version:        Solution
Purpose:        Parallelize integral calculation using OpenMP
Author:         Murilo Boratto  <muriloboratto 'at' fieb.org.br>
Usage:
HowToCompile:   gcc integral.c -o integral -fopenmp -lm
HowToExecute:   OMP_NUM_THREADS=${num_threads} ./integral ${steps}
Example:        OMP_NUM_THREADS=4              ./integral   2000
*/


#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <omp.h>

/* f(x) function from which the integral will be calculated. */
double f(double x)
{
   return 100 * x + sin(2 * x * M_PI);
}

/* Calculates the integral of the function betweens point a and b. */
double calculate_integral_1(double a, double b, int n)
{
   double h, s = 0, result;
   int i;

   h = (b - a) / n;

   #pragma omp parallel for reduction(+:s)
   for (i = 0; i < n; i++)
   {
      s += f(a + h * (i + 0.5));
   }

   result = h * s;
   return result;
}

int main(int argc, char *argv[])
{
   double a = 0, b = 1, result, start, end;
   int steps, variant;

   if (argc < 1)
   {
      printf("Incorrect number of arguments!\n");
      return 1;
   }
   if (argc > 1)
      steps = atoi(argv[2]);

   variant = 1;

   switch (variant)
   {
   case 1:
      start = omp_get_wtime();
      result = calculate_integral_1(a, b, steps);
      end = omp_get_wtime();
      break;
   default:
      printf("Incorrect variant number!\n");
      return 1;
   }

   printf("%f\t%f\n", end - start, result);

   return 0;
}
