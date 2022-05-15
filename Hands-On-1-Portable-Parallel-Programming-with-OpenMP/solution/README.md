# Portable Parallel Programming with OpenMP  

This repository contains OpenMP-examples which I created while learning OpenMP. The aim of this benchmark is learn how to analyse modules for C using the APIs OpenMP. A practical example to see how it can be used and to see a real example of the speed gains. The results are impressive for the effort and performance on the supercomputacional environment.

## Repository

| Name             | Description                                                      |
|------------------|------------------------------------------------------------------|
| mm.c             | Matrix multiply C code in OpenMP                                 |
| START.sh         | Shell script that measures execution time and speedup parameters |
| asyncTaskOpenMP.c| Asynchronous tasks using in OpenMP                               |

----
## Command Line Arguments
### Matrix Multiple

~$ bash START.sh 

~~~c++

#pragma omp parallel for private(i,j,k)
for(i = 0; i < n; i++) 
  for(j = 0; j < n; j++)
    for(k = 0; k < n; k++) 
       C[i * n + j] += A[i * n + k] * B[k * n + j];
			
}
~~~


### Asynchronous Tasks

~$ ./asyncTaskOpenMP  10  2 

## OpenMP

~~~c++

  #pragma omp parallel
  {
    int id = omp_get_thread_num();

    if (id)
    {
      for (int row = 0; row < size; row++)
        for (int column = (id)*blockSize; column < (id+1)*blockSize; column++)
          matrix[row][column] *= k[id];
    }
  }  
~~~

----

## Asynchronous Material

[Introduction to OpenMP](https://www.youtube.com/watch?v=nE-xN4Bf8XI&list=PLLX-Q6B8xqZ8n8bwjGdzBJ25X2utwnoEG) 

[Guide into OpenMP](https://bisqwit.iki.fi/story/howto/openmp/) 

[OpenMP Traps For C++ Developers](https://pvs-studio.com/en/blog/posts/cpp/a0054/) 


