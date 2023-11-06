#include "my_memmove.h"

void *my_memmove(void *dest, const void *src, size_t n)
{
    if (!n)
        return dest;
    unsigned int i = 0;
    unsigned char *tmp1 = dest;
    const unsigned char *tmp2 = src;

    unsigned char tmpp[n];

    for (; i < n; ++i)
    {
        const unsigned char *s2_i = tmp2 + i;
        tmpp[i] = *s2_i;
    }

    for (unsigned int j = 0; j < n; ++j)
    {
        unsigned char *s1_j = tmp1 + j;
        *s1_j = tmpp[j];
    }

    return dest;
}
/*
int main()
{
    return 1;
}*/
