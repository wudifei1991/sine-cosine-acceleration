#ifndef DEFINE_H
#define DEFINE_H
 
#define x86

#define CONST_PI  3.14159265358979323846264338327950288419716939937510
#define CONST_2PI 6.28318530717958623199592693708837032318115234375000
#define CONST_PI_2  1.570796327

#define CONST_PI_F  3.14159265F
#define CONST_2PI_F 6.28318531F
#define CONST_PI_2_F  1.57079633F

#define D2R 0.017453293

#define modd(x, y) ((x) - (int)((x) / (y)) * (y))
#define lerp(w, v1, v2) ((1.0 - (w)) * (v1) + (w) * (v2))
#define max(x, y) ((x) > (y) ? (x) : (y))
#define min(x, y) ((x) < (y) ? (x) : (y))

static inline double absd(double a) { *((unsigned long *)&a) &= ~(1UL << 63); return a; }
static inline float absf(float a) { *((unsigned int *)&a) &= ~(1UL << 31); return a; }

// reference: https://en.wikipedia.org/wiki/Fast_inverse_square_root
static inline double rsqrt(double x)
{
  double y = x;
  double x2 = y * 0.5;
  long i = *(long *)&y;
  // https://cs.uwaterloo.ca/~m32rober/rsqrt.pdf
  i = 0x5fe6eb50c7b537a9 - (i >> 1);
  y = *(double *)&i;
  y = y * (1.5 - (x2 * y * y));
  return y * (1.5 - (x2 * y * y));
}

#endif