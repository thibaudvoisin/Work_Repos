#include "utils/ast.h"

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

#include "utils/alloc.h"
#include "utils/list.h"
#include "utils/list_ast.h"

struct ast *ast_create(enum ast_type type, union ast_data data)
{
    struct ast *new = zalloc(sizeof(struct ast));
    new->type = type;
    new->data = data;
    return new;
}

static union ast_data ast_data_copy(enum ast_type type, union ast_data data)
{
    union ast_data data_copy;
    char **argv = NULL;
    size_t size = 0;
    size_t i = 0;
    switch (type)
    {
    case NODE_CMD:
        argv = calloc(1, sizeof(char *));
        argv[0] = NULL;
        size = 0;
        i = 0;
        while (data.ast_cmd.argv && data.ast_cmd.argv[i])
        {
            size++;
            argv = realloc(argv, (size + 1) * sizeof(char *));
            argv[i] = strdup(data.ast_cmd.argv[i]);
            argv[i + 1] = NULL;
            i++;
        }
        data_copy.ast_cmd.argv = argv;
        break;
    case NODE_IF:
        data_copy.ast_if.cond = ast_copy(data.ast_if.cond, true);
        data_copy.ast_if.then = ast_copy(data.ast_if.then, true);
        data_copy.ast_if.elif = ast_copy(data.ast_if.elif, true);
        data_copy.ast_if.else_ = ast_copy(data.ast_if.else_, true);
        break;
    case NODE_ELIF:
        data_copy.ast_elif.cond = ast_copy(data.ast_elif.cond, true);
        data_copy.ast_elif.then = ast_copy(data.ast_elif.then, true);
        break;
    case NODE_WHILE:
        data_copy.ast_while.cond = ast_copy(data.ast_while.cond, true);
        data_copy.ast_while.do_ = ast_copy(data.ast_while.do_, true);
        break;
    case NODE_UNTIL:
        data_copy.ast_until.cond = ast_copy(data.ast_until.cond, true);
        data_copy.ast_until.do_ = ast_copy(data.ast_until.do_, true);
        break;
    case NODE_FOR:
        data_copy.ast_for.var =
            data.ast_for.var ? strdup(data.ast_for.var) : NULL;
        data_copy.ast_for.words = list_copy(data.ast_for.words);
        data_copy.ast_for.do_ = ast_copy(data.ast_for.do_, true);
        break;
    case NODE_ASSIGN:
        data_copy.ast_assign.name =
            data.ast_assign.name ? strdup(data.ast_assign.name) : NULL;
        data_copy.ast_assign.value =
            data.ast_assign.value ? strdup(data.ast_assign.value) : NULL;
        break;
    case NODE_PIPE:
        data_copy.ast_pipe.left = ast_copy(data.ast_pipe.left, true);
        data_copy.ast_pipe.right = ast_copy(data.ast_pipe.right, true);
        break;
    case NODE_OR:
        data_copy.ast_or.left = ast_copy(data.ast_or.left, true);
        data_copy.ast_or.right = ast_copy(data.ast_or.right, true);
        break;
    case NODE_AND:
        data_copy.ast_and.left = ast_copy(data.ast_and.left, true);
        data_copy.ast_and.right = ast_copy(data.ast_and.right, true);
        break;
    case NODE_BLK:
        data_copy.ast_blk.ast = ast_copy(data.ast_blk.ast, true);
        break;
    case NODE_FUN:
        data_copy.ast_fun.name = strdup(data.ast_fun.name);
        data_copy.ast_fun.blk = ast_copy(data.ast_fun.blk, true);
        break;
    case NODE_SUB:
        data_copy.ast_sub.ast = ast_copy(data.ast_sub.ast, true);
        break;
    case NODE_CASE:
        data_copy.ast_case.var = strdup(data.ast_case.var);
        data_copy.ast_case.list_cond = list_ast_copy(data.ast_case.list_cond);
        break;
    }
    return data_copy;
}

static struct redir *redir_copy(struct redir *redir)
{
    struct redir *rd = calloc(1, sizeof(struct redir));
    rd->file_name = strdup(redir->file_name);
    rd->redir_type = redir->redir_type;
    rd->fd = redir->fd;
    return rd;
}

struct ast *ast_copy(struct ast *ast, bool next)
{
    if (!ast)
        return NULL;
    union ast_data data_copy = ast_data_copy(ast->type, ast->data);
    struct ast *copy = ast_create(ast->type, data_copy);
    if (ast->redir)
        copy->redir = redir_copy(ast->redir);
    else
        copy->redir = NULL;
    if (!next)
        copy->next = NULL;
    else
        copy->next = ast_copy(ast->next, true);
    copy->is_neg = ast->is_neg;
    copy->in_func = ast->in_func;
    return copy;
}

static void ast_data_destroy(enum ast_type type, union ast_data data)
{
    int i = 0;

    switch (type)
    {
    case NODE_CMD:
        i = 0;
        if (data.ast_cmd.argv)
        {
            while (data.ast_cmd.argv[i])
            {
                free(data.ast_cmd.argv[i]);
                i++;
            }
            free(data.ast_cmd.argv);
        }
        break;
    case NODE_IF:
        ast_destroy(data.ast_if.cond);
        ast_destroy(data.ast_if.then);
        ast_destroy(data.ast_if.elif);
        ast_destroy(data.ast_if.else_);
        break;
    case NODE_ELIF:
        ast_destroy(data.ast_elif.cond);
        ast_destroy(data.ast_elif.then);
        break;
    case NODE_WHILE:
        ast_destroy(data.ast_while.cond);
        ast_destroy(data.ast_while.do_);
        break;
    case NODE_UNTIL:
        ast_destroy(data.ast_until.cond);
        ast_destroy(data.ast_until.do_);
        break;
    case NODE_FOR:
        free(data.ast_for.var);
        list_destroy(data.ast_for.words);
        ast_destroy(data.ast_for.do_);
        break;
    case NODE_ASSIGN:
        free(data.ast_assign.name);
        free(data.ast_assign.value);
        break;
    case NODE_PIPE:
        ast_destroy(data.ast_pipe.left);
        ast_destroy(data.ast_pipe.right);
        break;
    case NODE_OR:
        ast_destroy(data.ast_or.left);
        ast_destroy(data.ast_or.right);
        break;
    case NODE_AND:
        ast_destroy(data.ast_and.left);
        ast_destroy(data.ast_and.right);
        break;
    case NODE_BLK:
        ast_destroy(data.ast_blk.ast);
        break;
    case NODE_FUN:
        free(data.ast_fun.name);
        ast_destroy(data.ast_fun.blk);
        break;
    case NODE_SUB:
        ast_destroy(data.ast_sub.ast);
        break;
    case NODE_CASE:
        free(data.ast_case.var);
        list_ast_destroy(data.ast_case.list_cond);
        break;
    }
}

void ast_destroy(struct ast *ast)
{
    while (ast)
    {
        if (ast->redir)
        {
            free(ast->redir->file_name);
            free(ast->redir);
        }
        struct ast *tmp = ast->next;
        ast_data_destroy(ast->type, ast->data);
        free(ast);
        ast = tmp;
    }
}
