#include "my_memcpy.h"

void *my_memcpy(void *dest, const void *source, size_t num)
{
    if (!num)
        return dest;
    unsigned int i = 0;
    unsigned char *tmp1 = dest;
    const unsigned char *tmp2 = source;

    for (; i < num; ++i)
    {
        unsigned char *s1_i = tmp1 + i;
        const unsigned char *s2_i = tmp2 + i;
        *s1_i = *s2_i;
    }
    return dest;
}
/*
int main()
{
    return 1;
}*/
