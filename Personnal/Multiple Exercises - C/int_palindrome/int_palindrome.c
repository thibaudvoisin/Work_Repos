int int_palindrome(int n)
{
    int a = 0;
    int rest = 0;
    int b = 0;

    b = n;

    while (n != 0)
    {
        rest = n % 10;
        a = a * 10 + rest;
        n /= 10;
    }

    if (b == a)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}
