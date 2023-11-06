#include "string_replace.h"

#include <stddef.h>
#include <stdlib.h>

char *string_replace(char c, const char *str, const char *pattern)
{
    if (str == NULL)
        return NULL;

    int len_patt = 0;

    int len = 0;
    int nb_c = 0;
    int supp_c = 0;

    for (int i = 0; *(str + i) != 0; ++i)
    {
        ++len;
        if (*(str + i) == c)
            ++nb_c;
    }
    char *a;
    if (pattern != NULL)
    {
        for (int i = 0; !supp_c && *(pattern + i); ++i)
            ++len_patt;
    }
    if (pattern == NULL)
    {
        a = calloc(len - nb_c + 1, sizeof(char));
        supp_c = 1;
    }
    else
        a = calloc(len + (len_patt - 1) * nb_c + 1, sizeof(char));
    for (int i = 0, j = 0; *(str + j) != 0; ++i, ++j)
    {
        if (*(str + j) == c)
        {
            if (supp_c)
                --i;
            else
            {
                for (int k = 0; k < len_patt; ++k)
                    *(a + i + k) = *(pattern + k);
                i += len_patt - 1;
            }
        }
        else
            *(a + i) = *(str + j);
    }
    return a;
}
