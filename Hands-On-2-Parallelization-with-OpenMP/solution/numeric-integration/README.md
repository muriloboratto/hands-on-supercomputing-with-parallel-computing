# Hands-On 2 - Session 1: Numeric Integration

The solution to this problem involves dividing the amount of points to be sampled from the function among different threads and then summing the results at the end. For the first approach, add the `parallel for` directive with the `+` reduction for the `s` variable:

```c
double integral(double a, double b, int n) {
/* ...omitted code... */
#pragma omp parallel for reduction(+:s)
   for (i = 0; i < n; i++) {
      s += f(a + h * (i + 0.5));
```
Looking at the performance results of the first approach of this code when executed in different conditions makes it is possible to understand how it can scale in regards to problem size and degrees of parallelism:

- The problem is of O(n) complexity, the execution time scales linearly with the number of steps in the integral calculation.

- The problem is embarrassingly parallel: all iterations can be computed independently and require no communication between threads. This allows the speedup factor to scale close to 1-to-1 with the number of threads.

|                             |                                |
| --------------------------- | ------------------------------ |
| ![](../images/time-1-1.jpg) | ![](../images/speedup-1-1.jpg) |
