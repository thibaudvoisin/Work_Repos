#include "utils/dict.h"

#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#include "utils/alloc.h"

struct dict *dict_add(struct dict *dict, char *key, char *value)
{
    struct dict *new = zalloc(sizeof(struct dict));
    struct dict *tmp = dict;
    new->key = key;
    new->value = value;
    if (!dict)
        return new;
    while (dict->next)
        dict = dict->next;
    new->next = NULL;
    dict->next = new;
    return tmp;
}

struct dict *get_value(struct dict *dict, char *key)
{
    while (dict && strcmp(dict->key, key) != 0)
        dict = dict->next;
    if (!dict)
        return NULL;
    return dict;
}

struct dict *dict_change_value(struct dict *dict, char *key, char *value)
{
    struct dict *to_change = get_value(dict, key);
    if (to_change)
    {
        free(key);
        free(to_change->value);
        to_change->value = value;
    }
    else
        dict = dict_add(dict, key, value);
    return dict;
}

struct dict *dict_remove(struct dict *dict, char *key)
{
    if (!dict)
        return NULL;
    if (dict->key && key && strcmp(dict->key, key) == 0)
    {
        struct dict *next = dict->next;
        free(dict->key);
        free(dict->value);
        free(dict);
        dict = next;
        return dict;
    }
    struct dict *to_rm = dict;
    while (to_rm->next && strcmp(to_rm->next->key, key) != 0)
        to_rm = to_rm->next;
    if (to_rm->next)
    {
        free(to_rm->next->value);
        free(to_rm->next->key);
        struct dict *next = to_rm->next->next;
        free(to_rm->next);
        to_rm->next = next;
    }
    return dict;
}

struct dict *dict_copy(struct dict *dict)
{
    if (!dict)
        return NULL;
    struct dict *copy = NULL;
    while (dict->next)
    {
        char *key = dict->key == NULL ? NULL : strdup(dict->key);
        char *value = dict->value == NULL ? NULL : strdup(dict->value);
        copy = dict_add(copy, key, value);
        dict = dict->next;
    }
    char *key = dict->key == NULL ? NULL : strdup(dict->key);
    char *value = dict->value == NULL ? NULL : strdup(dict->value);
    copy = dict_add(copy, key, value);
    return copy;
}

void dict_destroy(struct dict *dict)
{
    while (dict)
    {
        struct dict *tmp = dict->next;
        free(dict->key);
        free(dict->value);
        free(dict);
        dict = tmp;
    }
}
