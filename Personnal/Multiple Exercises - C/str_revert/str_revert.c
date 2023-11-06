void str_revert(char str[])
{
    int len = 0;
    for (int i = 0; str[i] != 0; ++i)
        ++len;
    char tmp = 0;

    for (int i = 0; i < (len / 2); ++i)
    {
        tmp = str[i];
        str[i] = str[len - i - 1];
        str[len - i - 1] = tmp;
    }
}
