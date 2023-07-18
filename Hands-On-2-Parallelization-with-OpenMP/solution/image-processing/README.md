# Hands-On 2 - Session 2: Image Filtering

The solution to this problem involves dividing the image lines among different threads by adding OpenMP directives to the middle loop. It is possible to parallelize the inner loop, but it will have worse memory access pattern than the code shown below. It is impossible to parallelize at the outer loop because the steps must be applied sequentially to achieve the correct filtering result.

- On the main part of the filtering algorithm, inside the `apply_filter` function, add the `parallel for` directive to the middle loop;
- Mark the variables `i`, `j`, `k`, `l`, `s`, `total`, `filter_factor`, and `result` as `private`.

The code then becomes as follows:

```c
int apply_filter(int steps, int radius, struct pixel **src, struct pixel **dst,
                 int width, int height) {
/* ...omitted code... */
for (s = 0; s < steps; s++) {
#pragma omp parallel for private(i, j, k, l, s, total, filter_factor, result)
    for (i = 0; i < width; i++) {
      for (j = 0; j < height; j++) {
```

The provided image has a resolution of 512 by 512 pixels. Splitting it by lines creates 512 units of work that can be distributed among the threads.

|                             |                                |
| --------------------------- | ------------------------------ |
| ![](../images/time-1-2.jpg) | ![](../images/speedup-1-2.jpg) |
