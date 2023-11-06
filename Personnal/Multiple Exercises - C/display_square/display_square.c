#include <stdio.h>

void display_square(int width)
{
    if (width > 0)
    {
        if (width % 2 == 0)
            ++width;

        for (int i = 0; i < width; ++i)
        {
            putchar('*');
        }

        if (width > 1)
            putchar('\n');

        for (int i = 0; i < (((width + 1) / 2) - 2); ++i)
        {
            putchar('*');
            for (int j = 0; j < width - 2; ++j)
                putchar(' ');
            putchar('*');
            putchar('\n');
        }

        for (int i = 0; i < width; ++i)
        {
            if (width > 1)
                putchar('*');
        }
        putchar('\n');
    }
}
