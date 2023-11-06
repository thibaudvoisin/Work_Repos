int abs_char(char c)
{
    if (c <= 'Z' && c >= 'A')
        return (c + 32);
    return c;
}

int my_strcasecmp(const char *s1, const char *s2)
{
    int i = 0;
    for (; (*(s1 + i) != 0) && (*(s2 + i) != 0); ++i)
    {
        if (abs_char(*(s1 + i)) > abs_char(*(s2 + i)))
            return 1;
        if (abs_char(*(s1 + i)) == abs_char(*(s2 + i)))
            continue;
        if (abs_char(*(s1 + i)) < abs_char(*(s2 + i)))
            return -1;
    }
    if (!*(s1 + i))
    {
        if (!*(s2 + i))
            return 0;
        else
            return -1;
    }
    else
        return 1;
}
