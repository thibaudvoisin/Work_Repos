char *rec(int n, const char *base, char *s, int *index)
{
    int base_l = 0;

    for (int i = 0; *(base + i) != 0; ++i)
        ++base_l;

    if (n < base_l)
        (*(s + *index)) = *(base + n);
    else
    {
        rec(n / base_l, base, s, index);
        ++(*index);
        *(s + *index) = *(base + (n % base_l));
    }
    return s;
}

char *my_itoa_base(int n, char *s, const char *base)
{
    int index = 0;
    return rec(n, base, s, &index);
}
