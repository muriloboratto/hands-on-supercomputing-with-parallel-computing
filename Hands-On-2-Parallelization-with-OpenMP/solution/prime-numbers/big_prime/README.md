# Hands-On 2 - Session 3: Biggest Prime Number

The solution to this problem involves identifying a strategy that allows the `is_prime` function to avoid parallelizing the calculation if the number being checked is not composed of large factors:

- Define a suitable boundary to switch between executing the modulo operations sequentially and in parallel. If the number being checked is composed of factors bigger than the boundary reference, then the code should execute in parallel for the subsequent iterations.

- Write the second part of the calculation as a `while` loop inside an OpenMP parallel region, utilizing the `omp_get_thread_num` and `omp_get_num_threads` to divide the work among the threads.

- Add the `firstprivate` directive to the `i` variable and the `&` reduction to the `result` variable.

The code then becomes as follows:

```c
#define PARALLEL_BOUNDARY ULLONG_MAX / 100000000000

int is_prime (big_integer n) {
  int result = (n % 2 != 0 || n == 2);
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

    while (result && i <= sq_root) {
      result = n % i != 0;
      i += increment;
    }
  }

  return result;
}
```

|                             |                                |
| --------------------------- | ------------------------------ |
| ![](/solutions/hands-on-1/images/time-1-3.jpg) | ![](/solutions/hands-on-1/images/speedup-1-3.jpg) |
