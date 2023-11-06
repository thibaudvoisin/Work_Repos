#include <stdio.h>

void rec(int n, int src, int des, int inter)
{
    if (n > 0)
    {
        rec(n - 1, src, inter, des);
        putchar(src + '0');
        putchar('-');
        putchar('>');
        putchar(des + '0');
        putchar('\n');
        rec(n - 1, inter, des, src);
    }
}

void hanoi(unsigned n)
{
    rec(n, 1, 3, 2);
}
