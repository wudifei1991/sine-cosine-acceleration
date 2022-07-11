/**
 * @file spheric_rigid_transform.c
 * @author wudifei (778205705@qq.com)
 * @brief
 * @version 0.1
 * @date 2022-07-06
 *
 * @copyright Copyright (c) 2022
 *
 */

#include "define.h"
#include <math.h>
#ifdef x86
#include <malloc.h>
#else
#include <stdlib.h>
#endif
#include <stdio.h>
#include <time.h>
#include <string.h>

// utility
static inline float absf(float a)
{
  *((unsigned int *)&a) &= ~(1UL << 31);
  return a;
}

static inline float rsqrtf(float x)
{
  int i;
  float x2, y;

  x2 = x * 0.5f;
  y = x;
  i = *(int *)&y;
  i = 0x5f3759df - (i >> 1);
  y = *(float *)&i;
  y = y * (1.5f - (x2 * y * y));
  y = y * (1.5f - (x2 * y * y));

  return y;
}

static inline float f_sinf(float x)
{
  float y = x * (1.273239545f + -0.405284735f * absf(x));
  return y * (absf(y) * (0.0192f * absf(y) + 0.1951f) + 0.7857f);
}

static inline float f_cosf(float x)
{
  x = absf(x);
  float y = x * (0.636619772f + -0.101321184f * x);
  float y1 = y * (y * (0.0271529f * y + 0.275913066f) + 1.111147596f);
  return 1.0f - y1 * y1;
}

static inline float f_asinf(float x)
{
  float x1 = absf(x);
  float y1 =
      CONST_PI_2_F - sqrtf(1 - x1) *
                         (1.5707288f +
                          x1 * (-0.2121144f + x1 * (0.0742610f + -0.0187293f * x1)));
  return x > 0 ? y1 : -y1;
}

static inline float f_atan2f(float y, float x)
{
  float t0, t1, t2, t3;
  t0 = absf(x);
  t1 = absf(y);
  t2 = max(t0, t1);
  t3 = min(t0, t1);
  t3 = t3 / t2;
  t2 = t3 * t3;

  t3 = t3 * (0.999868f + t2 * (-0.330337f + t2 * (0.180269f + t2 * (-0.085259f + t2 * 0.020857f))));

  t3 = (t1 > t0) ? CONST_PI_2_F - t3 : t3;
  t3 = (x < 0) ? CONST_PI_F - t3 : t3;
  t3 = (y < 0) ? -t3 : t3;

  return t3;
}

// impl
void fast_rigid_transform(float rotate[9], float trans[3], float *points, const unsigned long point_num)
{
 unsigned long i;
  float t0, t1, t2, t3, t4, t5, t6;

  for (i = 0; i < point_num; ++i)
  {
    t0 = points[3 * i];                                               // h
    t1 = points[3 * i + 1];                                           // v
    t2 = points[3 * i + 2];                                           // r
    t3 = t2 * f_sinf(t1);                                               // x
    t6 = t2 * f_cosf(t1);                                               // t
    t4 = t6 * f_sinf(t0);                                               // y
    t5 = t6 * f_cosf(t0);                                               // z
    t0 = t3 * rotate[0] + t4 * rotate[1] + t5 * rotate[2] + trans[0]; // dx
    t1 = t3 * rotate[3] + t4 * rotate[4] + t5 * rotate[5] + trans[1]; // dy
    t2 = t3 * rotate[6] + t4 * rotate[7] + t5 * rotate[8] + trans[2]; // dz
    t3 = t0 * t0 + t1 * t1 + t2 * t2;                                 // r2
    t6 = sqrtf(t3);                                                    // r
    points[3 * i] = t6 > 0 ? f_atan2f(t1, t2) : 0;
    points[3 * i + 1] = t6 > 0 ? f_asinf(t0 / t6) : 0;
    points[3 * i + 2] = t6;
  }
}

void fast_rigid_transform2(float rotate[9], float trans[3], float *points, const unsigned long point_num, float *dst_points)
{
  unsigned long i;
  float t0, t1, t2, t3, t4, t5, t6;

  for (i = 0; i < point_num; ++i)
  {
    t0 = points[3 * i];                                               // h
    t1 = points[3 * i + 1];                                           // v
    t2 = points[3 * i + 2];                                           // r
    t3 = t2 * f_sinf(t1);                                               // x
    t6 = t2 * f_cosf(t1);                                               // t
    t4 = t6 * f_sinf(t0);                                               // y
    t5 = t6 * f_cosf(t0);                                               // z
    t0 = t3 * rotate[0] + t4 * rotate[1] + t5 * rotate[2] + trans[0]; // dx
    t1 = t3 * rotate[3] + t4 * rotate[4] + t5 * rotate[5] + trans[1]; // dy
    t2 = t3 * rotate[6] + t4 * rotate[7] + t5 * rotate[8] + trans[2]; // dz

    t3 = t0 * t0 + t1 * t1 + t2 * t2; // r2
    t6 = sqrtf(t3);                    // r
    dst_points[3 * i] = t6 > 0.f ? f_atan2f(t1, t2) : 0.f;
    dst_points[3 * i + 1] = t6 > 0.f ? f_asinf(t0 / t6) : 0.f;
    dst_points[3 * i + 2] = t6;
  }
}

