#include <stdlib.h>
#include <time.h>

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

void fill(char *s, int n, int len)
{
    char *d = calloc(len + 1, sizeof(char));
    d = my_itoa(n, d);
    if (len == 2 && !d[1])
    {
        d[1] = d[0];
        d[0] = '0';
    }
    for (int i = 0; *(d + i) != 0; ++i)
        *(s + i) = *(d + i);

    free(d);
}
int check_fail(int *set_val, char char_)
{
    if (*set_val)
    {
        if (char_ != ';' && char_ != ':')
        {
            if (char_ != ',' && char_ != '-')
            {
                if (char_ != '_')
                    return 1;
            }
        }
    }
    return 0;
}

char *date_format(const char *format)
{
    int len = 0;
    for (; format[len]; ++len)
        continue;
    if (len != 5)
        return NULL;
    time_t t = time(NULL);
    struct tm tm = *gmtime(&t);
    int year = tm.tm_year + 1900;
    int month = tm.tm_mon + 1;
    int day = tm.tm_mday;
    char *res = calloc(11, sizeof(char));
    int index = 0;
    int set_val_ = 0;
    int *set_val = &set_val_;
    for (int i = 0; *(format + i) != 0; ++i)
    {
        if (check_fail(set_val, *(format + i)))
            return NULL;

        *set_val = 1;

        switch (*(format + i))
        {
        case 'D':
            fill(res + index, day, 2);
            index += 2;
            break;
        case 'M':
            fill(res + index, month, 2);
            index += 2;
            break;
        case 'Y':
            fill(res + index, year, 4);
            index += 4;
            break;
        default:
            *(res + index) = *(format + i);
            ++index;
            *set_val = 0;
            break;
        }
    }
    return res;
}
