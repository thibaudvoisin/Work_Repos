#include "list.h"

#include <stdlib.h>
#include <string.h>

struct list *list_prepend(struct list *list, const void *value,
                          size_t data_size)
{
    if (value == NULL || !data_size)
        return NULL;
    struct list *list2 = calloc(1, sizeof(struct list));
    if (list2 == NULL)
        return NULL;
    list2->next = list;
    void *data = calloc(data_size, 1);
    if (data == NULL)
        return NULL;
    list2->data = memcpy(data, value, data_size);
    return list2;
}

size_t list_length(struct list *list)
{
    unsigned int len = 0;
    while (list != NULL)
    {
        ++len;
        list = list->next;
    }
    return len;
}

void list_destroy(struct list *list)
{
    if (list != NULL)
    {
        list_destroy(list->next);
        free(list->data);
        free(list);
    }
}
/*
#include <stdio.h>

int main()
{
    struct list *list = NULL;
    char *lol = calloc(3, 1);
    int *lol2 = calloc(8, sizeof(int));
    long *lol3 = calloc(45, sizeof(long));
    list = list_prepend(list, lol, 3);
    printf("%zu\n",list_length(list));
    list = list_prepend(list, lol2, 8*sizeof(int));
    printf("%zu\n",list_length(list));
    list = list_prepend(list, lol3, 45*sizeof(long));
    printf("%zu\n",list_length(list));
    list_destroy(list);
    free(lol);
    free(lol2);
    free(lol3);
}*/
