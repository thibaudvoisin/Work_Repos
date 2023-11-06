#include <stddef.h>

size_t my_strlen(const char *s)
{
    int i = 0;
    for (; *(s + i) != 0; ++i)
    {
        continue;
    }
    return i;
}
