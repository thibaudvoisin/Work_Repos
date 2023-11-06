#include <stdio.h>

int pine(unsigned n)
{
    for (unsigned i = 0; i < n; ++i)
    {
        for (unsigned j = 0; j < n + i; ++j)
        {
            if (j < (n - (i + 1)))
                putchar(' ');
            else
                putchar('*');
        }
        putchar('\n');
    }

    for (unsigned i = 0; i < (n / 2); ++i)
    {
        for (unsigned j = 1; j < n; ++j)
            putchar(' ');
        putchar('*');
        putchar('\n');
    }

    return (n < 3) ? 1 : 0;
}