// standard
void rigid_transform(float rotate[9], float trans[3], float *points, const unsigned long point_num)
{
  unsigned long i;
  float t0, t1, t2, t3, t4, t5, t6;

  for (i = 0; i < point_num; ++i)
  {
    t0 = points[3 * i];                                               // h
    t1 = points[3 * i + 1];                                           // v
    t2 = points[3 * i + 2];                                           // r
    t3 = t2 * sinf(t1);                                               // x
    t6 = t2 * cosf(t1);                                               // t
    t4 = t6 * sinf(t0);                                               // y
    t5 = t6 * cosf(t0);                                               // z
    t0 = t3 * rotate[0] + t4 * rotate[1] + t5 * rotate[2] + trans[0]; // dx
    t1 = t3 * rotate[3] + t4 * rotate[4] + t5 * rotate[5] + trans[1]; // dy
    t2 = t3 * rotate[6] + t4 * rotate[7] + t5 * rotate[8] + trans[2]; // dz
    t3 = t0 * t0 + t1 * t1 + t2 * t2;                                 // r2
    t6 = sqrtf(t3);                                                    // r
    points[3 * i] = t6 > 0 ? atan2f(t1, t2) : 0;
    points[3 * i + 1] = t6 > 0 ? asinf(t0 / t6) : 0;
    points[3 * i + 2] = t6;
  }
}

void rigid_transform2(float rotate[9], float trans[3], float *points, const unsigned long point_num, float *dst_points)
{
  unsigned long i;
  float t0, t1, t2, t3, t4, t5, t6;

  for (i = 0; i < point_num; ++i)
  {
    t0 = points[3 * i];                                               // h
    t1 = points[3 * i + 1];                                           // v
    t2 = points[3 * i + 2];                                           // r
    t3 = t2 * sinf(t1);                                               // x
    t6 = t2 * cosf(t1);                                               // t
    t4 = t6 * sinf(t0);                                               // y
    t5 = t6 * cosf(t0);                                               // z
    t0 = t3 * rotate[0] + t4 * rotate[1] + t5 * rotate[2] + trans[0]; // dx
    t1 = t3 * rotate[3] + t4 * rotate[4] + t5 * rotate[5] + trans[1]; // dy
    t2 = t3 * rotate[6] + t4 * rotate[7] + t5 * rotate[8] + trans[2]; // dz

    t3 = t0 * t0 + t1 * t1 + t2 * t2; // r2
    t6 = sqrtf(t3);                    // r
    dst_points[3 * i] = t6 > 0.f ? atan2f(t1, t2) : 0.f;
    dst_points[3 * i + 1] = t6 > 0.f ? asinf(t0 / t6) : 0.f;
    dst_points[3 * i + 2] = t6;
  }
}

// test tools
float max_ptr(float *ptr, unsigned num)
{
  float max_val = 0.f;
  float tmp_val, err_x, err_y, err_z;
  unsigned nan_count = 0;
  for (unsigned i = 0; i < num; ++i)
  {
    err_x = ptr[3 * i];
    err_y = ptr[3 * i + 1];
    err_z = ptr[3 * i + 2];
    tmp_val = (err_x * err_x + err_y * err_y + err_z * err_z);
    if (tmp_val == tmp_val)
      max_val = max(max_val, tmp_val);
    else
      nan_count++;
  }
  printf("nan count is %u\n", nan_count);
  return sqrt(max_val);
}

static inline void mul_R3x3_R3x3(float src1[9], float src2[9], float dst[9])
{
  dst[0] = src1[0] * src2[0] + src1[1] * src2[3] + src1[2] * src2[6];
  dst[1] = src1[0] * src2[1] + src1[1] * src2[4] + src1[2] * src2[7];
  dst[2] = src1[0] * src2[2] + src1[1] * src2[5] + src1[2] * src2[8];

  dst[3] = src1[3] * src2[0] + src1[4] * src2[3] + src1[5] * src2[6];
  dst[4] = src1[3] * src2[1] + src1[4] * src2[4] + src1[5] * src2[7];
  dst[5] = src1[3] * src2[2] + src1[4] * src2[5] + src1[5] * src2[8];

  dst[6] = src1[6] * src2[0] + src1[7] * src2[3] + src1[8] * src2[6];
  dst[7] = src1[6] * src2[1] + src1[7] * src2[4] + src1[8] * src2[7];
  dst[8] = src1[6] * src2[2] + src1[7] * src2[5] + src1[8] * src2[8];
}

