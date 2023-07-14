/*
File:           image.c
Last changed:   20220303 11:35:00 
Purpose:        Parallelize image filtering using openMP
Author:         Murilo Boratto  <muriloboratto 'at' fieb.org.br>
Usage:
HowToCompile:   gcc image.c -o image -fopenmp
HowToExecute:   OMP_NUM_THREADS=${num_threads} ./image
                OMP_NUM_THREADS=4              ./image
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define max(a, b) ((a) > (b) ? (a) : (b))
#define min(a, b) ((a) < (b) ? (a) : (b))
#define MAXCAD 100

#define NUM_STEPS 5
#define RADIUS 8
#define INPUT_IMAGE "lenna.ppm"
#define OUTPUT_IMAGE "lenna-fil.ppm"

struct pixel
{
  unsigned char r, g, b;
};

int read_ppm_image(char *file_name, struct pixel ***img, int *out_width, int *out_height)
{
  FILE *fd;
  char type[MAXCAD];
  int rgb_max, actual_width, actual_height, i, j;

  fd = fopen(file_name, "r");
  if (fd == NULL)
  {
    printf("failed opening file %s\n", file_name);
    return -1;
  }

  fscanf(fd, "%s", type);
  if (strcmp(type, "P3") != 0)
  {
    printf("wrong format. P3 format expected\n");
    return -1;
  }

  fscanf(fd, "%d%d", &actual_width, &actual_height);

  if ((*img = (struct pixel **)malloc(sizeof(struct pixel *) * actual_height)) == NULL)
  {
    printf("failed allocating memory for %d rows\n", actual_height);
    return -1;
  }
  if (((*img)[0] = (struct pixel *)malloc(sizeof(struct pixel) * actual_width * actual_height)) == NULL)
  {
    printf("failed de locating memory for %d * %d pixels\n", actual_width, actual_height);
    return -1;
  }
  for (i = 1; i < actual_height; i++)
    (*img)[i] = (*img)[i - 1] + actual_width;

  fscanf(fd, "%d", &rgb_max);

  for (i = 0; i < actual_height; i++)
    for (j = 0; j < actual_width; j++)
    {
      fscanf(fd, "%hhu", &((*img)[i][j].r));
      fscanf(fd, "%hhu", &((*img)[i][j].g));
      fscanf(fd, "%hhu", &((*img)[i][j].b));
    }

  *out_width = actual_height;
  *out_height = actual_width;

  fclose(fd);

  return 0;
}

int write_ppm_image(char *file_name, struct pixel **img, int width, int height)
{
  FILE *fd;
  int rgb_max = 255, i, j;

  fd = fopen(file_name, "w");
  if (fd == NULL)
  {
    printf("failed opening file %s\n", file_name);
    return -1;
  }

  fprintf(fd, "P3\n");
  fprintf(fd, "%d %d\n", height, width);
  fprintf(fd, "%d\n", rgb_max);

  for (i = 0; i < width; i++)
  {
    for (j = 0; j < height; j++)
    {
      fprintf(fd, "%d ", img[i][j].r);
      fprintf(fd, "%d ", img[i][j].g);
      fprintf(fd, "%d ", img[i][j].b);
    }
    fprintf(fd, "\n");
  }

  fclose(fd);

  return 0;
}

int apply_filter(int steps, int radius, struct pixel **src, struct pixel **dst, int width, int height)
{
  int i, j, k, l, s, total;

  struct
  {
    int r, g, b;
  } result;
  int **filter_block, filter_factor;

  if ((filter_block = (int **)malloc(sizeof(int *) * (2 * (radius + 1)))) == NULL)
  {
    printf("failed allocating memory for the filter block\n");
    exit(-1);
  }
  if ((filter_block[0] = (int *)malloc(sizeof(int) * (2 * (radius + 1)) * (2 * (radius + 1)))) == NULL)
  {
    printf("failed allocating memory for the filter block\n");
    exit(-1);
  }
  for (i = 1; i < 2 * (radius + 1); i++)
    filter_block[i] = filter_block[i - 1] + 2 * (radius + 1);

  for (i = -radius; i <= radius; i++)
    for (j = -radius; j <= radius; j++)
      filter_block[i + radius][j + radius] = (radius - abs(i)) * (radius - abs(i)) + (radius - abs(j)) * (radius - abs(j)) + 1;

  for (s = 0; s < steps; s++)
  {
    for (i = 0; i < width; i++)
    {
      for (j = 0; j < height; j++)
      {
        result.r = 0;
        result.g = 0;
        result.b = 0;
        total = 0;
        for (k = max(0, i - radius); k <= min(width - 1, i + radius); k++)
        {
          for (l = max(0, j - radius); l <= min(height - 1, j + radius); l++)
          {
            filter_factor = filter_block[k - i + radius][l - j + radius];
            result.r += src[k][l].r * filter_factor;
            result.g += src[k][l].g * filter_factor;
            result.b += src[k][l].b * filter_factor;
            total += filter_factor;
          }
        }
        result.r /= total;
        result.g /= total;
        result.b /= total;
        dst[i][j].r = result.r;
        dst[i][j].g = result.g;
        dst[i][j].b = result.b;
      }
    }
    if (s + 1 < steps)
      memcpy(src[0], dst[0], width * height * sizeof(struct pixel));
  }
  free(filter_block[0]);
  free(filter_block);
  return 0;
}

int main()
{
  struct pixel **src_img, **dst_img;
  int width, height;
  int i, result;

  result = read_ppm_image(INPUT_IMAGE, &src_img, &width, &height);

  dst_img = (struct pixel **)malloc(width * sizeof(struct pixel *));
  dst_img[0] = (struct pixel *)malloc(height * width * sizeof(struct pixel));
  for (i = 1; i < width; i++)
    dst_img[i] = dst_img[i - 1] + height;

  apply_filter(NUM_STEPS, RADIUS, src_img, dst_img, width, height);

  result = write_ppm_image(OUTPUT_IMAGE, dst_img, width, height);

  free(src_img[0]);
  free(dst_img[0]);
  free(src_img);
  free(dst_img);

  printf("filtered image resolution: %dx%d\n", width, height);

  return 0;
}
