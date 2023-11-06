#include "my_atoi.h"

int check_val_plus(char char_, int value)
{
    value *= 10;
    value += ((value < 0) ? (-(char_ - '0')) : (char_ - '0'));
    return value;
}

int get_value(int *mode, int type_detected, char char_, int value)
{
    switch (*mode)
    {
    case 0:
        switch (type_detected)
        {
        case 2:
            *mode = 1;
            (char_ == '-') ? value *= (-1) : 0;
            break;
        case 3:
            *mode = 2;
            value *= (char_ - '0');
            break;
        }
        break;
    case 1:
        if (type_detected == 3)
        {
            (value *= (char_ - '0'));
            *mode = 2;
            break;
        }
        return 0;
    case 2:
        if (type_detected == 3)
            value = check_val_plus(char_, value);
        else
            return 0;
        break;
    }
    return value;
}

int my_atoi(const char *str)
{
    int mode = 0;
    int type_detected = 0;
    int value = 1;

    for (int i = 0; *(str + i) != 0; ++i)
    {
        switch (*(str + i))
        {
        case ' ':
            type_detected = 1;
            break;
        case '-':
            type_detected = 2;
            break;
        case '+':
            type_detected = 2;
            break;
        default:
            if (*(str + i) >= '0' && (*(str + i) <= '9'))
                type_detected = 3;
            else
                return 0;
            break;
        }
        value = get_value(&mode, type_detected, *(str + i), value);
        if (!value)
            return value;
    }
    return value;
}
