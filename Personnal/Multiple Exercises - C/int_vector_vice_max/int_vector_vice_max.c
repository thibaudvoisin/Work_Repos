#include "int_vector_vice_max.h"

int int_vector_vice_max(const struct int_vector vec)
{
    int max_1;
    int max_2;
    if (vec.data[0] > vec.data[1])
    {
        max_1 = vec.data[0];
        max_2 = vec.data[1];
    }
    else
    {
        max_2 = vec.data[0];
        max_1 = vec.data[1];
    }

    for (unsigned int i = 2; i < vec.size; ++i)
    {
        if (vec.data[i] > max_1)
        {
            max_2 = max_1;
            max_1 = vec.data[i];
        }
        else if (vec.data[i] > max_2)
            max_2 = vec.data[i];
    }

    return max_2;
}
