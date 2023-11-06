#include <stddef.h>

int top_of_the_hill(int tab[], size_t len)
{
    if (!len)
        return -1;

    for (unsigned int i = 0; i < len; ++i)
    {
        if (tab[i] < 0)
            return -1;
    }

    int max_1;
    int max_2;
    if (tab[0] > tab[1])
    {
        max_1 = tab[0];
        max_2 = tab[1];
    }
    else
    {
        max_2 = tab[0];
        max_1 = tab[1];
    }

    for (unsigned int i = 2; i < len; ++i)
    {
        if (tab[i] > max_1)
        {
            max_2 = max_1;
            max_1 = tab[i];
        }
        else if (tab[i] > max_2)
            max_2 = tab[i];
    }

    return max_2;
}
