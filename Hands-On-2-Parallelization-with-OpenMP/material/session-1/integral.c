/*
File:           integral.c
Last changed:   20220303 11:35:00 
Purpose:        Sequential integral calculation
Author:         muriloboratto 'at' fieb.org.br
Usage:
HowToCompile:   gcc integral.c -o integral -fopenmp -lm
HowToExecute:   OMP_NUM_THREADS=${num_threads} ./integral ${steps}
Example:        OMP_NUM_THREADS=4              ./integral   2000
*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

/* f(x) function from which the integral will be calculated. */
double f(double x)
{
 return 100 * x + sin(2 * x * M_PI);
}

/* Calculates the integral of the function betweens point a and b. */
double integral(double a, double b, int n)
{
   double h, s = 0, result;
   int i;

   h = (b - a) / n;

   for (i = 0; i < n; i++)
   {
      s += f(a + h * (i + 0.5));
   }

   result = h * s;
   return result;
}

int main(int argc, char *argv[])
{
   int steps = atoi(argv[1]);
   
   result = integral(0, 1, steps);
  
   printf("%f\n", result);

   return 0;
}
