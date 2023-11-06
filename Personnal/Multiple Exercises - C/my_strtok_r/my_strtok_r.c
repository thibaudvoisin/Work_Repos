#include <stddef.h>

char *my_strtok_bis(char *str, const char *delim, char **saveptr)
{
    if (*saveptr == NULL)
        return 0;
    if (str == NULL)
        str = *saveptr;
    int hist[128];

    for (int i = 0; i < 128; ++i)
        hist[i] = 0;

    for (int i = 0; *(delim + i); ++i)
        hist[*(delim + i) + 0] = 1;

    for (int i = 0; *(str + i) != 0; ++i)
    {
        if (hist[*(str + i) + 0])
        {
            *(str + i) = 0;
            *saveptr = str + i + 1;
            if (*saveptr == 0)
            {
                return my_strtok_bis(*saveptr, delim, saveptr);
            }
            return str;
        }
    }
    if (*str == 0 || str == NULL)
        return NULL;
    *saveptr = NULL;
    return str;
}

char *my_strtok_r(char *str, const char *delim, char **saveptr)
{
    int done = 0;
    if (*saveptr == NULL)
        return 0;
    if (str == NULL)
        str = *saveptr;

    char str_2[100000];

    int hist[128];

    for (int i = 0; i < 128; ++i)
        hist[i] = 0;

    int len_str = 0;
    for (int i = 0; *(delim + i); ++i)
        hist[*(delim + i) + 0] = 1;
    while (*(str + len_str) != 0)
        ++len_str;

    while (!done)
    {
        for (int i = 0; *(str + i); ++i)
        {
            str_2[i] = str[i];
        }
        for (int i = 0; i < len_str; ++i)
        {
            if (hist[*(str + i) + 0]
                && (hist[*(str + i) + 0] && hist[*(str + i + 1) + 0]))
            {
                for (int j = i; j <= len_str; ++j)
                    *(str + j) = *(str + j + 1);
                --len_str;
                *(str + len_str) = 0;
                i = 0;
            }
        }
        done = 1;
        for (int i = 0; i < len_str - 1; ++i)
        {
            if (str[i] != str_2[i]
                || (hist[str[i] + 0] && hist[str[i + 1] + 0]))
            {
                done = 0;
            }
        }
    }
    return my_strtok_bis(str, delim, saveptr);
}
