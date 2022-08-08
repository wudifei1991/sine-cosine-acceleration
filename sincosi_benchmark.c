#include <time.h>
#include <malloc.h>
#include "sincosi.c"
#include "stdio.h"
#include <string.h>

#define RTEST(x) \
  {              \
#x, x        \
  }

static struct
{
  char name[35];
  void (*func)(int v, float *, float *);
} tests_sincosi[] = {RTEST(sincosi_lut), RTEST(sincosi_half_lut),
                     RTEST(sincosi_fast), RTEST(sincosi_std)};

int num_tests_sincosi = sizeof(tests_sincosi) / sizeof(*tests_sincosi);

double runtime_sincosi(void (*func)(int, float *, float *))
{
  int size = 8192 * 2 + 1;
  int max_size = 100000000;
  int *data = (int *)(malloc(max_size * sizeof(int)));
  for (int i = 0; i < max_size; ++i)
  {
    data[i] = i % size - 8192;
  }
  float *s = (float *)(malloc(max_size * sizeof(float)));
  float *c = (float *)(malloc(max_size * sizeof(float)));
  clock_t start = clock();
  for (int i = 0; i < max_size; i++)
  {
    func(data[i], c + i, s + i);
  }
  double res = (clock() - start) / (double)CLOCKS_PER_SEC;
  volatile float x1 = 0.f;
  for (int i = 0; i < max_size; ++i)
  {
    x1 += c[i];
    x1 += s[i];
  }
  (void)x1;
  free(data);
  free(c);
  free(s);
  return res;
}

float accuracy_sincosi(void (*func)(int, float *, float *))
{
  float w = 0.f;
  int start = 0;
  int stop = 8192;
  for (int i = start; i < stop; ++i)
  {
    float s, c;
    func(i, &c, &s);
    float angle = i * 3.83495197E-4;
    float dc = absf(c - cosf(angle));
    if (dc > w)
    {
      w = dc;
    }
    float ds = absf(s - sinf(angle));
    if (ds > w)
    {
      w = ds;
    }
  }
  return w;
}

int main(int argc, char *argv[])
{
  int run_sincosi = 1;
  int run_rt = 0;
  int run_accuracy = 1;
  int run_runtime = 1;
  int loop = 30;
  int i;

  for (i = 1; i < argc; i++)
  {
    if (!strcmp(argv[i], "-na"))
    {
      run_accuracy = 0;
    }
    else if (!strcmp(argv[i], "-nt"))
    {
      run_runtime = 0;
    }
    else if (!strcmp(argv[i], "-l"))
    {
      i++;
      loop = atoi(argv[i]);
    }
    else if (!strcmp(argv[i], "-rt"))
    {
      run_rt = 1;
    }
    else
    {
      printf(
          "Usage: %s [-na] [-nt] [-sincosi] [-rt] [-l <loop "
          "times>]\n"
          "   -na - Don't run accuracy tests\n"
          "   -nt - Don't run speed tests.\n"
          "   -sincosi - Run sincosi test.\n"
          "   -asini - Run asiniw test.\n"
          "   -atan2i - Run atan2i test.\n"
          "   -l <loop time> - Runtime loop times.\n"
          "   -rt - Run spheric_rigid_transform.\n",
          argv[0]);
      return 0;
    }
  }

  if (run_sincosi)
  {
    printf("\nsincosi benchmark\n");
    if (run_accuracy)
    {
      printf("ACCURACY\n");
      for (i = 0; i < num_tests_sincosi; i++)
      {
        printf("%-35s %.16lf\n", tests_sincosi[i].name,
               accuracy_sincosi(tests_sincosi[i].func));
      }
    }

    if (run_runtime)
    {
      printf("\nTIME\n");
      for (i = 0; i < num_tests_sincosi; i++)
      {
        printf("%-20s,", tests_sincosi[i].name);
      }
      printf("\n");
      double ct_sum_list[num_tests_sincosi];
      for (i = 0; i < num_tests_sincosi; i++)
      {
        ct_sum_list[i] = 0.;
      }
      int count = 0;
      double ct_tmp;
      while (count++ < loop)
      {
        for (i = 0; i < num_tests_sincosi; i++)
        {
          ct_tmp = runtime_sincosi(tests_sincosi[i].func);
          printf("%.16lf,", ct_tmp);
          ct_sum_list[i] += ct_tmp;
        }
        printf("\n");
      }

      printf("\nAERAGE TIME\n");
      double inv_loop = 1.0 / loop;
      for (i = 0; i < num_tests_sincosi; i++)
      {
        printf("%-35s %.18lf\n", tests_sincosi[i].name,
               ct_sum_list[i] * inv_loop);
      }
    }
  }
}