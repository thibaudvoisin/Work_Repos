unsigned int digit(int n, int k)
{
    if (n <= 0 || k <= 0)
        return 0;

    int a = 1;

    for (int j = 0; j < k; ++j)
        a *= 10;

    if (n < a)
        return 0;

    for (int i = 1; i < k; ++i)
        n /= 10;

    n %= ((n / 10) * 10);
    return n;
}
