# Cosine

Various methods of calculating cosine without math.h.

See the explanation and benchmarks at [http://austinhenley.com/blog/cosine.html](http://austinhenley.com/blog/cosine.html)

*For educational purposes only :)*

# BUILDING

```
clang -O3 -mavx benchmarks.c -o benchmarks -lm
./benchmarks -h
Usage: ./benchmarks [-na] [-nt] [-t <testname>]
   -na - Don't run accuracy tests
   -nt - Don't run speed tests.
   -t <testname> - Run a particular test instead of all tests.
   -p - Print all test names.
```

# TESTS

There are four types of tests:

* Taylor series with literal values
* Taylor series with running values
* Table lookup with clamp to nearest
* Table lookup with linear interpolation (LERP)

Table tests have the following properties

* cos_table_1 - a cosine table with 1 value per radian
* cos_table_0_1 - a cosine table with 10 values per radian
* cos_table_0_01 - a cosine table with 100 values per radian
* cos_table_0_001 - a cosine table with 1000 values per radian
* cos_table_0_0001 - a cosine table with 10,000 values per radian

```
cos_taylor_literal_4terms_naive
cos_taylor_literal_6terms_naive
cos_taylor_literal_6terms_2pi
cos_taylor_literal_6terms_pi
cos_taylor_literal_6terms
cos_taylor_literal_10terms
cos_taylor_running_6terms
cos_taylor_running_8terms
cos_taylor_running_10terms
cos_taylor_running_16terms
cos_table_1
cos_table_0_1
cos_table_0_01
cos_table_0_001
cos_table_0_0001
cos_table_1_LERP
cos_table_0_1_LERP
cos_table_0_01_LERP
cos_table_0_001_LERP
cos_table_0_0001_LERP
cos_math_h
***NEW***
fast_cosine
fast_sine
fast_acc_cosine
fast_acc_sine 
***NEW***
```

You can run a particular test by using the `-t` switch:

```
./benchmarks -t cos_table_1_LERP
ACCURACY
cos_table_1_LERP                    0.1147496616359112

TIME
cos_table_1_LERP                    1.3534999999999999
```

Or run all tests with no arguments.

1. macos arm64

```
clang -O3 -mavx benchmarks.c -o benchmarks -lm
./benchmarks                        
Cosine benchmark

ACCURACY
cos_taylor_literal_4terms_naive     0.0000247372721736
cos_taylor_literal_6terms_naive     0.0000000063214679
cos_taylor_literal_6terms_2pi       0.0000000063214679
cos_taylor_literal_6terms_pi        0.0000000063214679
cos_taylor_literal_6terms           0.0000000063214679
cos_taylor_literal_10terms          0.0000000000000004
cos_taylor_running_6terms           0.0000000063214679
cos_taylor_running_8terms           0.0000000000005261
cos_taylor_running_10terms          0.0000000000000004
cos_taylor_running_16terms          0.0000000000000004
cos_table_1                         0.4868840381728474
cos_table_0_1                       0.0499943500331001
cos_table_0_01                      0.0049999675472431
cos_table_0_001                     0.0004999999109268
cos_table_0_0001                    0.0000499999164148
cos_table_1_LERP                    0.1076522572415404
cos_table_0_1_LERP                  0.0012481779730024
cos_table_0_01_LERP                 0.0000124998177090
cos_table_0_001_LERP                0.0000001249999818
cos_table_0_0001_LERP               0.0000000012500000
cos_math_h                          0.0000000000000000
fast_cosine                         0.0010902926026138
fast_sine                           0.0010902926026137
fast_acc_cosine                  0.0000233416851561
fast_acc_cosine_v2               0.0000338486153860
fast_acc_sine                    0.0000233443382702

TIME
cos_taylor_literal_4terms_naive     0.2006480000000000
cos_taylor_literal_6terms_naive     0.3015650000000000
cos_taylor_literal_6terms_2pi       0.4629260000000000
cos_taylor_literal_6terms_pi        0.5711940000000000
cos_taylor_literal_6terms           0.4606070000000000
cos_taylor_literal_10terms          0.7248470000000000
cos_taylor_running_6terms           0.4103160000000000
cos_taylor_running_8terms           0.5273930000000000
cos_taylor_running_10terms          0.6415490000000000
cos_taylor_running_16terms          1.0646249999999999
cos_table_1                         0.1761700000000000
cos_table_0_1                       0.1847380000000000
cos_table_0_01                      0.1874380000000000
cos_table_0_001                     0.1876330000000000
cos_table_0_0001                    0.1888590000000000
cos_table_1_LERP                    0.2331610000000000
cos_table_0_1_LERP                  0.2458810000000000
cos_table_0_01_LERP                 0.2488140000000000
cos_table_0_001_LERP                0.2466950000000000
cos_table_0_0001_LERP               0.2466110000000000
cos_math_h                          0.3569660000000000
fast_cosine                         0.1416710000000000
fast_sine                           0.1353450000000000
fast_acc_cosine                  0.1396380000000000
fast_acc_cosine_v2               0.1488490000000000
fast_acc_sine                    0.1312620000000000


Done
```

