int get_digit_nb(int num)
{
    int count = 0;
    while (num)
    {
        num /= 10;
        ++count;
    }
    return count;
}

int power_(int a, int b)
{
    int cpy = a;
    for (int i = 0; i < b - 2; ++i)
    {
        a *= cpy;
    }
    return a;
}

char *my_itoa(int value, char *s)
{
    int i = 0;
    int digit = get_digit_nb(value);

    if (value < 0)
    {
        value *= (-1);
        *(s + i) = '-';
        ++i;
    }
    while (value > 9)
    {
        *(s + i) = (value / (power_(10, digit))) + '0';
        value %= power_(10, digit);
        --digit;
        ++i;
    }
    *(s + i) = value + '0';
    *(s + i + 1) = 0;
    return s;
}
