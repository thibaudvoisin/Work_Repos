#include "utils/list.h"

#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#include "utils/alloc.h"

struct list *list_add(struct list *list, char *data)
{
    struct list *new = zalloc(sizeof(struct list));
    new->data = data;
    new->next = NULL;
    if (!list)
        return new;
    struct list *tmp = list;
    while (tmp->next)
        tmp = tmp->next;
    tmp->next = new;
    return list;
}

char *list_pop(struct list **list)
{
    if (!(*list))
        return NULL;
    char *data = strdup((*list)->data);
    struct list *next = (*list)->next;
    free((*list)->data);
    free(*list);
    *list = next;
    return data;
}

struct list *list_remove(struct list *list, char *data)
{
    if (!list)
        return NULL;
    struct list *tmp = list;
    if (strcmp(list->data, data) == 0)
    {
        struct list *n = list->next;
        free(list->data);
        free(list);
        list = n;
        return list;
    }
    while (tmp->next && strcmp(tmp->next->data, data) != 0)
        tmp = tmp->next;
    if (tmp->next)
    {
        struct list *next = tmp->next->next;
        free(tmp->next->data);
        free(tmp->next);
        tmp->next = next;
    }
    return list;
}

struct list *list_copy(struct list *list)
{
    if (!list)
        return NULL;
    struct list *copy = NULL;
    while (list->next)
    {
        char *data = list->data == NULL ? NULL : strdup(list->data);
        copy = list_add(copy, data);
        list = list->next;
    }
    char *data = list->data == NULL ? NULL : strdup(list->data);
    copy = list_add(copy, data);
    return copy;
}

int list_in(struct list *list, char *data)
{
    while (list && strcmp(list->data, data) != 0)
        list = list->next;
    return list != NULL;
}

void list_destroy(struct list *list)
{
    while (list)
    {
        struct list *tmp = list->next;
        free(list->data);
        free(list);
        list = tmp;
    }
}
