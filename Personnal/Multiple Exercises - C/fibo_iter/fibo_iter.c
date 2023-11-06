unsigned long fibo_iter(unsigned long n)
{
    unsigned long U_0 = 0;
    unsigned long U_1 = 1;
    unsigned long U_n = 0;
    for (unsigned long i = 0; i < n; ++i)
    {
        U_0 = U_1;
        U_1 = U_n;
        U_n = U_0 + U_1;
    }
    return U_n;
}
