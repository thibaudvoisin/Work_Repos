#include <stdio.h>

int main(void)
{
    char tmp;
    for (int i = 0; i < 26; ++i)
    {
        tmp = 'a' + i;
        putchar(tmp);
        (i != 25) ? putchar(' ') : putchar('\n');
    }
    return 0;
}
