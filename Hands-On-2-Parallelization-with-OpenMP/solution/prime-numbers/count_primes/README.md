# Hands-On 2 - Session 3: Counting Prime Numbers

The solution to this problem involves dividing the amount of numbers between the lower and upper limit among different threads and then incrementing a counter if the number evaluates as prime. The code necessary to parallelize this solution is similar to the one used to parallelize the integral calculation:

- Add the `parallel for` directive to the loop inside the main function.
- Associate the `+` reduction with the `primes` variable to avoid triggering a race condition when incrementing the counter.
- Add the `schedule` directive set to `runtime` so as to make it possible to alter the scheduling strategy using the `OMP_SCHEDULE` environment variable when executing the program, like so:

```
$ OMP_NUM_THREADS=4 OMP_SCHEDULE=static ./count_primes
```

The code then becomes as follows:

```c
int main() {
/* ...omitted code... */
#pragma omp parallel for reduction(+:primes) schedule(runtime)
  for (i = 3; i <= TOP_LIMIT; i += 2)
    if (is_prime(i))
      primes++;
```

Each loop iteration can have drastically different compute costs, which creates the opportunity to observe the effects of different scheduling strategies. It can be more performant to use `dynamic` scheduling when the compute cost varies per item. Also, specifying a chunk size, like `dynamic,64`, can help minimize the cost of shuffling work between threads and contribute to better performance characteristics.

|                             |                                |
| --------------------------- | ------------------------------ |
| ![](/solutions/hands-on-1/images/time-1-4.jpg) | ![](/solutions/hands-on-1/images/speedup-1-4.jpg) |

