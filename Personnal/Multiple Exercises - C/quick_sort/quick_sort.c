void swap(int *tab, int pos_1, int pos_2)
{
    int tmp = *(tab + pos_1);
    *(tab + pos_1) = *(tab + pos_2);
    *(tab + pos_2) = tmp;
}

int part(int *tab, int low_b, int high_b)
{
    int pivot = *(tab + high_b);

    int i = (low_b - 1);

    for (int j = low_b; j <= high_b - 1; ++j)
    {
        if (*(tab + j) < pivot)
        {
            ++i;
            swap(tab, i, j);
        }
    }
    swap(tab, i + 1, high_b);
    return i + 1;
}

void rec_(int *tab, int low_b, int high_b)
{
    if (low_b >= high_b)
        return;

    int tmp = part(tab, low_b, high_b);

    rec_(tab, low_b, tmp - 1);
    rec_(tab, tmp + 1, high_b);
}

void quicksort(int *tab, int len)
{
    if (tab && len > 1)
    {
        int low = 0;
        int high = len - 1;

        rec_(tab, low, high);
    }
}
