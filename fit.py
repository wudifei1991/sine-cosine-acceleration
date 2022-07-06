from math import atan, pi
import numpy as np

print("*******************start to fit **********************\n")

a = np.zeros([4, 4])
b = np.zeros([4, 1])
for i in range(0, 101) :
  x = i  / 100
  s9 = x ** 9 - x
  s7 = x ** 7 - x
  s5 = x ** 5 - x
  s3 = x ** 3 - x
  a[0, 0] += s9 ** 2
  a[0, 1] += s7 * s9
  a[0, 2] += s5 * s9
  a[0, 3] += s3 * s9
  a[1 ,1] += s7 ** 2
  a[1, 2] += s5 * s7
  a[1, 3] += s3 * s7
  a[2, 2] += s5 ** 2
  a[2, 3] += s3 * s5
  a[3, 3] += s3 ** 2
  t = (atan(x) - pi / 4 * x)
  b[0, 0] +=  t * s9
  b[1, 0] +=  t * s7
  b[2, 0] +=  t * s5
  b[3, 0] +=  t * s3
a[1, 0] = a[0, 1]
a[2, 0] = a[0, 2]
a[2, 1] = a[1, 2]
a[3, 0] = a[0, 3]
a[3, 1] = a[1, 3]
a[3, 2] = a[2, 3]

c = np.linalg.inv(a) @ b
d = pi/4 - c[0] - c[1] - c[2] - c[3]

max_err = 0
for i in range (0, 101) :
  x = i / 100
  tmp_err = abs(c[0] * x ** 9 + c[1] * x ** 7 + c[2] * x ** 5 + c[3] * x ** 3 + d * x - atan(x))
  if (tmp_err > max_err) :
    max_err = tmp_err
print("max error is %.6f\n"%(max_err))
print('a = %.6f, b = %.6f, c = %.6f, d = %.6f, e = %.6f\n'%(c[0],c[1],c[2],c[3],d))
print("**********************fit done*************************\n")

