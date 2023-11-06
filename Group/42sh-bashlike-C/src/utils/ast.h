#ifndef AST_H
#define AST_H

#include <stdbool.h>

#include "lexer/token/token.h"
#include "utils/list.h"
#include "utils/list_ast.h"

struct global
{
    struct ast *ast;
    struct dict *dict;
};

struct redir
{
    enum token_type redir_type;
    int fd;
    char *file_name;
};

// Define type of the ast node
enum ast_type
{
    NODE_CMD,
    NODE_IF,
    NODE_ELIF,
    NODE_WHILE,
    NODE_UNTIL,
    NODE_FOR,
    NODE_ASSIGN,
    NODE_PIPE,
    NODE_OR,
    NODE_AND,
    NODE_BLK,
    NODE_FUN,
    NODE_SUB,
    NODE_CASE
};

// ast node which old the command and its arguments
struct ast_cmd
{
    char **argv;
};

// ast node which old its condition and its execution if the condition is true
struct ast_if
{
    struct ast *cond;
    struct ast *then;
    struct ast *elif;
    struct ast *else_;
};

struct ast_elif
{
    struct ast *cond;
    struct ast *then;
};

struct ast_while
{
    struct ast *cond;
    struct ast *do_;
};

struct ast_until
{
    struct ast *cond;
    struct ast *do_;
};

struct ast_for
{
    char *var;
    struct list *words;
    struct ast *do_;
};

struct ast_assign
{
    char *name;
    char *value;
};

struct ast_pipe
{
    struct ast *left;
    struct ast *right;
};

struct ast_and
{
    struct ast *left;
    struct ast *right;
};

struct ast_or
{
    struct ast *left;
    struct ast *right;
};

struct ast_blk
{
    struct ast *ast;
};

struct ast_sub
{
    struct ast *ast;
};

struct ast_fun
{
    char *name;
    struct ast *blk;
};

struct ast_case
{
    char *var;
    struct list_ast *list_cond;
};

/*
 ** union type ast
 ** old all the type an ast can old
 */
union ast_data
{
    struct ast_cmd ast_cmd;
    struct ast_if ast_if;
    struct ast_elif ast_elif;
    struct ast_while ast_while;
    struct ast_until ast_until;
    struct ast_for ast_for;
    struct ast_assign ast_assign;
    struct ast_pipe ast_pipe;
    struct ast_or ast_or;
    struct ast_and ast_and;
    struct ast_blk ast_blk;
    struct ast_fun ast_fun;
    struct ast_sub ast_sub;
    struct ast_case ast_case;
};

/*
 ** ast main structure
 ** containing typem data and the next thing to execute
 */
struct ast
{
    int in_func;
    struct redir *redir;
    int is_neg;
    enum ast_type type;
    union ast_data data;
    struct ast *next;
};

// create an ast node
struct ast *ast_create(enum ast_type type, union ast_data data);

struct ast *ast_copy(struct ast *ast, bool next);

// destroy the ast
void ast_destroy(struct ast *ast);

#endif /* AST_H */
