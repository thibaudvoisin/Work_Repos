#include <stddef.h>

void array_max_min(int tab[], size_t len, int *max, int *min)
{
    int max_ = 0;
    int min_ = 0;

    if ((len > 0) && (tab != NULL))
    {
        max_ = tab[0];
        min_ = tab[0];

        for (unsigned int i = 0; i < len; ++i)
        {
            if (tab[i] > max_)
                max_ = tab[i];
            else if (tab[i] < min_)
                min_ = tab[i];
        }

        *max = max_;
        *min = min_;
    }
}
