void swap(int *tab, int pos_1, int pos_2)
{
    int tmp = *(tab + pos_1);

    *(tab + pos_1) = *(tab + pos_2);
    *(tab + pos_2) = tmp;
}

unsigned array_min(const int arr[], unsigned start, unsigned size)
{
    int min = arr[start];
    int min_pos = start;
    for (unsigned i = start; i < size; ++i)
    {
        if (arr[i] < min)
        {
            min = arr[i];
            min_pos = i;
        }
    }
    return min_pos;
}

void selection_sort(int arr[], unsigned size)
{
    int pos_min = 0;
    for (unsigned i = 0; i < size; ++i)
    {
        pos_min = array_min(arr, i, size);
        swap(arr, i, pos_min);
    }
}
