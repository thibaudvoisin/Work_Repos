#include <stddef.h>

int *add_int_ptr(int *a, int *b)
{
    if ((a != NULL) && (b != NULL))
        *a += *b;
    return a;
}
