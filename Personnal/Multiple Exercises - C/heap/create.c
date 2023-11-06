#include <err.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#include "heap.h"

struct heap *create_heap(void)
{
    struct heap *heap = malloc(sizeof(struct heap));
    heap->size = 0;
    heap->capacity = 8;
    int *array = malloc(8 * sizeof(int));
    heap->array = array;

    return heap;
}
