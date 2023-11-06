#include <stdlib.h>

void switch_k(char *s_2, int *index, int *len_cur_char, int *curr_char)
{
    *(s_2 + *index) = *len_cur_char + '0';
    *(s_2 + *index + 1) = *curr_char;
    *len_cur_char = 1;
    *index += 2;
}

char *fill(int size, const char *s)
{
    int curr_char = 0;
    int len_cur_char = 0;
    int index = 0;

    char *s_2 = calloc(size + 1, sizeof(char));

    for (int i = 0; *(s + i) != 0; ++i)
    {
        if (!len_cur_char && !curr_char)
        {
            curr_char = *(s + i);
            len_cur_char = 1;
        }
        else if (len_cur_char == 9)
        {
            switch_k(s_2, &index, &len_cur_char, &curr_char);
            curr_char = *(s + i);
        }
        else if (*(s + i) == curr_char)
        {
            ++len_cur_char;
        }
        else
        {
            switch_k(s_2, &index, &len_cur_char, &curr_char);
            curr_char = *(s + i);
        }
    }

    *(s_2 + index) = len_cur_char + '0';
    *(s_2 + index + 1) = curr_char;
    *(s_2 + index + 2) = 0;

    return s_2;
}

char *rle_encode(const char *s)
{
    int size = 0;
    int curr_char = 0;
    int len_cur_char = 0;

    for (int i = 0; *(s + i) != 0; ++i)
    {
        if (*(s + i) == curr_char)
            ++len_cur_char;
        else
        {
            if (len_cur_char > 9)
                size += (((len_cur_char / 9) + 1) * 2);
            else
            {
                if (len_cur_char)
                    size += 2;
            }
            len_cur_char = 1;
            curr_char = *(s + i);
        }
    }
    if (len_cur_char > 9)
        size += (((len_cur_char / 9) + 1) * 2);
    else
        size += 2;
    return fill(size, s);
}

char *rle_decode(const char *s)
{
    int len_s = 0;
    while (*(s + len_s))
        ++len_s;
    int index = 0;
    int len_tot = 0;

    for (int i = 0; i < len_s; i += 2)
    {
        len_tot += ((*(s + i)) - '0');
    }

    char *s_res = calloc(len_tot, sizeof(char));

    for (int i = 0; i < len_s; i += 2)
    {
        for (int j = 0; j < (*(s + i) - '0'); ++j)
        {
            *(s_res + index) = *(s + i + 1);
            ++index;
        }
    }
    return s_res;
}
