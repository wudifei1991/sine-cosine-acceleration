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

```
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
fast_arracy_cosine                  0.0000233416851561
fast_arracy_sine                    0.0000233443382702

TIME
cos_taylor_literal_4terms_naive     0.1995320000000000
cos_taylor_literal_6terms_naive     0.3010010000000000
cos_taylor_literal_6terms_2pi       0.4646890000000000
cos_taylor_literal_6terms_pi        0.5706970000000000
cos_taylor_literal_6terms           0.4619100000000000
cos_taylor_literal_10terms          0.7223550000000000
cos_taylor_running_6terms           0.4091610000000000
cos_taylor_running_8terms           0.5246490000000000
cos_taylor_running_10terms          0.6409340000000000
cos_taylor_running_16terms          1.0557960000000000
cos_table_1                         0.1759670000000000
cos_table_0_1                       0.1839590000000000
cos_table_0_01                      0.1886830000000000
cos_table_0_001                     0.1878550000000000
cos_table_0_0001                    0.1873800000000000
cos_table_1_LERP                    0.2324810000000000
cos_table_0_1_LERP                  0.2444090000000000
cos_table_0_01_LERP                 0.2468160000000000
cos_table_0_001_LERP                0.2490870000000000
cos_table_0_0001_LERP               0.2519970000000000
cos_math_h                          0.3563780000000000
fast_cosine                         0.1366210000000000
fast_sine                           0.1332880000000000
fast_arracy_cosine                  0.1388320000000000
fast_arracy_sine                    0.1293180000000000


Done
```

Your results will differ.
