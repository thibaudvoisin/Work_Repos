#include <stdio.h>
#include <stdlib.h>

#include "dlist.h"

void dlist_map_square(struct dlist *list)
{
    struct dlist_item *list_ = list->head;

    while (list_ != NULL)
    {
        list_->data *= list_->data;
        list_ = list_->next;
    }
}

void rec_swap(struct dlist_item *list_)
{
    struct dlist_item *tmp;
    if (list_ != NULL)
    {
        rec_swap(list_->next);
        tmp = list_->next;
        list_->next = list_->prev;
        list_->prev = tmp;
    }
}

void dlist_reverse(struct dlist *list)
{
    struct dlist_item *list_ = list->head;

    rec_swap(list_);
    struct dlist_item *tmp;
    tmp = list->head;
    list->head = list->tail;
    list->tail = tmp;
}

struct dlist *dlist_split_at(struct dlist *list, size_t index)
{
    struct dlist *list_res = dlist_init();

    struct dlist_item *list_item = list->head;

    unsigned int max = 0;

    while (max < index)
    {
        list_item = list_item->next;
        ++max;
    }

    list_res->size = (list->size - index);

    list->size -= (list->size - index);

    list_res->tail = list->tail;

    list->tail = list_item->prev;
    list->tail->next = NULL;
    list_res->head = list_item;
    list_res->head->prev = NULL;

    return list_res;
}

void dlist_concat(struct dlist *list1, struct dlist *list2)
{
    if (list2 != NULL && list2->head != NULL)
    {
        if (list1->head == NULL)
        {
            list1->head = list2->head;
            list1->tail = list2->tail;
            list1->size = list2->size;
            list2->head = NULL;
            list2->tail = NULL;
            list2->size = 0;
        }
        else
        {
            list1->tail->next = list2->head;
            list2->head->prev = list1->tail;
            list1->tail = list2->tail;
            list2->head = NULL;
            list2->tail = NULL;
            list1->size += list2->size;
            list2->size = 0;
        }
    }
}
