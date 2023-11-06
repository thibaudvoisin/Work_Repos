#ifndef LIST_AST_H
#define LIST_AST_H

#include "utils/ast.h"

struct list_ast
{
    char *cond;
    struct ast *ast;
    struct list_ast *next;
};

struct list_ast *list_ast_add(struct list_ast *list, char *cond,
                              struct ast *ast);

struct list_ast *list_ast_remove(struct list_ast *list, char *cond);

struct list_ast *list_ast_copy(struct list_ast *list);

int list_ast_in(struct list_ast *list, char *cond);

void list_ast_destroy(struct list_ast *list);

#endif /* !LIST_H */
