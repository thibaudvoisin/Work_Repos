#include "bst_static.h"

#include <stdlib.h>

struct bst *init(size_t capacity)
{
    struct bst *bst = malloc(sizeof(struct bst));
    bst->size = 0;
    bst->capacity = capacity;
    struct value **data = malloc(capacity * sizeof(struct value *));
    bst->data = data;
    for (unsigned int i = 0; i < capacity; ++i)
        bst->data[i] = NULL;
    return bst;
}

void add_p(struct bst *tree, int value, unsigned pos)
{
    if (pos >= tree->capacity)
    {
        tree->data =
            realloc(tree->data, 2 * tree->capacity * sizeof(struct value *));
        for (unsigned int i = tree->capacity - 1; i < (2 * tree->capacity); ++i)
            tree->data[i] = NULL;
        tree->capacity *= 2;
    }
    if (tree->data[pos] == NULL)
    {
        tree->data[pos] = malloc(sizeof(struct value));
        tree->data[pos]->val = value;
    }
    else if (tree->data[pos]->val > value)
        add_p(tree, value, pos * 2 + 1);
    else
        add_p(tree, value, pos * 2 + 2);
}

void add(struct bst *tree, int value)
{
    if (tree == NULL || tree->data[0] == NULL)
    {
        if (tree == NULL)
            tree = init(1);
        tree->data[0] = malloc(sizeof(struct value));
        tree->data[0]->val = value;
        tree->size = 1;
    }
    else
    {
        if (tree->size == tree->capacity)
        {
            tree->data = realloc(tree->data,
                                 2 * tree->capacity * sizeof(struct value *));
            for (unsigned int i = tree->capacity - 1; i < (2 * tree->capacity);
                 ++i)
                tree->data[i] = NULL;
            tree->capacity *= 2;
        }
        add_p(tree, value, 0);
        ++(tree->size);
    }
}

int search(struct bst *tree, int value)
{
    if (tree == NULL)
        return -1;
    int found = 1;
    for (unsigned int i = 0; i < tree->capacity; ++i)
    {
        if (tree->data[i] != NULL)
        {
            int pos = i;

            while (found)
            {
                if (found > 10000)
                    return -1;
                if (tree->data[pos] == NULL)
                    return -1;
                if (tree->data[pos]->val == value)
                    return pos;
                else if (tree->data[pos]->val <= value)
                    pos = pos * 2 + 2;
                else
                    pos = pos * 2 + 1;
            }
        }
    }
    return (-1);
}

void bst_free(struct bst *tree)
{
    for (unsigned int i = 0; i < tree->capacity; ++i)
        free(tree->data[i]);
    free(tree->data);
    free(tree);
}
