#include "null_terminated_arrays.h"

#include <stddef.h>

size_t arrlen2(const char **matrix)
{
    unsigned int i = 0;

    for (; matrix[i] != NULL; ++i)
        continue;
    return i;
}

size_t arrlen(const char ***matrix)
{
    unsigned int i = 0;

    for (; matrix[i] != NULL; ++i)
        continue;
    return i;
}

void reverse_matrix(const char ***matrix)
{
    if (matrix == NULL || *matrix == NULL)
        return;
    size_t matlen = arrlen(matrix);

    for (unsigned int i = 0; i < (matlen / 2); ++i)
    {
        const char **tmp = matrix[i];
        matrix[i] = matrix[matlen - 1 - i];
        matrix[matlen - i - 1] = tmp;
    }

    for (unsigned int i = 0; i < matlen; ++i)
    {
        unsigned int len_cur_arr = arrlen2(matrix[i]);
        for (unsigned int j = 0; j < (len_cur_arr / 2); ++j)
        {
            const char *tmp = matrix[i][j];
            matrix[i][j] = matrix[i][len_cur_arr - 1 - j];
            matrix[i][len_cur_arr - 1 - j] = tmp;
        }
    }
}
/*
#include <stdio.h>
#include <stdlib.h>
int main()
{
    char ***matrix = calloc(4, sizeof(char **));
    char **matrix1 = calloc(4, sizeof(char *));
    char **matrix2 = calloc(3, sizeof(char *));
    char **matrix3 = calloc(5, sizeof(char *));
    char *matrix11 = calloc(2, sizeof(char));
    char *matrix12 = calloc(2, sizeof(char));
    char *matrix13 = calloc(2, sizeof(char));
    char *matrix21 = calloc(2, sizeof(char));
    char *matrix22 = calloc(2, sizeof(char));
    char *matrix31 = calloc(2, sizeof(char));
    char *matrix32 = calloc(2, sizeof(char));
    char *matrix33 = calloc(2, sizeof(char));
    char *matrix34 = calloc(2, sizeof(char));

    matrix34[1] = 0;
    matrix34[0] = '9';
    matrix33[1] = 0;
    matrix33[0] = '8';
    matrix32[1] = 0;
    matrix32[0] = '7';
    matrix31[1] =0;
    matrix31[0] = '6';
    matrix22[1] = 0;
    matrix22[0] ='5';
    matrix21[1] = 0;
    matrix21[0] = '4';
    matrix13[1] =0;
    matrix13[0] = '3';
    matrix12[1] = 0;
    matrix12[0] ='2';
    matrix11[1] = 0;
    matrix11[0] = '1';


    matrix3[4] =  NULL;
    matrix3[3] = matrix34;
    matrix3[2] = matrix33;
    matrix3[1] = matrix32;
    matrix3[0] = matrix31;

    matrix2[2] = NULL;
    matrix2[1] = matrix22;
    matrix2[0] = matrix21;

    matrix1[3] = NULL;
    matrix1[2] = matrix13;
    matrix1[1] = matrix12;
    matrix1[0] = matrix11;

    matrix[3] = NULL;
    matrix[2] = matrix3;
    matrix[1] = matrix2;
    matrix[0] = matrix1;

    for (unsigned int i = 0; i < 4; ++i)
    {
        if (matrix[i] == NULL)
            printf("NULL\n");
        else
        {
            for (unsigned int j = 0; j < arrlen2(matrix[i]); ++j)
            {
                if (matrix[i][j] == NULL)
                    printf("NULL-");
                else
                {
                    printf("%s-", matrix[i][j]) ;
                }
            }
            printf("\n");
        }
    }


    reverse_matrix(matrix);
    for (unsigned int i = 0; i < 4; ++i)
    {
        if (matrix[i] == NULL)
            printf("NULL\n");
        else
        {
            for (unsigned int j = 0; j < arrlen2(matrix[i]); ++j)
            {
                if (matrix[i][j] == NULL)
                    printf("NULL-");
                else
                {
                    printf("%s-", matrix[i][j]) ;
                }
            }
            printf("\n");
        }
    }

}*/
