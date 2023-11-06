#include "my_memset.h"

void *my_memset(void *s, int c, size_t n)
{
    unsigned char c2 = c;
    if (!n)
        return s;
    unsigned char i = 0;
    unsigned char *tmp1 = s;
    for (; i < n; ++i)
    {
        unsigned char *s1_i = tmp1 + i;
        *s1_i = c2;
    }
    return s;
}
/*
int main()
{
    return 1;
}*/
