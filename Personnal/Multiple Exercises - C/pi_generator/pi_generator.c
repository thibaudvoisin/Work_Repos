double rec(int precision, double i)
{
    if (precision == i || !precision)
        return 1;
    return (1 + ((i / (i * 2 + 1)) * rec(precision, i + 1)));
}

double pi_generator(int precision)
{
    double res_rec = rec(precision, 1);
    return 2 * res_rec;
}
