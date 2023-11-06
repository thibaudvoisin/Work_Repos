#include <err.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#include "heap.h"

int get_digit_nb(int num)
{
    int count = 0;
    while (num)
    {
        num /= 10;
        ++count;
    }
    return count;
}

int power_(int a, int b)
{
    int cpy = a;
    for (int i = 0; i < b - 2; ++i)
    {
        a *= cpy;
    }
    return a;
}

char *my_itoa(int value, char *s)
{
    int i = 0;
    int digit = get_digit_nb(value);

    if (value < 0)
    {
        value *= (-1);
        *(s + i) = '-';
        ++i;
    }
    while (value > 9)
    {
        *(s + i) = (value / (power_(10, digit))) + '0';
        value %= power_(10, digit);
        --digit;
        ++i;
    }
    *(s + i) = value + '0';
    *(s + i + 1) = 0;
    return s;
}

void p_int(int i)
{
    char *s = malloc(100);

    for (int i = 0; i < 100; ++i)
        s[i] = 0;

    s = my_itoa(i, s);
    for (int i = 0; s[i]; ++i)
        putchar(s[i]);
    free(s);
}

void print_rec(int *arr, int pos, int max, int a)
{
    if (pos < max)
    {
        if (a)
            putchar(' ');
        p_int(arr[pos]);
        print_rec(arr, pos * 2 + 1, max, 1);
        print_rec(arr, pos * 2 + 2, max, 1);
    }
}
void print_heap(const struct heap *heap)
{
    print_rec(heap->array, 0, heap->size, 0);
    putchar('\n');
}