static inline void mul_R3x3_R3x3_R3x3(float src1[9], float src2[9], float src3[9],
                                      float dst[9])
{
  float tmp[9];
  mul_R3x3_R3x3(src1, src2, tmp);
  mul_R3x3_R3x3(tmp, src3, dst);
}

void convert_eluer_matrix(float x, float y, float z, float r[9])
{
  float cz = cosf(z);
  float sz = sinf(z);
  float rz[9] = {cz, -sz, 0, sz, cz, 0, 0, 0, 1};
  float cy = cosf(y);
  float sy = sinf(y);
  float ry[9] = {cy, 0, sy, 0, 1, 0, -sy, 0, cy};
  float cx = cosf(x);
  float sx = sinf(x);
  float rx[9] = {1, 0, 0, 0, cx, -sx, 0, sx, cx};
  mul_R3x3_R3x3_R3x3(rz, ry, rx, r);
}

unsigned test_case_size(const float step)
{
  unsigned long size_h = CONST_2PI_F / step;
  unsigned long size_v = CONST_PI_F / step;
  unsigned long size = size_h * size_v;
  return size;
}

void test_case(const float step, const float r, float *points)
{
  unsigned long count = 0;
  unsigned long size_h = CONST_2PI_F / step;
  unsigned long size_v = CONST_PI_F / step;
  for (size_t hi = 0; hi < size_h; ++hi)
  {
    float h = hi * step - CONST_PI_F;
    for (size_t vi = 0; vi < size_v; ++vi)
    {
      float v = vi * step - CONST_PI_2_F;
      points[count++] = h;
      points[count++] = v;
      points[count++] = r;
    }
  }
}

unsigned rt_size(const float step)
{
  float inv_step = 1.f / step;
  unsigned pitch_size = CONST_2PI * inv_step; // x
  unsigned yaw_size = CONST_PI * inv_step;    // y
  unsigned roll_size = CONST_2PI * inv_step;  // z
  unsigned trans_h_size = CONST_2PI * inv_step;
  unsigned trans_v_size = CONST_PI * inv_step;
  return pitch_size * yaw_size * roll_size * trans_h_size * trans_v_size;
}

void rt_case(const float step, const float trans_r, float *r_list, float *t_list)
{
  float inv_step = 1.f / step;
  unsigned pitch_size = CONST_2PI * inv_step; // x
  unsigned yaw_size = CONST_PI * inv_step;    // y
  unsigned roll_size = CONST_2PI * inv_step;  // z
  unsigned trans_h_size = CONST_2PI * inv_step;
  unsigned trans_v_size = CONST_PI * inv_step;
  unsigned pi, yi, ri, hi, vi;
  float pitch, yaw, roll, h_angle, v_angle;
  float tyz;
  float *r_tmp = r_list;
  float *t_tmp = t_list;
  float rotate[9], trans[3];
  unsigned count = 0;
  for (pi = 0; pi < pitch_size; ++pi)
  {
    pitch = pi * step - CONST_PI_F;
    for (yi = 0; yi < yaw_size; ++yi)
    {
      yaw = yi * step - CONST_PI_2_F;
      for (ri = 0; ri < roll_size; ++ri)
      {
        roll = ri * step - CONST_PI_F;
        convert_eluer_matrix(pitch, yaw, roll, rotate);
        for (vi = 0; vi < trans_h_size; ++vi)
        {
          v_angle = vi * step - CONST_PI_2_F;
          trans[0] = trans_r * sinf(v_angle);
          tyz = trans_r * cosf(v_angle);
          for (hi = 0; hi < trans_v_size; ++hi)
          {
            h_angle = hi * step - CONST_PI_F;
            trans[1] = tyz * sinf(h_angle);
            trans[2] = tyz * cosf(h_angle);
            memcpy(r_list + count * 9, rotate, 9 * sizeof(float));
            memcpy(t_list + count * 3, trans, 3 * sizeof(float));
            count++;
          }
        }
      }
    }
  }
}

