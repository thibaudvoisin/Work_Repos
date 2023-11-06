#include <stdio.h>

void sieve(int n)
{
    int hist[1000];

    for (int i = 0; i < 1000; ++i)
        hist[i] = 0;

    hist[2] = 2;

    for (int i = 2; i <= n; ++i)
    {
        for (int j = 3; j <= n; ++j)
        {
            if ((j % i) && (*(hist + j) != -1))
                *(hist + j) = j;
            else if (j != i)
                *(hist + j) = -1;
        }
    }

    for (int i = 2; i <= n; ++i)
    {
        if (*(hist + i) != -1)
            printf("%d\n", *(hist + i));
    }
}
