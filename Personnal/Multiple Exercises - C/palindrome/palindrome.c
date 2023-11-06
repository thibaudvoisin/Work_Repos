#include <stddef.h>

int palindrome(const char *s)
{
    if (s == NULL)
        return 0;

    int len = 0;

    for (int i = 0; *(s + i) != 0; ++i)
        ++len;

    int j = len - 1;
    for (int i = 0; i <= j; ++i)
    {
        while (1)
        {
            if (!(*(s + i) >= 'a' && *(s + i) <= 'z'))
            {
                if (!(*(s + i) >= 'A' && *(s + i) <= 'Z'))
                    ++i;
                else
                    break;
            }
            else
                break;
        }

        while (1)
        {
            if (!(*(s + j) >= 'a' && *(s + j) <= 'z'))
            {
                if (!(*(s + j) >= 'A' && *(s + j) <= 'Z'))
                    --j;
                else
                    break;
            }
            else
                break;
        }

        if (*(s + i) != *(s + j))
            return 0;
        --j;
    }
    return 1;
}
