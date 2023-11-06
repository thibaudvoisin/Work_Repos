int int_sqrt(int n)
{
    int tmp = 0;

    for (int i = n; i >= 0;)
    {
        i -= tmp++;
        i -= tmp;
    }
    return (tmp - 1);
}
