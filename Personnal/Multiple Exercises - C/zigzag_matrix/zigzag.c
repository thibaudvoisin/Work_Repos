#include <stddef.h>
#include <stdlib.h>

void fill_dir_b_left(int *val, unsigned int n, int *mat, unsigned int *x,
                     unsigned int *y);

void fill_dir_t_right(int *val, unsigned int n, int *mat, unsigned int *x,
                      unsigned int *y)
{
    if (!mat[*x * n + *y])
    {
        mat[*x * n + *y] = *val;
        ++*val;
    }

    if (*x)
    {
        if (*y != n - 1)
        {
            --*x;
            ++*y;
            fill_dir_t_right(val, n, mat, x, y);
        }
    }
}

void fill_dir_b_left(int *val, unsigned int n, int *mat, unsigned int *x,
                     unsigned int *y)
{
    if (!mat[*x * n + *y])
    {
        mat[*x * n + *y] = *val;
        ++*val;
    }

    if (*y)
    {
        if (*x != n - 1)
        {
            ++*x;
            --*y;

            fill_dir_b_left(val, n, mat, x, y);
        }
    }
}

void verif(int *mat_2d, int *val, unsigned int n, unsigned int *x,
           unsigned int *y)
{
    if (!*x && (*y < n - 1))
    {
        if (!mat_2d[*x * n + *y])
        {
            mat_2d[*x * n + *y] = *val;
            ++*val;
        }
        ++*y;
        fill_dir_b_left(val, n, mat_2d, x, y);
    }

    else if (!*y && (*x < n - 1))
    {
        if (!mat_2d[*x * n + *y])
        {
            mat_2d[*x * n + *y] = *val;
            ++*val;
        }

        ++*x;
        fill_dir_t_right(val, n, mat_2d, x, y);
    }
    else if (*x >= n - 1)
    {
        if (!mat_2d[*x * n + *y])
        {
            mat_2d[*x * n + *y] = *val;
            ++*val;
        }

        ++*y;
        fill_dir_t_right(val, n, mat_2d, x, y);
    }
    else if (*y >= n - 1)
    {
        if (!mat_2d[*x * n + *y])
        {
            mat_2d[*x * n + *y] = *val;
            ++*val;
        }

        ++*x;
        fill_dir_b_left(val, n, mat_2d, x, y);
    }
}

int *zigzag(size_t n)
{
    if (!n)
        return NULL;
    int *mat_2d = calloc(n * n, sizeof(char *));

    unsigned int x_b = 0;
    unsigned int *x = &x_b;
    unsigned int y_b = 0;
    unsigned int *y = &y_b;
    int v_b = 0;
    int *val = &v_b;

    while ((*x != (n - 1)) || (*y != (n - 1)))
    {
        verif(mat_2d, val, n, x, y);
    }
    return mat_2d;
}
