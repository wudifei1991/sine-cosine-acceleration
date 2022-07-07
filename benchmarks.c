#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <string.h>
#include "define.h"
#ifdef x86
#include "malloc.h"
#endif
#include "cosine.c"
#include "asin.c"
#include "atan2.c"
#include "spheric_rigid_transform.c"

// Measures the time of many executions in seconds. Smaller number is better.
double runtime(double (*func)(double))
{
    clock_t start = clock();
    for (int i = 0; i < 100000000; i++)
    {
        volatile double c = func(i / 10000.0);
        (void)c;
    }
    return (clock() - start) / (double)CLOCKS_PER_SEC;
}

double runtime_asin(double (*func)(double))
{
    double sp = -1.0;
    double ep = 1.0;
    double step = 0.0001;
    unsigned long size = (ep - sp) / step + 1;
    double *data = (double *)malloc(size * sizeof(double));
    unsigned long count;
    for (count = 0; count < size; ++count)
    {
        data[count] = count * step + sp;
        if (data[count] > 1.0)
        {
            break;
        }
    }
    size = count;
    int loop = 100000000 / size + 1;
    clock_t start = clock();
    for (int i = 0; i < loop; i++)
    {
        for (int j = 0; j < size; ++j)
        {
            volatile double c = func(data[j]);
            (void)c;
        }
    }
    double res = (clock() - start) / (double)CLOCKS_PER_SEC;
    free(data);
    return res;
}

double runtime_atan2(double (*func)(double, double)) {
    double sp = 0;
    double ep = CONST_2PI;
    double step = 0.0001;
    unsigned long size = (ep - sp) / step + 1;
    double *data_x = (double *)malloc(size * sizeof(double));
    double *data_y = (double *)malloc(size * sizeof(double));
    unsigned long count;
    double angle;
    for (count = 0; count < size; ++count)
    {
        angle = count * step + sp;
        if (angle > ep)
        {
            break;
        }
        data_x[count] = cos(angle);
        data_y[count] = cos(angle);
        
    }
    size = count;
    int loop = 100000000 / size + 1;
    clock_t start = clock();
    for (int i = 0; i < loop; i++)
    {
        for (int j = 0; j < size; ++j)
        {
            volatile double c = func(data_y[j], data_x[j]);
            (void)c;
        }
    }
    double res = (clock() - start) / (double)CLOCKS_PER_SEC;
    free(data_x);
    free(data_y);
    return res;
}

// Finds the worst case for accuracy compared to math.h. Smaller number is better.
double accuracy(double (*func)(double))
{
    double w = -1;
    double start = 0;
    double stop = CONST_PI_2;
    double step = 0.0000001;
    for (double i = start; i < stop; i += step)
    {
        double c = absd(func(i) - cos(i));
        if (c > w)
        {
            w = c;
        }
    }
    return w;
}

double accuracy_sin(double (*func)(double))
{
    double w = -1;
    double start = 0;
    double stop = CONST_PI_2;
    double step = 0.0000001;
    for (double i = start; i < stop; i += step)
    {
        double c = absd(func(i) - sin(i));
        if (c > w)
        {
            w = c;
        }
    }
    return w;
}

double accuracy_asin(double (*func)(double))
{
    double w = 0.;
    double start = 0.;
    double stop = 1.;
    double step = 0.0000001;
    for (double i = 0; i < stop; i += step)
    {
        double c = absd(func(i) - asin(i));
        if (c > w)
        {
            w = c;
        }
    }
    return w;
}

double accuracy_atan2(double (*func)(double, double)) {
    double w = 0.;
    double sp = 0.;
    double step = 0.0000001;
    double ep = CONST_2PI - step;
    double x, y;
    for (double i = 0; i < ep; i += step) {
        x = cos(i);
        y = sin(i);
        double c = absd(func(y, x) - atan2(y, x));
        if (c > w) {
            w = c;
        }
    }
    return w;
}

#define RTEST(x) \
    {            \
#x, x    \
    }

