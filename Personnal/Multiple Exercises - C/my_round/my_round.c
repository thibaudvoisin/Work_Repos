float abs_(float n)
{
    return (n >= 0) ? n : -n;
}

int my_round(float n)
{
    int arr = n;
    float diff = arr - n;

    if (abs_(diff) >= 0.5)
        return (n < 0) ? arr - 1 : arr + 1;

    return arr;
}
