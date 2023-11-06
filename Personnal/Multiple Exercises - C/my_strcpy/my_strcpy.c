char *my_strcpy(char *dest, const char *source)
{
    int i = 0;
    for (; (*(source + i) != 0); ++i)
    {
        *(dest + i) = *(source + i);
    }
    *(dest + i) = 0;
    return dest;
}
