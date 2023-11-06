#include <stddef.h>
#include <stdlib.h>

int *my_calloc(size_t size, int init)
{
    if (!size)
        return NULL;

    size_t res;

    if (__builtin_mul_overflow(size, sizeof(int), &res))
        return NULL;

    int *p = malloc(res);

    if (p == NULL)
        return p;

    for (unsigned int i = 0; i < size; ++i)
    {
        *(p + i) = init;
    }
    return p;
}
