#include "mat_mult.h"

void mat_mult(int **mat1, int **mat2, size_t *matrices_size, int **out)
{
    int a = matrices_size[0];
    int b = matrices_size[1];
    int c = matrices_size[2];

    for (int i = 0; i < a; ++i)
    {
        for (int j = 0; j < c; ++j)
        {
            int sum = 0;

            for (int k = 0; k < b; ++k)
                sum = sum + mat1[i][k] * mat2[k][j];

            out[i][j] = sum;
        }
    }
}
