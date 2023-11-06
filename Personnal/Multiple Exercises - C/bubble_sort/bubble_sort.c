#include <stddef.h>

void bubble_sort(int array[], size_t size)
{
    int tmp;
    for (unsigned int i = 0; i < size; ++i)
    {
        for (unsigned int j = 0; j < (size - 1); ++j)
        {
            if (array[j] > array[j + 1])
            {
                tmp = array[j];
                array[j] = array[j + 1];
                array[j + 1] = tmp;
                i = 0;
            }
        }
    }
}
