#include <stdio.h>

void freq_analysis(const char text[], const char table[])
{
    int histo[26];
    char histo_stock[26];
    for (int i = 0; i < 26; ++i)
    {
        histo[i] = 0;
        histo_stock[i] = 0;
    }

    for (int i = 0; text[i] != 0; ++i)
    {
        ++histo[text[i] - 'A'];
    }

    for (int i = 0; table[i] != 0; ++i)
    {
        int max = 0;
        int pos_max = 0;
        for (int j = 0; j < 26; ++j)
        {
            if (histo[j] > max)
            {
                max = histo[j];
                pos_max = j;
            }
        }
        histo[pos_max] = 0;
        histo_stock[pos_max] = table[i];
    }

    for (int i = 0; i < 26; ++i)
    {
        if (histo_stock[i] != 0)
            printf("%c %c\n", i + 'A', histo_stock[i]);
    }
}
