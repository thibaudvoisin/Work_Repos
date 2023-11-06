#include "complex_operations.h"

#include "complex.h"
#include "complex_print.h"

struct complex neg_complex(struct complex a)
{
    a.real *= (-1);
    a.img *= (-1);

    return a;
}

struct complex add_complex(struct complex a, struct complex b)
{
    a.real += b.real;
    a.img += b.img;

    return a;
}

struct complex sub_complex(struct complex a, struct complex b)
{
    a.real -= b.real;
    a.img -= b.img;

    return a;
}

struct complex mul_complex(struct complex a, struct complex b)
{
    float tmp = a.real;
    a.real = (a.real * b.real) - (a.img * b.img);
    a.img = (tmp * b.img) + (b.real * a.img);

    return a;
}

struct complex div_complex(struct complex a, struct complex b)
{
    float tmp = a.real;

    float numer_a = ((a.real * b.real) + (a.img * b.img));

    a.real = numer_a / ((b.real * b.real) + (b.img * b.img));

    float numer_b = ((a.img * b.real) - (tmp * b.img));

    a.img = numer_b / ((b.real * b.real) + (b.img * b.img));

    return a;
}
