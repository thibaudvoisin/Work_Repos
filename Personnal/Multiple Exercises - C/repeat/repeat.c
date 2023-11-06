#include <stdio.h>

int main(int argc, char **args)
{
    if (argc != 3)
        return 1;
    for (int i = 0; i < **(args + 2) - '0'; ++i)
    {
        puts(*(args + 1));
    }
    return 0;
}
