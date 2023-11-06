#include "lca.h"

#include <stdlib.h>

int f_pos(int *values, int length, int val)
{
    for (int i = 0; i < length; ++i)
    {
        if (values[i] == val)
            return i;
    }
    return -1;
}

int lca(int *values, int length, int p, int q)
{
    char *bool_v = calloc(length, 1);

    int pos_p = f_pos(values, length, p);
    int pos_q = f_pos(values, length, q);

    if (pos_p == -1 || pos_q == -1)
        return 0;

    bool_v[pos_p] = 1;

    int i = pos_p;
    while (i > 0)
    {
        bool_v[i] = 1;
        i = (i - 1) / 2;
    }

    bool_v[0] = 1;

    i = pos_q;
    while (!bool_v[i])
        i = (i - 1) / 2;

    free(bool_v);
    return values[i];
}

/*
int main()
{
    int *array = calloc(9, sizeof(int));
    array[0] = 0;
    array[1] = 1;
    array[2] = 4;
    array[3] = 5;
    array[4] = 9;
    array[5] = 10;
    array[6] = 18;
    array[7] = 22;
    array[8] = 42;

    return (lca(array, 9, 22, 9));
}*/
