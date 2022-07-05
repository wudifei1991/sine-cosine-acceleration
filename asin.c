#include "asintable_1.h"
#include "asintable_0_1.h"
#include "asintable_0_01.h"
#include "asintable_0_001.h"
#include "asintable_0_0001.h"
#include <math.h>
#include "define.h"

//
// Lookup table.
//

double asin_table_1(double x)
{
    double x1 = absd(x);
    double y = asintable_1[(int)(x1 + 0.5)];
    return x > 0 ? y : -y;
}

double asin_table_0_1(double x)
{
    double x1 = absd(x);
    double y = asintable_0_1[(int)(x1 * 10 + 0.5)];
    return x > 0 ? y : -y;
}


double asin_table_0_01(double x)
{
    double x1 = absd(x);
    double y = asintable_0_01[(int)(x1 * 100 + 0.5)];
    return x > 0 ? y : -y;
}

double asin_table_0_001(double x)
{
    double x1 = absd(x);
    double y = asintable_0_001[(int)(x1 * 1000 + 0.5)];
    return x > 0 ? y : -y;
}

double asin_table_0_0001(double x)
{
    double x1 = absd(x);
    double y = asintable_0_0001[(int)(x1 * 10000 + 0.5)];
    return x > 0 ? y : -y;
}

//
// Lookup table with LERP.
//

double asin_table_1_LERP(double x)
{
    double x1 = absd(x);
    int index = (int)x1;
    return lerp(x1 - index,             /* weight      */
                asintable_1[index],     /* lower value */
                asintable_1[index + 1]  /* upper value */
                );
}

double asin_table_0_1_LERP(double x)
{
    double x1 = absd(x);
    double i = x1 * 10.0;
    int index = (int)i;
    return lerp(i - index,               /* weight      */
                asintable_0_1[index],     /* lower value */
                asintable_0_1[index + 1]  /* upper value */
                );
}

double asin_table_0_01_LERP(double x)
{
    double x1 = absd(x);
    double i = x1 * 100.0;
    int index = (int)i;
    return lerp(i - index,                /* weight      */
                asintable_0_01[index],     /* lower value */
                asintable_0_01[index + 1]  /* upper value */
                );
}

double asin_table_0_001_LERP(double x)
{
    double x1 = absd(x);
    double i = x1 * 1000.0;
    int index = (int)i;
    return lerp(i - index,                 /* weight      */
                asintable_0_001[index],     /* lower value */
                asintable_0_001[index + 1]  /* upper value */
                );
}

double asin_table_0_0001_LERP(double x)
{
    double x1 = absd(x);
    double i = x1 * 10000.0;
    int index = (int)i;
    return lerp(i - index,                  /* weight      */
                asintable_0_0001[index],     /* lower value */
                asintable_0_0001[index + 1]  /* upper value */
                );
}



// reference: https://developer.download.nvidia.cn/cg/index_stdlib.html
static inline double fast_acc_asin(double x)
{
  double x1 = absd(x);
  double y1 =
      CONST_PI_2 - sqrt(1 - x1) *
                      (1.5707288 +
                       x1 * (-0.2121144 + x1 * (0.0742610 + -0.0187293 * x1)));
  return x > 0 ? y1 : -y1;
}

static inline double fast_acc_rsqrt_asin(double x)
{
  double x1 = absd(x);
  double x2 = 1 - x1;
  double y1 =
      CONST_PI_2 - x2 * rsqrt(x2) *
                      (1.5707288 +
                       x1 * (-0.2121144 + x1 * (0.0742610 + -0.0187293 * x1)));
  return x > 0 ? y1 : -y1;
}

static inline double fast_acc_bit_asin(double x)
{
  double x1 = absd(x);
  double y1 =
      CONST_PI_2 - sqrt(1 - x1) *
                      (1.5707288 +
                       x1 * (-0.2121144 + x1 * (0.0742610 + -0.0187293 * x1)));
  long sgn = (*(long *)&x) & 0x80000000;
  long y1_i = *(long *)&y1 | sgn;
  return *(double *)&(y1_i);
}

static inline double fast_asin(double x)
{
  double x1 = absd(x);
  double y1 = CONST_PI_2 - sqrt(1 - x1) *
                              (CONST_PI_2 + x1 * (-0.207035422 + x1 * 0.053103955));
  return x > 0 ? y1 : -y1;
}

static inline double fast_rsqrt_asin(double x)
{
  double x1 = absd(x);
  double x2 = 1 - x1;
  double y1 = CONST_PI_2 - x2 * rsqrt(x2) *
                              (CONST_PI_2 + x1 * (-0.207035422 + x1 * 0.053103955));
  return x > 0 ? y1 : -y1;
}

static inline double fast_bit_asin(double x)
{
  double x1 = absd(x);
  double y1 = CONST_PI_2 - sqrt(1 - x1) *
                              (CONST_PI_2 + x1 * (-0.207035422 + x1 * 0.053103955));
  long sgn = (*(long *)&x) & 0x80000000;
  long y1_i = *(long *)&y1 | sgn;
  return *(double *)&(y1_i);
}

static inline double asin_math_h(double x)
{
  return asin(x);
}