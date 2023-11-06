int my_pow(int a, int b)
{
    int res = 1;
    while (1)
    {
        if (b & 1)
            res *= a;
        b >>= 1;
        if (!b)
            break;
        a *= a;
    }
    return res;
}
