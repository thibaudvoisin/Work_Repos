#include "int_vector_hill.h"

int int_vector_hill(struct int_vector vec)
{
    unsigned int len = vec.size;
    if (!len)
        return -1;
    int index_max = len - 1;
    int mode = 0;
    int prem_ = -324443;
    int *prem = &prem_;

    for (unsigned int i = 0; i < len - 1; ++i)
    {
        if (vec.data[i] < 0 || vec.data[i + 1] < 0)
            return -1;

        if (!mode)
        {
            if (vec.data[i] != vec.data[i + 1])
            {
                if (vec.data[i] > *prem)
                    *prem = -324443;
                if (vec.data[i] > vec.data[i + 1])
                {
                    if (vec.data[i] > *prem)
                    {
                        ++mode;
                        index_max = i;
                    }
                    else
                        ++mode;
                }
                else
                {
                    if (vec.data[i] > *prem)
                    {
                        index_max = i + 1;
                    }
                }
            }
            else
            {
                if (*prem == -324443 && vec.data[i] != *prem)
                {
                    index_max = i;
                    *prem = vec.data[i];
                }
            }
        }
        if (mode && (vec.data[i] < vec.data[i + 1]))
        {
            return -1;
        }
    }
    return index_max;
}
