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

int main (int argc, char **argv){

  double coefficient[4], total, x;
  char c;

  printf ("\nf(x) = a*x^3 + b*x^2 + c*x + d\n");

  for(c = 'a'; c < 'e'; c++) {
    printf ("\nEnter the value of the 'constants' %c:\n", c);
    scanf ("%lf", &coefficient[c - 'a']);
  }

  printf("\nf(x) = %lf*x^3 + %lf*x^2 + %lf*x + %lf\n", coefficient[0], coefficient[1], coefficient[2], coefficient[3]);

  printf("\nEnter the value of 'x':\n");
  scanf("%lf", &x);

  total = (coefficient[0]* x * x * x) + (coefficient[1]* x * x) + (coefficient[2]* x + coefficient[3]);

  printf("\nf(%lf) = %lf*x^3 + %lf*x^2 + %lf*x + %lf = %lf\n", x, coefficient[0], coefficient[1], coefficient[2], coefficient[3], total);

  return 0;
}
