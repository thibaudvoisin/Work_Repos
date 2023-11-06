#include "connect4.h"

#include <stddef.h>

int ver_largeur(char *game[], size_t columns, size_t lines)
{
    int j_max = columns - 4;
    // 0 = default, no winner / 1 = player 1 win / 2 = player 2 win / 3 = both
    // win
    int winner = 0;
    for (unsigned int i = 0; i < lines; ++i)
    {
        for (int j = 0; j <= j_max; ++j)
        {
            if (*(game[i] + j) == *(game[i] + j + 1)
                && *(game[i] + j + 1) == *(game[i] + j + 2)
                && *(game[i] + j + 2) == *(game[i] + j + 3))
            {
                if ((*(game[i] + j) == 'X') && ((!winner) || (winner == 1)))
                    winner = 1;
                else if ((*(game[i] + j) == 'X') && (winner == 2))
                    winner = 3;
                else if ((*(game[i] + j) == 'O') && (!winner || winner == 2))
                    winner = 2;
                else if ((*(game[i] + j) == 'O') && (winner == 1))
                    winner = 3;
            }
        }
    }
    return winner;
}

int ver_hauteur(char *game[], size_t columns, size_t lines)
{
    int i_max = lines - 4;
    // 0 = default, no winner / 1 = player 1 win / 2 = player 2 win / 3 = both
    // win
    int winner = 0;
    for (unsigned int j = 0; j < columns; ++j)
    {
        for (int i = 0; i <= i_max; ++i)
        {
            if (*(game[i] + j) == *(game[i + 1] + j)
                && *(game[i + 1] + j) == *(game[i + 2] + j)
                && *(game[i + 2] + j) == *(game[i + 3] + j))
            {
                if ((*(game[i] + j) == 'X') && ((!winner) || (winner == 1)))
                    winner = 1;
                else if ((*(game[i] + j) == 'X') && (winner == 2))
                    winner = 3;
                else if ((*(game[i] + j) == 'O') && (!winner || winner == 2))
                    winner = 2;
                else if ((*(game[i] + j) == 'O') && (winner == 1))
                    winner = 3;
            }
        }
    }
    return winner;
}

int ver_diag_leftoright(char *game[], size_t columns, size_t lines)
{
    int i_max = lines - 4;
    int j_max = columns - 4;
    // 0 = default, no winner / 1 = player 1 win / 2 = player 2 win / 3 = both
    // win
    int winner = 0;
    for (int j = 0; j <= j_max; ++j)
    {
        for (int i = 0; i <= i_max; ++i)
        {
            if (*(game[i] + j) == *(game[i + 1] + j + 1)
                && *(game[i + 1] + j + 1) == *(game[i + 2] + j + 2)
                && *(game[i + 2] + j + 2) == *(game[i + 3] + j + 3))
            {
                if ((*(game[i] + j) == 'X') && ((!winner) || (winner == 1)))
                    winner = 1;
                else if ((*(game[i] + j) == 'X') && (winner == 2))
                    winner = 3;
                else if ((*(game[i] + j) == 'O') && (!winner || winner == 2))
                {
                    winner = 2;
                }
                else if ((*(game[i] + j) == 'O') && (winner == 1))
                    winner = 3;
            }
        }
    }

    return winner;
}

int ver_diag_rightoleft(char *game[], size_t columns, size_t lines)
{
    int i_max = lines - 4;
    // 0 = default, no winner / 1 = player 1 win / 2 = player 2 win / 3 = both
    // win
    int winner = 0;
    for (unsigned int j = columns - 1; j >= 3; --j)
    {
        for (int i = 0; i <= i_max; ++i)
        {
            if (*(game[i] + j) == *(game[i + 1] + j - 1)
                && *(game[i + 1] + j - 1) == *(game[i + 2] + j - 2)
                && *(game[i + 2] + j - 2) == *(game[i + 3] + j - 3))
            {
                if ((*(game[i] + j) == 'X') && (!winner || winner == 1))
                    winner = 1;
                else if ((*(game[i] + j) == 'X') && (winner == 2))
                    winner = 3;
                else if ((*(game[i] + j) == 'O') && (!winner || winner == 2))
                    winner = 2;
                else if ((*(game[i] + j) == 'O') && (winner == 1))
                    winner = 3;
            }
        }
    }
    return winner;
}

int connect4(char *game[], size_t columns, size_t lines)
{
    int res_larg = ver_largeur(game, columns, lines);
    int res_haut = ver_hauteur(game, columns, lines);
    int res_diag_1 = ver_diag_leftoright(game, columns, lines);
    int res_diag_2 = ver_diag_rightoleft(game, columns, lines);

    if (res_larg == 3 || res_haut == 3 || res_diag_1 == 3 || res_diag_2 == 3)
        return 0;

    if (res_larg == 1 || res_haut == 1 || res_diag_1 == 1 || res_diag_2 == 1)
    {
        if (res_larg == 2 || res_haut == 2 || res_diag_1 == 2
            || res_diag_2 == 2)
            return 0;
        return 1;
    }

    if (res_larg == 2 || res_haut == 2 || res_diag_1 == 2 || res_diag_2 == 2)
        return 2;
    return 0;
}