static struct
{
    char name[35];
    double (*func)(double v);
} tests[] = {
    /* cos_taylor_literal */
    RTEST(cos_taylor_literal_4terms_naive),
    RTEST(cos_taylor_literal_6terms_naive),
    RTEST(cos_taylor_literal_6terms_2pi),
    RTEST(cos_taylor_literal_6terms_pi),
    RTEST(cos_taylor_literal_6terms),
    RTEST(cos_taylor_literal_10terms),
    /* cos_taylor_running */
    RTEST(cos_taylor_running_6terms),
    RTEST(cos_taylor_running_8terms),
    RTEST(cos_taylor_running_10terms),
    RTEST(cos_taylor_running_16terms),
    /* cos_table */
    RTEST(cos_table_1),
    RTEST(cos_table_0_1),
    RTEST(cos_table_0_01),
    RTEST(cos_table_0_001),
    RTEST(cos_table_0_0001),
    /* cos_table_lerp */
    RTEST(cos_table_1_LERP),
    RTEST(cos_table_0_1_LERP),
    RTEST(cos_table_0_01_LERP),
    RTEST(cos_table_0_001_LERP),
    RTEST(cos_table_0_0001_LERP),
    /* Built-in cosine */
    RTEST(cos_math_h),
},
  tests_asin[] = {
      /* asin_table */
      RTEST(asin_table_1), RTEST(asin_table_0_1), RTEST(asin_table_0_01), RTEST(asin_table_0_001), RTEST(asin_table_0_0001),
      /* asin_table_lerp */
      RTEST(asin_table_1_LERP), RTEST(asin_table_0_1_LERP), RTEST(asin_table_0_01_LERP), RTEST(asin_table_0_001_LERP), RTEST(asin_table_0_0001_LERP),
      /* cubic curve */
      RTEST(fast_acc_asin), RTEST(fast_acc_rsqrt_asin), RTEST(fast_acc_bit_asin),
      /* quadratic curve */
      RTEST(fast_asin), RTEST(fast_rsqrt_asin), RTEST(fast_bit_asin),
      /* Built-in arcsine */
      RTEST(asin_math_h)};

static struct
{
    char name[35];
    double (*func)(double y, double x);
} tests_atan2[] = {
    RTEST(nvd_atan2),
    RTEST(fast_atan2),
    RTEST(fast_acc_atan2),
    RTEST(atan2_math_h)
};

static struct
{
    char name[35];
    void (*func)(float *, float *, float *, unsigned long);
} tests_rigid_transform_runtime[] = {
    RTEST(fast_rigid_transform),
    RTEST(rigid_transform)
};

static struct
{
    char name[35];
    void (*func)(float *, float *, float *, unsigned long, float *);
} tests_rigid_transform_accuracy[] = {
    RTEST(fast_rigid_transform2)
};

const int num_tests = sizeof(tests) / sizeof(*tests);
const int num_tests_asin = sizeof(tests_asin) / sizeof(*tests_asin);
const int num_tests_atan2 = sizeof(tests_atan2) / sizeof(*tests_atan2);
const int num_tests_rigid_transform_runtime = sizeof(tests_rigid_transform_runtime) / sizeof(*tests_rigid_transform_runtime);
const int num_tests_rigid_transform_accuracy = sizeof(tests_rigid_transform_accuracy) / sizeof(*tests_rigid_transform_accuracy);

