/*
@(#)File:           $sequential-basic-operations-handson3.c$
@(#)Last changed:   $Date: 20220407 11:35:00 $
@(#)Purpose:        Basic operations in a sequential algorithm in C
@(#)Author:         muriloboratto 'at' fieb.org.br
@(#)Usage:         
@(*) Hotocompile:   gcc sequential-basic-operations-handson3.c -o obj
@(*) Hotoexecute:   ./obj 
*/

#include <stdio.h>

#define SIZE 12

int main (int argc, char **argv){

  int i, sum = 0, subtraction = 0, mult = 1;
  int array[SIZE];

  for(i = 0; i < SIZE; i++)
    array[i] = i + 1;
  
  for(i = 0; i < SIZE; i++)
    printf("array[%d] = %d\n", x, array[x]);

  for(i = 0; i < SIZE; i++) {
    sum = sum + array[i];
    subtraction = subtraction - array[i];
    mult = mult * array[i];
  }

  printf("Sum = %d\n", sum);
  printf("Subtraction = %d\n", subtraction);
  printf("Multiply = %d\n", mult);

  return 0;

}