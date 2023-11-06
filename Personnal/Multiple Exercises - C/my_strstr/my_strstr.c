int my_strstr(const char *haystack, const char *needle)
{
    if (!needle)
        return 0;
    int len_needle = 0;
    int len_hay = 0;
    int j = 0;

    for (int i = 0; *(haystack + i) != 0; ++i)
        ++len_hay;

    for (int i = 0; *(needle + i) != 0; ++i)
        ++len_needle;

    for (int i = 0; i < (len_hay - len_needle + 1); ++i)
    {
        j = 0;
        while (*(haystack + i + j) == *(needle + j) && *(needle + j) != 0)
            ++j;
        if (!(*(needle + j)))
            return i;
    }
    return -1;
}
