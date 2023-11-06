#include <err.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#include "heap.h"

void swap(struct heap *heap, int fpos, int spos)
{
    int tmp;
    tmp = heap->array[fpos];
    heap->array[fpos] = heap->array[spos];
    heap->array[spos] = tmp;
}

int parent_pos(int pos)
{
    return (pos - 1) / 2;
}
void add(struct heap *heap, int val)
{
    if (heap->capacity == (heap->size + 1))
    {
        heap->array = realloc(heap->array, (heap->capacity * 2) * sizeof(int));
        heap->capacity *= 2;
    }
    heap->array[heap->size] = val;

    int current = heap->size;
    while (heap->array[current] > heap->array[parent_pos(current)])
    {
        swap(heap, current, parent_pos(current));
        current = parent_pos(current);
    }
    (heap->size)++;
}
