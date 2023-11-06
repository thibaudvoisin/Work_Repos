#include "utils/dict_fun.h"

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#include "utils/alloc.h"
#include "utils/ast.h"

struct dict_fun *dict_fun_add(struct dict_fun *dict, char *key, struct ast *ast)
{
    struct dict_fun *new = zalloc(sizeof(struct dict_fun));
    struct dict_fun *tmp = dict;
    new->key = key;
    new->ast = ast_copy(ast, true);
    if (!dict)
        return new;
    while (dict->next)
        dict = dict->next;
    new->next = NULL;
    dict->next = new;
    return tmp;
}

struct dict_fun *get_fun_value(struct dict_fun *dict, char *key)
{
    while (dict && strcmp(dict->key, key) != 0)
        dict = dict->next;
    if (!dict)
        return NULL;
    return dict;
}

struct dict_fun *dict_fun_change_value(struct dict_fun *dict, char *key,
                                       struct ast *ast)
{
    struct dict_fun *to_change = get_fun_value(dict, key);
    if (to_change)
    {
        free(key);
        ast_destroy(to_change->ast);
        to_change->ast = ast_copy(ast, true);
    }
    else
        dict = dict_fun_add(dict, key, ast);
    return dict;
}

struct dict_fun *dict_fun_remove(struct dict_fun *dict, char *key)
{
    if (!dict)
        return NULL;
    if (dict->key && key && strcmp(dict->key, key) == 0)
    {
        struct dict_fun *next = dict->next;
        free(dict->key);
        ast_destroy(dict->ast);
        free(dict);
        dict = next;
        return dict;
    }
    struct dict_fun *to_rm = dict;
    while (to_rm->next && strcmp(to_rm->next->key, key) != 0)
        to_rm = to_rm->next;
    if (to_rm->next)
    {
        ast_destroy(to_rm->next->ast);
        free(to_rm->next->key);
        struct dict_fun *next = to_rm->next->next;
        free(to_rm->next);
        to_rm->next = next;
    }
    return dict;
}

struct dict_fun *dict_fun_copy(struct dict_fun *dict)
{
    if (!dict)
        return NULL;
    struct dict_fun *copy = NULL;
    while (dict->next)
    {
        char *key = dict->key == NULL ? NULL : strdup(dict->key);
        copy = dict_fun_add(copy, key, dict->ast);
        dict = dict->next;
    }
    char *key = dict->key == NULL ? NULL : strdup(dict->key);
    copy = dict_fun_add(copy, key, dict->ast);
    return copy;
}

void dict_fun_destroy(struct dict_fun *dict)
{
    while (dict)
    {
        struct dict_fun *tmp = dict->next;
        free(dict->key);
        ast_destroy(dict->ast);
        free(dict);
        dict = tmp;
    }
}
