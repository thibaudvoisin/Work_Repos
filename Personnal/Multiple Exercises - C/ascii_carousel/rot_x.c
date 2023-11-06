#include <stddef.h>

void rot_x(char *s, int x)
{
    if (s == NULL)
        return;

    int stock = 0;
    for (int i = 0; *(s + i) != 0; ++i)
    {
        stock = *(s + i);
        if ((stock >= 'A') && (stock <= 'Z'))
        {
            stock += x;
            stock -= 'A';
            if (stock > 0)
                stock %= 26;
            else
            {
                while (stock < 0)
                    stock += 26;
            }
            stock += 'A';
        }
        else if ((stock >= 'a') && (stock <= 'z'))
        {
            stock += x;
            stock -= 'a';
            if (stock > 0)
                stock %= 26;
            else
            {
                while (stock < 0)
                    stock += 26;
            }
            stock += 'a';
        }
        *(s + i) = stock;
    }
}
