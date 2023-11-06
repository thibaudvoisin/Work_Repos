#include <stddef.h>

void plus_equal(int *a, int *b)
{
    if (a == NULL)
        a = b;
    else if (b != NULL)
        *a += *b;
}

void minus_equal(int *a, int *b)
{
    if (a == NULL)
        return;
    if (b != NULL)
        *a -= *b;
}

void mult_equal(int *a, int *b)
{
    if (a != NULL && b != NULL)
        *a *= *b;
}

int div_equal(int *a, int *b)
{
    if (a == NULL || b == NULL || (*b == 0))
        return 0;

    int tmp;
    {
        tmp = *a;
        *a /= *b;
    }
    return (tmp % *b);
}
