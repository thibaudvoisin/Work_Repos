#include <stddef.h>
#include <stdlib.h>

int min(int a, int b, int c)
{
    if (a < b)
    {
        if (c < a)
            return c;
        else
            return a;
    }
    if (c < b)
        return c;
    return b;
}

void free_all(char **D, int len_s1)
{
    for (int i = 0; i <= len_s1; ++i)
        free(D[i]);
    free(D);
}

size_t levenshtein(char *s1, char *s2)
{
    int len_s1 = 0;
    int len_s2 = 0;

    int weight = 0;

    while (s1[len_s1])
        ++len_s1;

    while (s2[len_s2])
        ++len_s2;

    char **D = malloc(sizeof(char *) * (len_s1 + 1));

    for (int i = 0; i <= len_s1; ++i)
    {
        D[i] = malloc(sizeof(char) * (len_s2 + 1));
    }
    for (int i = 0; i <= len_s1; ++i)
        D[i][0] = i;

    for (int j = 0; j <= len_s2; ++j)
        D[0][j] = j;

    for (int i = 1; i <= len_s1; ++i)
    {
        for (int j = 1; j <= len_s2; ++j)
        {
            if (s1[i - 1] == s2[j - 1])
                weight = 0;
            else
                weight = 1;
            D[i][j] =
                min(D[i - 1][j] + 1, D[i][j - 1] + 1, D[i - 1][j - 1] + weight);
        }
    }
    int res = D[len_s1][len_s2];
    free_all(D, len_s1);
    return res;
}
