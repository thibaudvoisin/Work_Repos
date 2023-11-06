#include "vector.h"

#include <err.h>
#include <stdio.h>
#include <stdlib.h>

struct vector *vector_init(size_t n)
{
    struct vector *vector = malloc(sizeof(struct vector));
    vector->size = 0;
    vector->capacity = n;
    vector->data = malloc(n * sizeof(int));

    return vector;
}

void vector_destroy(struct vector *v)
{
    if (v == NULL)
        return;
    free(v->data);
    free(v);
}

struct vector *vector_resize(struct vector *v, size_t n)
{
    if (v != NULL && v->capacity != n)
    {
        v->data = realloc(v->data, n * sizeof(int));
        v->capacity = n;
        if (v->size >= n)
            v->size = n;
    }
    return v;
}

struct vector *vector_append(struct vector *v, int elt)
{
    if (v == NULL)
        return NULL;
    if (v->size == v->capacity)
    {
        v = vector_resize(v, 2 * v->capacity);
        if (v == NULL)
            return NULL;
    }
    v->data[v->size] = elt;
    ++(v->size);
    return v;
}

void vector_print(const struct vector *v)
{
    if (v == NULL)
        return;
    for (unsigned int i = 0; i < v->size; ++i)
    {
        if (i == (v->size - 1))
            printf("%d", v->data[i]);
        else
            printf("%d,", v->data[i]);
    }
    printf("\n");
}

struct vector *vector_reset(struct vector *v, size_t n)
{
    vector_destroy(v);
    return (vector_init(n));
}

struct vector *vector_insert(struct vector *v, size_t i, int elt)
{
    if (v == NULL)
        return NULL;
    if (i > v->capacity)
        return NULL;
    if (v->size == v->capacity)
    {
        //        printf("Im IN\n");
        v = vector_resize(v, 2 * v->capacity);
        if (v == NULL)
            return NULL;
    }
    for (unsigned int j = v->size; j > i; --j)
        v->data[j] = v->data[j - 1];
    v->data[i] = elt;
    (v->size)++;
    return v;
}

struct vector *vector_remove(struct vector *v, size_t i)
{
    if (v == NULL)
        return NULL;
    if (i > v->capacity)
        return NULL;
    for (unsigned int j = i; j < (v->size - 1); ++j)
        v->data[j] = v->data[j + 1];
    v->data[v->size - 1] = 0;
    (v->size)--;
    if ((v->size * 2) < v->capacity)
        return vector_resize(v, v->capacity / 2);
    return v;
}
