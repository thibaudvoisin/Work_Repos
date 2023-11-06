#include "int_vector_bubble_sort.h"

static struct int_vector swap(struct int_vector vec, size_t i, size_t j)
{
    int tmp;
    tmp = vec.data[i];
    vec.data[i] = vec.data[j];
    vec.data[j] = tmp;
    return vec;
}

struct int_vector int_vector_bubble_sort(struct int_vector vec)
{
    for (unsigned int i = 0; i < vec.size; ++i)
    {
        for (unsigned int j = 0; j < (vec.size - 1); ++j)
        {
            if (vec.data[j] > vec.data[j + 1])
            {
                vec = swap(vec, j, j + 1);
            }
        }
    }
    return vec;
}
