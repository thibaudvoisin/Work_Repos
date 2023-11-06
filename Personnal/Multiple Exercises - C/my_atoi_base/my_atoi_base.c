int is_in_base(char c, const char *base)
{
    for (int i = 0; *(base + i) != 0; ++i)
    {
        if (*(base + i) == c)
        {
            return 1;
        }
    }
    return 0;
}

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

int my_atoi(const char *str, const char *base)
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
            if (is_in_base(*(str + i), base))
                type_detected = 3;
            else
                return 0;
            break;
        }
        value = get_value(&mode, type_detected, *(str + i), value);

        if (!value)
            return 0;
    }
    return 1;
}

int val(char c, int size_base, const char *base)
{
    for (int i = 0; i < size_base; ++i)
    {
        if (c == *(base + i))
            return i;
    }
    return -1;
}

int my_atoi_base_r(const char *str, const char *base)
{
    if (!str || !my_atoi(str, base))
        return 0;
    int base_len = 0;
    int str_len = 0;
    int power = 1;
    int num = 0;
    int neg = 0;
    int neg_dec = 0;
    if (*str == '-')
    {
        neg = 1;
        neg_dec = 1;
    }
    else if (*str == '+')
        neg_dec = 1;

    for (int i = 0; *(base + i) != 0; ++i)
        ++base_len;

    for (int i = 0; *(str + i); ++i)
        ++str_len;

    for (int i = str_len - 1; i >= neg_dec; --i)
    {
        num += val(str[i], base_len, base) * power;
        power = power * base_len;
    }
    if (neg)
        num *= (-1);
    return num;
}

int my_atoi_base(const char *str, const char *base)
{
    int decc = 0;
    for (int i = 0; *(str + i) != 0 && *(str + i) == ' '; ++i)
        ++decc;
    return my_atoi_base_r(str + decc, base);
}
