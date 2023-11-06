#ifndef LIST_H
#define LIST_H

struct list
{
    char *data;
    struct list *next;
};

struct list *list_add(struct list *list, char *data);

char *list_pop(struct list **list);

struct list *list_remove(struct list *list, char *data);

struct list *list_copy(struct list *list);

int list_in(struct list *list, char *data);

void list_destroy(struct list *list);

#endif /* !LIST_H */
