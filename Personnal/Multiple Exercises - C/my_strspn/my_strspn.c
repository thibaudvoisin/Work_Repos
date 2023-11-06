#include <stddef.h>

size_t my_strspn(const char *s, const char *accept)
{
    int hist[128];
    int len = 0;
    int max = 0;

    for (int i = 0; i < 128; ++i)
    {
        hist[i] = 0;
    }

    for (int i = 0; *(accept + i) != 0; ++i)
    {
        hist[0 + *(accept + i)] = 1;
    }

    for (int i = 0; *(s + i) != 0; ++i)
    {
        if (hist[0 + *(s + i)] && max == 0)
            ++len;
        else
        {
            max = len;
        }
    }
    if (len > max)
        return len;
    return max;
}
