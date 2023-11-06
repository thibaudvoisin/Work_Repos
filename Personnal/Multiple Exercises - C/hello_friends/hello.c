#include <stdio.h>

int main(int nb_args, char **args)
{
    if (nb_args == 1)
        printf("Hello World!\n");

    for (int i = 1; i < nb_args; ++i)
    {
        printf("Hello %s!\n", *(args + i));
    }
    return 0;
}
