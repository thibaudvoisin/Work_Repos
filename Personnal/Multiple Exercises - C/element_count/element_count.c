#include <stddef.h>

size_t element_count(int *begin, int *end)
{
    size_t len = 0;
    if ((begin != NULL) && (end != NULL))
    {
        for (int i = 0; (begin + i) < end; ++i)
            ++len;
        return len;
    }
    return -1;
}
