/*
@(#)File:           $sequential-algebraic-function-handson3.c$
@(#)Last changed:   $Date: 20220407 11:35:00 $
@(#)Purpose:        Algebraic value applied in a function in C
@(#)Author:         muriloboratto 'at' fieb.org.br
@(#)Usage:
@(*) Hotocompile:   gcc sequential-algebraic-function-handson3.c -o obj
@(*) Hotoexecute:   ./obj
*/

#include <stdio.h>

int main (int argc, char **argv)
{
  double coef[4], total, x;   
  char c;

  printf ("\nf(x) = a*x^3 + b*x^2 + c*x + d\n");          
            
  coef[0] = 1;
  coef[1] = 2;  
  coef[2] = 3;
  coef[3] = 4;
  
  printf("\nf(x)=%lf*x^3+%lf*x^2+%lf*x+%lf\n", coef[0], coef[1], coef[2], coef[3]);

  x = 10;

  total = (coef[0]* x * x * x) + (coef[1]* x * x) + (coef[2]* x + coef[3]); 
    
  printf("\nf(%lf) = %lf*x^3 + %lf*x^2 + %lf*x + %lf = %lf\n", x, coef[0], coef[1], coef[2], coef[3], total);
    
  return 0;
    
}