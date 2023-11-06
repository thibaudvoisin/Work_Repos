#include <stdio.h>
#include <stdlib.h>

void sieve(int n)
{
    if (n > 2)
    {
        long cmp = 0;
        int *hist = calloc(n, sizeof(int));
        for (int i = 2; i < n; ++i)
        {
            if (!hist[i])
                ++cmp;
            if (i * i >= n)
                continue;

            if (!hist[i])
            {
                for (int j = 1; (i * j) < n; ++j)
                    hist[j * i] = 1;
            }
        }
        free(hist);
        printf("%ld\n", cmp);
    }
}
