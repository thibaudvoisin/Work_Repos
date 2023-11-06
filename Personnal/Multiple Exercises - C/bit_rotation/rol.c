#include <stddef.h>

unsigned char rol(unsigned char value, unsigned char roll)
{
    for (unsigned i = 0; i < roll; ++i)
    {
        int last_bit = (value > 127) ? 1 : 0;
        value = value << 1;
        if (last_bit)
            value += 1;
    }
    return value;
}
