#include <stddef.h>

int vmax(const int *ptr, size_t size)
{
    int max_1;
    int max_2;
    if (*ptr > *(ptr + 1))
    {
        max_1 = *ptr;
        max_2 = *(ptr + 1);
    }
    else
    {
        max_2 = *ptr;
        max_1 = *(ptr + 1);
    }

    for (unsigned int i = 2; i < size; ++i)
    {
        if (*(ptr + i) > max_1)
        {
            max_2 = max_1;
            max_1 = *(ptr + i);
        }
        else if (*(ptr + i) > max_2)
            max_2 = *(ptr + i);
    }

    return max_2;
}
