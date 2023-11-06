#ifndef DICT_H
#define DICT_H

struct dict
{
    char *key;
    char *value;
    struct dict *next;
};

struct dict *dict_add(struct dict *dict, char *key, char *value);

struct dict *get_value(struct dict *dict, char *key);

struct dict *dict_change_value(struct dict *dict, char *key, char *value);

struct dict *dict_remove(struct dict *dict, char *key);

struct dict *dict_copy(struct dict *dict);

void dict_destroy(struct dict *dict);

#endif /* !DICT_H */
