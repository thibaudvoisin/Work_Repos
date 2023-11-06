#include "fifo.h"

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

struct fifo *fifo_init(void)
{
    struct fifo *fifo_ = calloc(1, sizeof(struct fifo));

    fifo_->head = NULL;
    fifo_->tail = NULL;
    fifo_->size = 0;

    return fifo_;
}

size_t fifo_size(struct fifo *fifo)
{
    if (fifo != NULL)
        return fifo->size;
    return -1;
}
void fifo_push(struct fifo *fifo, int elt)
{
    struct list *list_insert = calloc(1, sizeof(struct list));
    list_insert->data = elt;
    list_insert->next = NULL;
    if (fifo == NULL)
    {
        fifo = fifo_init();
        fifo->tail = list_insert;
        fifo->head = list_insert;
        ++fifo->size;
    }
    else
    {
        if (fifo->head == NULL && fifo->tail == NULL)
        {
            fifo->tail = list_insert;
            fifo->head = list_insert;
            ++fifo->size;
        }
        else
        {
            list_insert->next = fifo->head;
            fifo->head = list_insert;
            ++fifo->size;
        }
    }
}

int fifo_head(struct fifo *fifo)
{
    if (fifo == NULL || fifo->head == NULL)
        return -1;
    return fifo->head->data;
}

void fifo_pop(struct fifo *fifo)
{
    if (fifo != NULL && fifo->head != NULL)
    {
        struct list *list_ = fifo->head;
        fifo->head = list_->next;
        free(list_);
    }
}

static void fifo_clear_rec(struct list *list)
{
    if (list != NULL)
    {
        fifo_clear_rec(list->next);
        free(list);
    }
}

void fifo_clear(struct fifo *fifo)
{
    if (fifo != NULL)
    {
        fifo_clear_rec(fifo->head);
        fifo->head = NULL;
        fifo->tail = NULL;
        fifo->size = 0;
    }
}

void fifo_print(const struct fifo *fifo)
{
    if ((fifo != NULL) && (fifo->head != NULL) && (fifo->tail != NULL))
    {
        struct list *list_tail = fifo->tail;
        struct list *list_head = fifo->head;
        while (list_head != list_tail)
        {
            printf("%d\n", list_head->data);
            list_head = list_head->next;
        }
        printf("%d\n", list_head->data);
    }
}

void fifo_destroy(struct fifo *fifo)
{
    if (fifo != NULL)
    {
        fifo_clear_rec(fifo->head);
        free(fifo);
    }
}