void update_errors(unsigned point_num, float *standard_points, float *buffer, float *errors)
{
  for (unsigned i = 0; i < point_num; ++i)
  {
    if (buffer[3 * i] == buffer[3 * i] &&
        buffer[3 * i + 1] == buffer[3 * i + 1] &&
        buffer[3 * i + 2] == buffer[3 * i + 2])
    {
      float sx, sy, sz, dx, dy, dz, t;
      sx = standard_points[3 * i + 2] * sinf(standard_points[3 * i + 1]);
      t = standard_points[3 * i + 2] * cosf(standard_points[3 * i + 1]);
      sy = t * sinf(standard_points[3 * i]);
      sz = t * cosf(standard_points[3 * i]);

      dx = buffer[3 * i + 2] * sinf(buffer[3 * i + 1]);
      t = buffer[3 * i + 2] * cosf(buffer[3 * i + 1]);
      dy = t * sinf(buffer[3 * i]);
      dz = t * cosf(buffer[3 * i]);
      
      errors[3 * i] = max(errors[3 * i], absf(sx - dx));
      errors[3 * i + 1] = max(errors[3 * i + 1], absf(sy - dy));
      errors[3 * i + 2] = max(errors[3 * i + 2], absf(sz - dz));
    }
    else
    {
      printf("i = %u, h = %f, v = %f, r = %f\n", i, buffer[3 * i], buffer[3 * i + 1], buffer[3 * i + 2]);
    }
  }
}

double runtime_rigid_transform_impl(float r[9], float t[3], float *points, unsigned point_num, void (*func)(float *, float *, float *, unsigned long))
{
  float *tmp_points = (float *)malloc(3 * point_num * sizeof(float));
  memcpy(tmp_points, points, 3 * point_num * sizeof(float));
  clock_t start = clock();
  func(r, t, tmp_points, point_num);
  double ct = (clock() - start) / (double)CLOCKS_PER_SEC;
  free(tmp_points);
  return ct;
}

double runtime_rigid_transform(void (*func)(float *, float *, float *, unsigned long))
{
  // test case
  float step = CONST_PI / 200;
  float radius = 200;
  unsigned num = test_case_size(step);
  float *points = (float *)(malloc(num * 3 * sizeof(float)));
  test_case(step, radius, points);
  // test rt
  float ax, ay, az;
  ax = ay = az = CONST_PI * 0.25;
  float tx, ty, tz;
  tx = ty = tz = 1.f;
  float r[9];
  convert_eluer_matrix(ax, ay, az, r);
  float t[3] = {tx, ty, tz};
  // run
  double ct = runtime_rigid_transform_impl(r, t, points, num, func);
  // free
  free(points);

  return ct;
}

double accuracy_rigid_transform(void (*func)(float *, float *, float *, unsigned long, float *))
{
  float rt_step = 45 * D2R;
  unsigned rt_num = rt_size(rt_step);
  float *rotate_list = (float *)(malloc(rt_num * 9 * sizeof(float)));
  float *trans_list = (float *)(malloc(rt_num * 3 * sizeof(float)));
  float trans_radius = 50.f;
  rt_case(rt_step, trans_radius, rotate_list, trans_list);
  float point_step = CONST_PI / 300;
  float radius = 200.f;
  unsigned point_num = test_case_size(point_step);
  float *src_points = (float *)(malloc(point_num * 3 * sizeof(float)));
  test_case(point_step, radius, src_points);
  float *dst_points = (float *)(malloc(point_num * 3 * sizeof(float)));
  float *std_points = (float *)(malloc(point_num * 3 * sizeof(float)));
  float *errors = (float *)(malloc(point_num * 3 * sizeof(float)));
  memset(errors, 0, point_num * 3 * sizeof(float));
  float *rotate, *trans;
  for (unsigned i = 0; i < rt_num; ++i)
  {
    rotate = rotate_list + 9 * i;
    trans = trans_list + 3 * i;
    rigid_transform2(rotate, trans, src_points, point_num, std_points);
    for (unsigned j = 0; j < point_num; ++j)
    {
      if (std_points[3 * j] != std_points[3 * j] ||
          std_points[3 * j + 1] != std_points[3 * j + 1] ||
          std_points[3 * j + 2] != std_points[3 * j + 2])
      {
        printf("std: j = %u, h = %f, v = %f, r = %f\n", j, std_points[3 * j], std_points[3 * j + 1], std_points[3 * j + 2]);
      }
    }
    func(rotate, trans, src_points, point_num, dst_points);
    for (unsigned j = 0; j < point_num; ++j)
    {
      if (dst_points[3 * j] != dst_points[3 * j] ||
          dst_points[3 * j + 1] != dst_points[3 * j + 1] ||
          dst_points[3 * j + 2] != dst_points[3 * j + 2])
      {
        printf("dst: j = %u, h = %f, v = %f, r = %f\n", j, dst_points[3 * j], dst_points[3 * j + 1], dst_points[3 * j + 2]);
      }
    }
    update_errors(point_num, std_points, dst_points, errors);
  }
  double max_error = max_ptr(errors, point_num);
  free(rotate_list);
  free(trans_list);
  free(src_points);
  free(dst_points);
  free(std_points);
  free(errors);
  return max_error;
}