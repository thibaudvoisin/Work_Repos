#include <stddef.h>

int check_alphabet(const char *str, const char *alphabet)
{
    if (alphabet == NULL)
        return 1;

    int histo[128];
    int pos = 0;
    for (int i = 0; i < 128; ++i)
    {
        histo[i] = 0;
    }
    for (int i = 0; *(i + alphabet) != 0; ++i)
    {
        pos = *(alphabet + i);
        histo[pos] = 1;
    }
    for (int i = 0; *(str + i) != 0; ++i)
    {
        pos = *(str + i);
        histo[pos] = 0;
    }
    for (int i = 0; i < 128; ++i)
    {
        if (histo[i])
            return 0;
    }
    return 1;
}