2. Ubuntu18 g++-7

```
g++-7 -O3 -mavx benchmarks.c -o benchmarks -lm
ACCURACY
cos_taylor_literal_4terms_naive 0.0000247372721736
cos_taylor_literal_6terms_naive 0.0000000063214679
cos_taylor_literal_6terms_2pi 0.0000000063214679
cos_taylor_literal_6terms_pi 0.0000000063214679
cos_taylor_literal_6terms 0.0000000063214679
cos_taylor_literal_10terms 0.0000000000000004
cos_taylor_running_6terms 0.0000000063214679
cos_taylor_running_8terms 0.0000000000005261
cos_taylor_running_10terms 0.0000000000000004
cos_taylor_running_16terms 0.0000000000000004
cos_table_1 0.4868840381728474
cos_table_0_1 0.0499943500331001
cos_table_0_01 0.0049999675472431
cos_table_0_001 0.0004999999109268
cos_table_0_0001 0.0000499999164148
cos_table_1_LERP 0.1076522572415404
cos_table_0_1_LERP 0.0012481779730024
cos_table_0_01_LERP 0.0000124998177090
cos_table_0_001_LERP 0.0000001249999818
cos_table_0_0001_LERP 0.0000000012500000
cos_math_h 0.0000000000000000
fast_cosine 0.0010902926026138
fast_sine 0.0010902926026137
fast_acc_cosine 0.0000233416851561
fast_acc_sine 0.0000233443382702



TIME
cos_taylor_literal_4terms_naive 0.4052360000000000
cos_taylor_literal_6terms_naive 0.5750660000000000
cos_taylor_literal_6terms_2pi 1.2396400000000001
cos_taylor_literal_6terms_pi 1.4296800000000001
cos_taylor_literal_6terms 1.1930259999999999
cos_taylor_literal_10terms 1.5523950000000000
cos_taylor_running_6terms 0.9517200000000000
cos_taylor_running_8terms 1.1430320000000000
cos_taylor_running_10terms 1.3806490000000000
cos_taylor_running_16terms 2.0492330000000001
cos_table_1 0.3221170000000000
cos_table_0_1 0.3380720000000000
cos_table_0_01 0.3403360000000000
cos_table_0_001 0.3485470000000000
cos_table_0_0001 0.3479600000000000
cos_table_1_LERP 0.4599750000000000
cos_table_0_1_LERP 0.4492510000000000
cos_table_0_01_LERP 0.4504650000000000
cos_table_0_001_LERP 0.4588620000000000
cos_table_0_0001_LERP 0.4529580000000000
cos_math_h 0.9298620000000000
fast_cosine 0.2139590000000000
fast_sine 0.1922930000000000
fast_acc_cosine 0.2128010000000000
fast_acc_sine 0.1982350000000000

```

Your results will differ.
