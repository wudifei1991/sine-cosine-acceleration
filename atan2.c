#include <math.h>
#include <float.h>
#include "define.h"

// reference: https://developer.download.nvidia.cn/cg/atan2.html
static inline double nvd_atan2(double y, double x)
{
  double t0, t1, t3, t4;

  t3 = absd(x);
  t1 = absd(y);
  t0 = max(t3, t1);
  t1 = min(t3, t1);
  t3 = 1.0/ t0;
  t3 = t1 * t3;

  t4 = t3 * t3;
  t0 =         - 0.013480470;
  t0 = t0 * t4 + 0.057477314;
  t0 = t0 * t4 - 0.121239071;
  t0 = t0 * t4 + 0.195635925;
  t0 = t0 * t4 - 0.332994597;
  t0 = t0 * t4 + 0.999995630;
  t3 = t0 * t3;


  t3 = (absd(y) > absd(x)) ? CONST_PI_2 - t3 : t3;
  t3 = (x < 0) ? CONST_PI - t3 : t3;
  t3 = (y < 0) ? -t3 : t3;

  return t3;
}

static inline double fast_atan2(double y, double x) {
  double t0, t1, t2, t3;
  t0 = absd(x);
  t1 = absd(y);
  t2 = max(t0, t1);
  t3 = min(t0, t1);
  t3 = t3 / t2;
  t2 = t3 * t3;

  t3 = t3 * (0.999208 + t2 * (-0.321088 + t2 * (0.145917 + t2 * -0.038638)));

  t3 = (t1 > t0) ? CONST_PI_2 - t3 : t3;
  t3 = (x < 0) ? CONST_PI - t3 : t3;
  t3 = (y < 0) ? -t3 : t3;

  return t3;
}

static inline double fast_acc_atan2(double y, double x) {
  double t0, t1, t2, t3;
  t0 = absd(x);
  t1 = absd(y);
  t2 = max(t0, t1);
  t3 = min(t0, t1);
  t3 = t3 / t2;
  t2 = t3 * t3;

  t3 = t3 * (0.999868 + t2 * (-0.330337 + t2 * (0.180269 + t2 * (-0.085259 + t2 * 0.020857))));

  t3 = (t1 > t0) ? CONST_PI_2 - t3 : t3;
  t3 = (x < 0) ? CONST_PI - t3 : t3;
  t3 = (y < 0) ? -t3 : t3;

  return t3;
}

static inline double atan2_math_h(double y, double x)
{
  return atan2(y, x);
}