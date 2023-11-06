#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

unsigned int strlenn(char *str)
{
    unsigned int len = 0;
    for (; str[len]; ++len)
    {
        continue;
    }
    return len;
}

void stdintail(unsigned int n)
{
    char *string = calloc(10000, 1);

    unsigned int len = read(STDIN_FILENO, string, 10000);

    int nb_l = 0;

    for (unsigned int i = 0; i < len; ++i)
    {
        if (string[i] == '\n' || string[i] == 0 || string[i] == EOF)
            ++nb_l;
    }

    char **list_string = calloc(nb_l + 10, sizeof(char *));

    int prev_pos = 0;
    int nb_p = 0;
    for (unsigned int i = 0; i < len; ++i)
    {
        if (string[i] == '\n' || string[i] == 0 || string[i] == EOF)
        {
            char *tmp = calloc(i - prev_pos + 10, 1);
            for (unsigned int j = prev_pos; j <= i; ++j)
                tmp[j - prev_pos] = string[j];
            prev_pos = i + 1;
            list_string[nb_p] = tmp;
            ++nb_p;
        }
    }
    int tmp_n = n;
    for (int i = 0; i < nb_l; ++i)
    {
        if ((i + 1) > (nb_l - tmp_n))
            write(1, list_string[i], strlenn(list_string[i]));
    }
    for (int i = 0; i < nb_l; ++i)
    {
        free(list_string[i]);
    }
    free(string);
    free(list_string);
}
