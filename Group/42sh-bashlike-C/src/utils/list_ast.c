#include "utils/list_ast.h"

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#include "utils/alloc.h"
#include "utils/ast.h"

struct list_ast *list_ast_add(struct list_ast *list, char *cond,
                              struct ast *ast)
{
    struct list_ast *new = zalloc(sizeof(struct list_ast));
    new->cond = cond;
    new->ast = ast_copy(ast, true);
    new->next = NULL;
    if (!list)
        return new;
    struct list_ast *tmp = list;
    while (tmp->next)
        tmp = tmp->next;
    tmp->next = new;
    return list;
}

struct list_ast *list_ast_remove(struct list_ast *list, char *cond)
{
    if (!list)
        return NULL;
    struct list_ast *tmp = list;
    if (strcmp(list->cond, cond) == 0)
    {
        struct list_ast *n = list->next;
        free(list->cond);
        ast_destroy(list->ast);
        free(list);
        list = n;
        return list;
    }
    while (tmp->next && strcmp(tmp->next->cond, cond) != 0)
        tmp = tmp->next;
    if (tmp->next)
    {
        struct list_ast *next = tmp->next->next;
        free(tmp->next->cond);
        ast_destroy(tmp->next->ast);
        free(tmp->next);
        tmp->next = next;
    }
    return list;
}

struct list_ast *list_ast_copy(struct list_ast *list)
{
    if (!list)
        return NULL;
    struct list_ast *copy = NULL;
    while (list->next)
    {
        char *cond = list->cond == NULL ? NULL : strdup(list->cond);
        copy = list_ast_add(copy, cond, list->ast);
        list = list->next;
    }
    char *cond = list->cond == NULL ? NULL : strdup(list->cond);
    copy = list_ast_add(copy, cond, list->ast);
    return copy;
}

int list_ast_in(struct list_ast *list, char *cond)
{
    while (list && strcmp(list->cond, cond) != 0)
        list = list->next;
    return list != NULL;
}

void list_ast_destroy(struct list_ast *list)
{
    while (list)
    {
        struct list_ast *tmp = list->next;
        free(list->cond);
        ast_destroy(list->ast);
        free(list);
        list = tmp;
    }
}
