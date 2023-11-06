#ifndef DICT_FUNC_H
#define DICT_FUNC_H

#include "utils/ast.h"

struct dict_fun
{
    char *key;
    struct ast *ast;
    struct dict_fun *next;
};

struct glob_dict
{
    struct dict *dict_var;
    struct dict_fun *dict_fun;
};

struct dict_fun *dict_fun_add(struct dict_fun *dict, char *key,
                              struct ast *ast);

struct dict_fun *get_fun_value(struct dict_fun *dict, char *key);

struct dict_fun *dict_fun_change_value(struct dict_fun *dict, char *key,
                                       struct ast *ast);

struct dict_fun *dict_fun_remove(struct dict_fun *dict, char *key);

struct dict_fun *dict_fun_copy(struct dict_fun *dict);

void dict_fun_destroy(struct dict_fun *dict);

#endif /* DICT_FUNC_H */