// Benchmarks the accuracy and time for all of our cosine implementations.
int main(int argc, char *argv[])
{

    int run_cos = 0;
    int run_asin = 0;
    int run_atan2 = 0;
    int run_rigid_transform = 0;
    int run_accuracy = 1;
    int run_runtime = 1;
    int loop = 30;
    int disable_table = 1;
    int i;
    int j;

    for (i = 1; i < argc; i++)
    {
        if (!strcmp(argv[i], "-cos"))
        {
            run_cos = 1;
        }
        else if (!strcmp(argv[i], "-asin"))
        {
            run_asin = 1;
        }
        else if (!strcmp(argv[i], "-atan2"))
        {
            run_atan2 = 1;
        }
        else if (!strcmp(argv[i], "-rt"))
        {
            run_rigid_transform = 1;
        }
        else if (!strcmp(argv[i], "-na"))
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
        else if (!strcmp(argv[i], "-table")) {
            disable_table = 0;
        }
        else
        {
            printf("Usage: %s [-na] [-nt] [-cos] [-asin] [-table] [-l <loop times>]\n   -na - Don't run accuracy tests\n   -nt - Don't run speed tests.\n   -cos - Run cos test.\n   -asin - Run asin test.\n   -table - enable lut functions.\n   -l <loop time> - Runtime loop times.\n", argv[0]);
            return 0;
        }
    }

    if (!run_cos && !run_asin && !run_atan2 && !run_rigid_transform) {
        run_cos = 1;
        run_asin = 1;
        run_atan2 = 1;
        run_rigid_transform = 1;
    }

    if (run_cos)
    {
        printf("Cosine benchmark\n\n");
        if (run_accuracy)
        {
            printf("ACCURACY\n");
            for (i = 0; i < num_tests; i++)
            {
                if (disable_table && strstr(tests[i].name, "table")) {
                    continue;
                }
                printf("%-35s %.16lf\n", tests[i].name, accuracy(tests[i].func));
            }

            printf("%-35s %.16lf\n", "fast_cosine", accuracy(fast_cosine));
            printf("%-35s %.16lf\n", "fast_sine", accuracy_sin(fast_sine));
            printf("%-35s %.16lf\n", "fast_acc_cosine", accuracy(fast_acc_cosine));
            printf("%-35s %.16lf\n", "fast_acc_cosine_v2", accuracy(fast_acc_cosine_v2));
            printf("%-35s %.16lf\n", "fast_acc_sine", accuracy_sin(fast_acc_sine));
        }

        if (run_runtime)
        {
            printf("\nTIME\n");
            for (i = 0; i < num_tests; i++)
            {
                if (disable_table && strstr(tests[i].name, "table")) {
                    continue;
                }
                printf("%-35s %.16lf\n", tests[i].name, runtime(tests[i].func));
            }

            printf("%-35s %.16lf\n", "fast_cosine", runtime(fast_cosine));
            printf("%-35s %.16lf\n", "fast_sine", runtime(fast_sine));
            printf("%-35s %.16lf\n", "fast_acc_cosine", runtime(fast_acc_cosine));
            printf("%-35s %.16lf\n", "fast_acc_cosine_v2", runtime(fast_acc_cosine_v2));
            printf("%-35s %.16lf\n", "fast_acc_sine", runtime(fast_acc_sine));
        }
    }

    if (run_asin)
    {
        printf("ArcSine benchmark\n\n");
        if (run_accuracy)
        {
            printf("ACCURACY\n");
            for (i = 0; i < num_tests_asin; i++)
            {
                if (disable_table && strstr(tests_asin[i].name, "table")) {
                    continue;
                }
                printf("%-35s %.16lf\n", tests_asin[i].name, accuracy_asin(tests_asin[i].func));
            }
        }

        if (run_runtime)
        {
            printf("\nTIME\n");
            for (i = 0; i < num_tests_asin - 1; i++)
            {
                if (disable_table && strstr(tests_asin[i].name, "table")) {
                    continue;
                }
                printf("%-20s,", tests_asin[i].name);
            }
            printf("%-20s\n", tests_asin[num_tests_asin - 1].name);
            double ct_sum_list[num_tests_asin];
            for (i = 0; i < num_tests_asin; i++)
            {
                ct_sum_list[i] = 0.;
            }
            int count = 0;
            while (count++ < loop)
            {
                double ct_tmp;
                for (i = 0; i < num_tests_asin - 1; i++)
                {
                    if (disable_table && strstr(tests_asin[i].name, "table")) {
                        continue;
                    }
                    ct_tmp = runtime_asin(tests_asin[i].func);
                    ct_sum_list[i] += ct_tmp;
                    printf("%.18lf,", ct_tmp);
                }
                ct_tmp = runtime_asin(tests_asin[num_tests_asin - 1].func);
                ct_sum_list[num_tests_asin - 1] += ct_tmp;
                printf("%.18lf\n", ct_tmp);
            }

            printf("\nAERAGE TIME\n");

            double inv_loop = 1.0 / loop;
            for (i = 0; i < num_tests_asin; i++)
            {
                if (disable_table && strstr(tests_asin[i].name, "table")) {
                    continue;
                }
                printf("%-35s %.18lf\n", tests_asin[i].name, ct_sum_list[i] * inv_loop);
            }
        }
    }

    if (run_atan2) {
        printf("ArcTan2 benchmark\n\n");
        if (run_accuracy) {
            printf("ACCURACY\n");
            for (i = 0; i < num_tests_atan2; i++)
            {
                if (disable_table && strstr(tests_atan2[i].name, "table")) {
                    continue;
                }
                printf("%-35s %.16lf\n", tests_atan2[i].name, accuracy_atan2(tests_atan2[i].func));
            }
        }
        if (run_runtime)
        {
            printf("\nTIME\n");
            for (i = 0; i < num_tests_atan2 - 1; i++)
            {
                if (disable_table && strstr(tests_atan2[i].name, "table")) {
                    continue;
                }
                printf("%-20s,", tests_atan2[i].name);
            }
            printf("%-20s\n", tests_atan2[num_tests_atan2 - 1].name);
            double ct_sum_list[num_tests_atan2];
            for (i = 0; i < num_tests_atan2; i++)
            {
                ct_sum_list[i] = 0.;
            }
            int count = 0;
            while (count++ < loop)
            {
                double ct_tmp;
                for (i = 0; i < num_tests_atan2 - 1; i++)
                {
                    if (disable_table && strstr(tests_atan2[i].name, "table")) {
                        continue;
                    }
                    ct_tmp = runtime_atan2(tests_atan2[i].func);
                    ct_sum_list[i] += ct_tmp;
                    printf("%.18lf,", ct_tmp);
                }
                ct_tmp = runtime_atan2(tests_atan2[num_tests_atan2 - 1].func);
                ct_sum_list[num_tests_atan2 - 1] += ct_tmp;
                printf("%.18lf\n", ct_tmp);
            }

            printf("\nAERAGE TIME\n");

            double inv_loop = 1.0 / loop;
            for (i = 0; i < num_tests_atan2; i++)
            {
                if (disable_table && strstr(tests_atan2[i].name, "table")) {
                    continue;
                }
                printf("%-35s %.18lf\n", tests_atan2[i].name, ct_sum_list[i] * inv_loop);
            }
        }

    }

    if (run_rigid_transform) {
        printf("Spheric Rigid Transform benchmark\n\n");
        float w = 0.;
        for (float i = -1; i < 1; i+=0.001) {
            float c = absf(asinf(i) - f_asinf(i));
            if (c > w) {
                w = c;
            }
        }
        printf("sin err: %f\n", w);

        if (run_accuracy) {
            printf("ACCURACY\n");
            for (i = 0; i < num_tests_rigid_transform_accuracy; i++)
            {
                printf("%-35s %.16lf\n", tests_rigid_transform_accuracy[i].name, accuracy_rigid_transform(tests_rigid_transform_accuracy[i].func));
            }
        }

        if (run_runtime) {
            printf("\nTIME\n");
            for (i = 0; i < num_tests_rigid_transform_runtime - 1; i++)
            {
                printf("%-20s,", tests_rigid_transform_runtime[i].name);
            }
            printf("%-20s\n", tests_rigid_transform_runtime[num_tests_rigid_transform_runtime - 1].name);
            double ct_sum_list[num_tests_rigid_transform_runtime];
            for (i = 0; i < num_tests_rigid_transform_runtime; i++)
            {
                ct_sum_list[i] = 0.;
            }
            int count = 0;
            while (count++ < loop)
            {
                double ct_tmp;
                for (i = 0; i < num_tests_rigid_transform_runtime - 1; i++)
                {
                    ct_tmp = runtime_rigid_transform(tests_rigid_transform_runtime[i].func);
                    ct_sum_list[i] += ct_tmp;
                    printf("%.18lf,", ct_tmp);
                }
                ct_tmp = runtime_rigid_transform(tests_rigid_transform_runtime[num_tests_rigid_transform_runtime - 1].func);
                ct_sum_list[num_tests_rigid_transform_runtime - 1] += ct_tmp;
                printf("%.18lf\n", ct_tmp);
            }

            printf("\nAERAGE TIME\n");

            double inv_loop = 1.0 / loop;
            for (i = 0; i < num_tests_rigid_transform_runtime; i++)
            {
                printf("%-35s %.18lf\n", tests_rigid_transform_runtime[i].name, ct_sum_list[i] * inv_loop);
            }
        }

        
    }

    printf("\n\nDone\n");

    return 0;
}