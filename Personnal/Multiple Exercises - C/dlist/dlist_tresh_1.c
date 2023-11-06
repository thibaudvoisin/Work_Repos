#include <stdio.h>
#include <stdlib.h>

#include "dlist.h"

void dlist_swap_(struct dlist *list, int index_1, int index_2);

struct dlist *dlist_init(void)
{
    struct dlist *dlist = calloc(1, sizeof(struct dlist));

    if (dlist == NULL)
        return NULL;

    dlist->size = 0;
    dlist->head = NULL;
    dlist->tail = NULL;
    return dlist;
}

int dlist_push_front(struct dlist *list, int element)
{
    struct dlist_item *dlist_item = calloc(1, sizeof(struct dlist_item));

    if (dlist_item == NULL)
        return 0;

    dlist_item->data = element;
    dlist_item->prev = NULL;
    dlist_item->next = list->head;
    if (list->head != NULL)
        list->head->prev = dlist_item;
    list->head = dlist_item;
    if (list->tail == NULL)
        list->tail = dlist_item;
    ++(list->size);

    return 1;
}

void dlist_print_rec(const struct dlist_item *dlist_item)
{
    if (dlist_item != NULL)
    {
        printf("%d\n", dlist_item->data);
        dlist_print_rec(dlist_item->next);
    }
}

void dlist_print(const struct dlist *list)
{
    if (list->size > 0)
    {
        dlist_print_rec(list->head);
    }
}

int dlist_push_back(struct dlist *list, int element)
{
    struct dlist_item *dlist_item = calloc(1, sizeof(struct dlist_item));

    if (dlist_item == NULL)
        return 0;

    dlist_item->data = element;
    dlist_item->prev = list->tail;
    dlist_item->next = NULL;
    if (list->tail != NULL)
        list->tail->next = dlist_item;
    list->tail = dlist_item;
    if (list->head == NULL)
        list->head = dlist_item;
    ++(list->size);

    return 1;
}

size_t dlist_size(const struct dlist *list)
{
    return list->size;
}
