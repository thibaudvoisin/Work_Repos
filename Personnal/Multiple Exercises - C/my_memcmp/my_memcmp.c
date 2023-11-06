#include "my_memcmp.h"

int my_memcmp(const void *s1, const void *s2, size_t num)
{
    if (!num)
        return 0;
    unsigned int i = 0;
    const unsigned char *tmp1 = s1;
    const unsigned char *tmp2 = s2;

    for (; i < num; ++i)
    {
        char s1_i = *(tmp1 + i);
        char s2_i = *(tmp2 + i);

        if (s1_i > s2_i)
            return 1;
        if (s1_i < s2_i)
            return -1;
    }
    char s1_i = *(tmp1 + i);
    char s2_i = *(tmp2 + i);

    if (!s1_i)
    {
        if (!s2_i)
            return 0;
        else
            return -1;
    }
    else
        return 1;
}
/*
int main()
{
    return 1;
}*/
