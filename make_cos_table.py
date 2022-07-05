#!/bin/python

# Generates arrays of pre-calculated cosine values as literals for C.

from math import cos, asin, pi

def main(f, PRECISION, NAME, fun, range, end_val):
    f.write("double %s[] = {\n" % NAME)
    j = 0
    p = 0.0
    while True:
        f.write("{:.20f}, ".format(fun(p)))
        j += 1
        p += PRECISION
        if p > range:
            break
    if p == range + PRECISION :
        f.write("%.20f \n};\n"%(end_val))
        f.write("const int %s_size = %d;\n" % (NAME, j+1))
    else :
        f.write("%.20f, %.20f\n};\n"%(end_val, end_val))
        f.write("const int %s_size = %d;\n" % (NAME, j+2))


if __name__ == '__main__':
    main(open("costable_1.h", "w"), 1.0, "costable_1", cos, 2 * pi, 1.0)
    main(open("costable_0_1.h", "w"), 0.1, "costable_0_1", cos, 2 * pi, 1.0)
    main(open("costable_0_01.h", "w"), 0.01, "costable_0_01", cos, 2 * pi, 1.0)
    main(open("costable_0_001.h", "w"), 0.001, "costable_0_001", cos, 2 * pi, 1.0)
    main(open("costable_0_0001.h", "w"), 0.0001, "costable_0_0001", cos, 2 * pi, 1.0)

    main(open("asintable_1.h", "w"), 1.0, "asintable_1", asin, 1.0, pi / 2)
    main(open("asintable_0_1.h", "w"), 0.1, "asintable_0_1", asin, 1.0, pi / 2)
    main(open("asintable_0_01.h", "w"), 0.01, "asintable_0_01", asin, 1.0, pi / 2)
    main(open("asintable_0_001.h", "w"), 0.001, "asintable_0_001", asin, 1.0, pi / 2)
    main(open("asintable_0_0001.h", "w"), 0.0001, "asintable_0_0001", asin, 1.0, pi / 2)

