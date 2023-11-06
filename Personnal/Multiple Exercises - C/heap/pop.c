#include <assert.h>
#include <err.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#include "heap.h"

void swap(struct heap *heap, int fpos, int spos);
int parent_pos(int pos);

void rearrange(struct heap *heap, unsigned int index)
{
    unsigned int left = 2 * index + 1;
    unsigned int right = 2 * index + 2;
    unsigned int biggest;

    if ((left < heap->size) && (heap->array[left] > heap->array[index]))
        biggest = left;
    else
        biggest = index;

    if ((right < heap->size) && (heap->array[right] > heap->array[biggest]))
        biggest = right;

    if (biggest != index)
    {
        swap(heap, index, biggest);
        rearrange(heap, biggest);
    }
}

/*
Step 1 − Remove root node.
Step 2 − Move the last element of last level to root.
Step 3 − Compare the value of this child node with its parent.
Step 4 − If value of parent is less than child, then swap them.
Step 5 − Repeat step 3 & 4 until Heap property holds.
*/

int pop(struct heap *heap)
{
    assert(heap != NULL);
    assert(heap->size);
    assert(heap->capacity);

    int elem = heap->array[0];
    heap->array[0] = heap->array[heap->size - 1];

    --(heap->size);

    rearrange(heap, 0);
    return elem;
}
