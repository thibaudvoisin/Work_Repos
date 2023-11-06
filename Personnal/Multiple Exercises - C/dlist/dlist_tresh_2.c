#include <stdio.h>
#include <stdlib.h>

#include "dlist.h"

int dlist_get(struct dlist *list, size_t index)
{
    if (list->size <= index)
        return -1;

    struct dlist_item *list_real = list->head;
    while (index)
    {
        list_real = list_real->next;
        --index;
    }
    return list_real->data;
}

int dlist_insert_at(struct dlist *list, int element, size_t index)
{
    if (list == NULL || list->size < index)
        return -1;

    if (index == list->size)
    {
        if (!dlist_push_back(list, element))
            return -1;
        return 1;
    }

    if (!(list->size) || !index)
    {
        if (!dlist_push_front(list, element))
            return -1;
        return 1;
    }

    --index;
    struct dlist_item *list_real = list->head;
    while (index)
    {
        list_real = list_real->next;
        --index;
    }
    struct dlist_item *new_item = calloc(1, sizeof(struct dlist_item));

    new_item->data = element;
    new_item->prev = list_real;
    new_item->next = list_real->next;
    list_real->next->prev = new_item;
    list_real->next = new_item;

    ++(list->size);
    return 1;
}
int dlist_find_rec(const struct dlist_item *list_r, int element)
{
    if (list_r != NULL)
    {
        if (list_r->data == element)
        {
            return 1;
        }
        int res = dlist_find_rec(list_r->next, element);
        if (!res)
            return 0;
        return res + 1;
    }
    return 0;
}

int dlist_find(const struct dlist *list, int element)
{
    return (dlist_find_rec(list->head, element) - 1);
}

int dlist_remove_at(struct dlist *list, size_t index)
{
    if (list->size <= index)
        return -1;

    struct dlist_item *list_real = list->head;
    int index_ = index;
    while (index_)
    {
        list_real = list_real->next;
        --index_;
    }
    if (index == list->size - 1)
    {
        if (list->size)
            list_real->prev->next = NULL;
        int value = list_real->data;
        list->tail = list_real->prev;
        if (!list->size)
            list->head = NULL;
        free(list_real);
        --list->size;
        return value;
    }
    if (!index)
    {
        if (list->size)
            list_real->next->prev = NULL;
        int value = list_real->data;
        list->head = list_real->next;
        if (!list->size)
            list->tail = NULL;
        free(list_real);
        --list->size;
        return value; // do head
    }
    list_real->next->prev = list_real->prev;
    list_real->prev->next = list_real->next;
    int value = list_real->data;
    free(list_real);
    --list->size;
    return value;
}
