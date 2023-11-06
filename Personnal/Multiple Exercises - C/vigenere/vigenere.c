#include "vigenere.h"

int verif(const char *msg, int i)
{
    if ((*(msg + i)) >= 'A' && (*(msg + i)) <= 'Z')
        return 0;

    if ((*(msg + i)) >= 'a' && (*(msg + i)) <= 'z')
        return 0;

    return 1;
}

void cipher(const char *key, const char *msg, char *res)
{
    int len_key = 0;
    int msg_l = 1;
    int key_l = 1;
    int tmp;

    while (*(key + len_key) != 0)
        ++len_key;

    int j = 0;
    for (int i = 0; *(msg + i) != 0; ++i)
    {
        if (verif(msg, i))
        {
            *(res + i) = *(msg + i);
            continue;
        }
        msg_l = 1;
        key_l = 1;
        tmp = 0;
        if (j == len_key)
            j = 0;

        if ((*(msg + i) >= 'A') && (*(msg + i) <= 'Z'))
            msg_l = 0;

        if ((*(key + j) >= 'A') && (*(key + j) <= 'Z'))
            key_l = 0;

        tmp += (*(msg + i) - (msg_l * 32));
        tmp += ((*(key + j)) - (key_l * 32));
        tmp %= 26;
        *(res + i) = tmp + 'A';
        ++j;
    }
}

void decipher(const char *key, const char *msg, char *res)
{
    int len_key = 0;
    int msg_l = 1;
    int key_l = 1;
    int tmp;

    while (*(key + len_key) != 0)
        ++len_key;

    int j = 0;
    for (int i = 0; *(msg + i) != 0; ++i)
    {
        if (verif(msg, i))
        {
            *(res + i) = *(msg + i);
            continue;
        }

        msg_l = 1;
        key_l = 1;
        tmp = 0;
        (j == len_key) ? j = 0 : 0;

        ((*(msg + i) >= 'A') && (*(msg + i) <= 'Z')) ? msg_l = 0 : 0;

        ((*(key + j) >= 'A') && (*(key + j) <= 'Z')) ? key_l = 0 : 0;

        tmp += (*(msg + i) - (msg_l * 32));
        tmp -= ((*(key + j)) - (key_l * 32));
        tmp %= 26;
        while (tmp < 0)
            tmp += 26;
        *(res + i) = tmp + 'A';
        ++j;
    }
}
