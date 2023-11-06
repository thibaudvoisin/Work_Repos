#include "parser.h"

#include <err.h>
#include <signal.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>

#include "lexer/lexer.h"
#include "lexer/token/token.h"
#include "utils/alloc.h"
#include "utils/ast.h"
#include "utils/list.h"
#include "utils/list_ast.h"

struct global *global = NULL;

static struct ast *add_ast(struct ast *ast, struct ast *child)
{
    if (!ast)
    {
        ast = child;
        return ast;
    }
    else
    {
        struct ast *tmp = ast;
        while (tmp->next)
            tmp = tmp->next;
        tmp->next = child;
        return ast;
    }
}

static bool is_correct_cmd(struct ast *cmd)
{
    while (cmd && cmd->data.ast_cmd.argv)
        cmd = cmd->next;
    return cmd == NULL;
}

static struct redir *parse_redir(struct lexer *lexer)
{
    struct redir *redir = zalloc(sizeof(struct redir));

    struct token *token = get_next_token(&lexer);

    switch (token->type)
    {
    case (T_REDIR_INPUT):
        if (strlen(token->data) > 1)
            redir->fd = token->data[0] - '0';
        else
            redir->fd = 0;
        break;
    case (T_REDIR_OUTPUT):
        if ((strlen(token->data) > 2) || (token->data[0] != '>'))
            redir->fd = token->data[0] - '0';
        else
            redir->fd = 1;
        break;
    case (T_REDIR_APPEND_OUTPUT):
        if (strlen(token->data) > 2)
            redir->fd = token->data[0] - '0';
        else
            redir->fd = 1;
        break;
    case (T_REDIR_DUPLI_INPUT):
        if (strlen(token->data) > 2)
            redir->fd = token->data[0] - '0';
        else
            redir->fd = 0;
        break;
    case (T_REDIR_DUPLI_OUTPUT):
        if (strlen(token->data) > 2)
            redir->fd = token->data[0] - '0';
        else
            redir->fd = 1;
        break;
    case (T_REDIR_OPEN_FILE):
        if (strlen(token->data) > 2)
            redir->fd = token->data[0] - '0';
        else
            redir->fd = 0;
        break;
    default:
        errx(2, "ERROR FROM US, PARSING REDIR");
    }
    redir->redir_type = token->type;
    free_token(token);
    token = get_next_token(&lexer);

    if (token->type == T_SEP || token->type == T_EOF || token->type == T_ERROR)
        errx(2, "INVALID FILE NAME");

    char *tmp = strdup(token->data);
    free(redir->file_name);
    redir->file_name = tmp;
    free_token(token);
    return redir;
}

bool is_redir(struct token *token)
{
    if ((token->type > 16) && (token->type < 23))
        return true;
    return false;
}
int is_n_all_clos_par(char *str)
{
    for (unsigned i = 0; i < strlen(str); ++i)
    {
        if (str[i] == ')')
            return 0;
    }
    return 1;
}

static struct ast *parse_cmd(struct lexer *lexer, bool in_pipe)
{
    struct token *token = peek_lexer(lexer);
    struct ast *cmd = NULL;
    int went_in = 0;
    while ((!went_in) || token->type == T_WORD)
    {
        struct redir *redir = NULL;
        went_in = 1;
        free_token(token);
        token = peek_lexer(lexer);
        if (is_redir(token))
        {
            free_token(token);
            if (redir)
            {
                free(redir->file_name);
                free(redir);
            }
            redir = parse_redir(lexer);
        }
        else
            free_token(token);
        token = get_next_token(&lexer);

        union ast_data data;
        data.ast_cmd.argv = NULL;
        if (token->type != T_EOF)
        {
            size_t size = 2;
            data.ast_cmd.argv = zalloc(2 * sizeof(char *));
            data.ast_cmd.argv[1] = NULL;
            data.ast_cmd.argv[0] = strdup(token->data);
            free_token(token);
            token = peek_lexer(lexer);
            if (token->type != T_SEP && token->type != T_EOF
                && token->type != T_ERROR && token->type != T_PIPE
                && token->type != T_OR && token->type != T_AND)
            {
                free_token(token);

                token = peek_lexer(lexer);
                if (is_redir(token))
                {
                    free_token(token);
                    if (redir)
                    {
                        free(redir->file_name);
                        free(redir);
                    }
                    redir = parse_redir(lexer);
                }
                else
                    free_token(token);

                token = peek_lexer(lexer);
                if (is_n_all_clos_par(token->data))
                {
                    free_token(token);
                    token = get_next_token(&lexer);
                }
                while (token->type != T_SEP && token->type != T_EOF
                       && token->type != T_ERROR && token->type != T_PIPE
                       && token->type != T_AND && token->type != T_OR
                       && token->type != T_END_SUB && token->type != T_START_SUB
                       && token->type != T_SUB)
                {
                    data.ast_cmd.argv = xrealloc(data.ast_cmd.argv,
                                                 (size + 1) * sizeof(char *));
                    size++;

                    data.ast_cmd.argv[size - 1] = NULL;
                    data.ast_cmd.argv[size - 2] = strdup(token->data);

                    free_token(token);
                    token = peek_lexer(lexer);
                    if (token->type != T_SEP && token->type != T_EOF
                        && token->type != T_ERROR && token->type != T_PIPE
                        && token->type != T_AND && token->type != T_OR
                        && token->type != T_END_SUB
                        && token->type != T_START_SUB && token->type != T_SUB)
                    {
                        free_token(token);
                        token = peek_lexer(lexer);
                        if (is_redir(token))
                        {
                            free_token(token);
                            if (redir)
                            {
                                free(redir->file_name);
                                free(redir);
                            }
                            redir = parse_redir(lexer);
                        }
                        else
                            free_token(token);

                        token = get_next_token(&lexer);
                    }
                    else
                    {
                        free_token(token);
                        token = peek_lexer(lexer);
                    }
                }
                if (token->type == T_START_SUB || token->type == T_SUB)
                {
                    errx(2, "unexpected subshell in cmd");
                }
                if (token->type == T_END_SUB)
                {
                    unsigned io = 0;
                    for (; token->data[io] != ')' && io < strlen(token->data);
                         ++io)
                        continue;
                    if (io != 0)
                    {
                        data.ast_cmd.argv = xrealloc(
                            data.ast_cmd.argv, (size + 1) * sizeof(char *));

                        size++;
                        data.ast_cmd.argv[size - 1] = NULL;
                        char *buf = calloc(io + 1, 1);
                        buf = strncpy(buf, token->data, io);
                        data.ast_cmd.argv[size - 2] = buf;
                    }
                }

                free_token(token);
                token = NULL;
            }
        }
        struct ast *tmp = ast_create(NODE_CMD, data);
        tmp->redir = redir;
        cmd = add_ast(cmd, tmp);

        free_token(token);
        token = peek_lexer(lexer);
        if (token->type == T_SEP)
        {
            if (!in_pipe)
            {
                free_token(token);
                token = get_next_token(&lexer);
                free_token(token);
                token = peek_lexer(lexer);
            }
        }
    }
    free_token(token);
    return cmd;
}

static bool is_correct_elif(struct ast *ast)
{
    if (ast->type != NODE_ELIF)
        return false;
    struct ast_elif ast_elif = ast->data.ast_elif;
    if (ast_elif.cond != NULL && is_correct_cmd(ast_elif.cond))
    {
        if (ast_elif.then != NULL && is_correct_cmd(ast_elif.then))
            return true;
    }
    return false;
}

static bool is_correct_if(struct ast *ast)
{
    if (ast->type != NODE_IF)
        return false;
    struct ast_if ast_if = ast->data.ast_if;
    if (ast_if.cond != NULL && is_correct_cmd(ast_if.cond))
    {
        if (ast_if.then != NULL && is_correct_cmd(ast_if.then))
        {
            if (ast_if.elif != NULL)
            {
                if (!(is_correct_elif(ast_if.elif)))
                {
                    return false;
                }
            }
            if (ast_if.else_ != NULL)
            {
                if (is_correct_cmd(ast_if.else_))
                    return true;
                return false;
            }
            return true;
        }
    }
    return false;
}

static struct ast *parse_elif(struct lexer *lexer)
{
    struct token *token = peek_lexer(lexer);
    struct ast *elif = NULL;
    while (token->type == T_ELIF)
    {
        free_token(token);
        token = get_next_token(&lexer);
        union ast_data a_data;

        a_data.ast_elif.cond = parse_cond(lexer, 1, false);

        struct token *tok_expect_then = get_next_token(&lexer);

        if (tok_expect_then->type != T_THEN)
            errx(2, "Error while parsing elif");

        free_token(tok_expect_then);
        a_data.ast_elif.then = parse_cond(lexer, 2, false);

        struct ast *current_elif_ast = ast_create(NODE_ELIF, a_data);
        elif = add_ast(elif, current_elif_ast);

        free_token(token);
        token = peek_lexer(lexer);
    }
    free_token(token);
    return elif;
}

static struct ast *parse_if(struct lexer *lexer)
{
    struct token *token = get_next_token(&lexer);
    int went_in = 0;
    struct ast *if_tree = NULL;
    while (token->type == T_IF)
    {
        went_in = 1;
        union ast_data a_data;

        a_data.ast_if.cond = parse_cond(lexer, 1, false);

        free_token(token);
        token = get_next_token(&lexer);
        if (token->type != T_THEN)
        {
            errx(2, "Error while parsing if 222222");
        }
        a_data.ast_if.then = parse_cond(lexer, 2, false);
        struct token *tok_expect_elif = peek_lexer(lexer);
        if (tok_expect_elif->type == T_ELIF)
            a_data.ast_if.elif = parse_elif(lexer);
        else
            a_data.ast_if.elif = NULL;

        free_token(tok_expect_elif);
        free_token(token);
        token = peek_lexer(lexer);
        if (token->type == T_ELSE)
        {
            free_token(token);
            token = get_next_token(&lexer);
            a_data.ast_if.else_ = parse_cond(lexer, 2, false);
        }
        else
            a_data.ast_if.else_ = NULL;
        free_token(token);
        token = peek_lexer(lexer);
        if (token->type != T_END_SUB || strncmp(token->data, "fi", 2))
        {
            free_token(token);
            token = get_next_token(&lexer);
            if (token->type != T_FI && token->type != T_END_SUB)
            {
                errx(2, "NO FI");
            }
            free_token(token);
            token = peek_lexer(lexer);
        }

        struct redir *redir = NULL;
        if (token->type != T_EOF && token->type != T_END_SUB)
        {
            if (token->type == T_SEP || token->type == T_EOF)
            {
                free_token(token);
                token = get_next_token(&lexer);
            }
            else if (is_redir(token))
            {
                free_token(token);
                redir = parse_redir(lexer);
                token = peek_lexer(lexer);
            }
            else if (token->type != T_PIPE && token->type != T_OR
                     && token->type != T_AND)
                errx(2, "error in if");
        }
        struct ast *current_if_ast = ast_create(NODE_IF, a_data);
        current_if_ast->redir = redir;
        if_tree = add_ast(if_tree, current_if_ast);
    }
    free_token(token);
    if (went_in)
        return if_tree;
    else
        errx(2, "Error while parsing if");
}

struct ast *parse_while(struct lexer *lexer)
{
    struct token *token = get_next_token(&lexer);
    int went_in = 0;
    struct ast *while_tree = NULL;
    while (token->type == T_WHILE)
    {
        went_in = 1;
        union ast_data a_data;

        a_data.ast_while.cond = parse_cond(lexer, 3, false);

        free_token(token);
        token = get_next_token(&lexer);
        if (token->type != T_DO)
        {
            errx(2, "Error while parsing while 333333");
        }
        a_data.ast_while.do_ = parse_cond(lexer, 4, false);

        free_token(token);
        token = peek_lexer(lexer);
        if (token->type != T_END_SUB || strncmp(token->data, "done", 4))
        {
            free_token(token);
            token = get_next_token(&lexer);
            if (token->type != T_DONE && token->type != T_END_SUB)
            {
                errx(2, "NO DONE");
            }
            free_token(token);
            token = peek_lexer(lexer);
        }

        struct redir *redir = NULL;
        if (token->type != T_EOF && token->type != T_END_SUB)
        {
            if (token->type == T_SEP || token->type == T_EOF)
            {
                free_token(token);
                token = get_next_token(&lexer);
            }
            else if (is_redir(token))
            {
                free_token(token);
                redir = parse_redir(lexer);
                token = get_next_token(&lexer);
            }
            else if (token->type != T_PIPE && token->type != T_OR
                     && token->type != T_AND)
                errx(2, "error in if");
        }

        struct ast *current_while_tree = ast_create(NODE_WHILE, a_data);
        current_while_tree->redir = redir;
        while_tree = add_ast(while_tree, current_while_tree);
    }
    free_token(token);
    if (went_in)
        return while_tree;
    else
        errx(2, "Error while parsing while");
}

struct ast *parse_until(struct lexer *lexer)
{
    struct token *token = get_next_token(&lexer);
    int went_in = 0;
    struct ast *until_tree = NULL;
    while (token->type == T_UNTIL)
    {
        went_in = 1;
        union ast_data a_data;
        a_data.ast_until.cond = parse_cond(lexer, 3, false);

        free_token(token);
        token = get_next_token(&lexer);
        if (token->type != T_DO)
        {
            errx(2, "Error while parsing until 444444");
        }
        a_data.ast_until.do_ = parse_cond(lexer, 4, false);

        free_token(token);
        token = peek_lexer(lexer);
        if (token->type != T_END_SUB || strncmp(token->data, "done", 4))
        {
            free_token(token);
            token = get_next_token(&lexer);
            if (token->type != T_DONE && token->type != T_END_SUB)
            {
                errx(2, "NO DONE");
            }
            free_token(token);
            token = peek_lexer(lexer);
        }

        struct redir *redir = NULL;
        if (token->type != T_EOF && token->type != T_END_SUB)
        {
            if (token->type == T_SEP || token->type == T_EOF)
            {
                free_token(token);
                token = get_next_token(&lexer);
            }
            else if (is_redir(token))
            {
                free_token(token);
                redir = parse_redir(lexer);
                token = get_next_token(&lexer);
            }
            else if (token->type != T_PIPE && token->type != T_OR
                     && token->type != T_AND)
                errx(2, "error in if");
        }

        struct ast *current_until_tree = ast_create(NODE_UNTIL, a_data);
        current_until_tree->redir = redir;
        until_tree = add_ast(until_tree, current_until_tree);
    }
    free_token(token);
    if (went_in)
        return until_tree;
    else
        errx(2, "Error while parsing until");
}

struct ast *parse_for(struct lexer *lexer)
{
    struct token *token = get_next_token(&lexer);
    int went_in = 0;
    struct ast *for_tree = NULL;
    while (token->type == T_FOR)
    {
        went_in = 1;
        union ast_data a_data;

        free_token(token);
        token = get_next_token(&lexer);
        a_data.ast_for.var = strdup(token->data);
        free_token(token);
        token = get_next_token(&lexer);

        if (token->type != T_IN)
            errx(2, "Syntax Error: for: no in");

        a_data.ast_for.words = NULL;
        free_token(token);
        token = get_next_token(&lexer);

        while (token->type != T_SEP && token->type != T_EOF
               && token->type != T_ERROR && token->type != T_PIPE
               && token->type != T_OR && token->type != T_AND)
        {
            a_data.ast_for.words =
                list_add(a_data.ast_for.words, strdup(token->data));
            free_token(token);
            token = get_next_token(&lexer);
        }
        if (token->type == T_EOF || token->type == T_ERROR)
            errx(2, "Syntaxt Error: for: no end of word list");
        else if (token->type == T_PIPE || token->type == T_AND
                 || token->type == T_OR)
            errx(2, "Syntax Error: token unexpected in for");

        free_token(token);
        token = get_next_token(&lexer);
        if (token->type != T_DO)
            errx(2, "Syntax Error: exepcted do in for");

        a_data.ast_for.do_ = parse_cond(lexer, 4, false);

        free_token(token);
        token = peek_lexer(lexer);
        if (token->type != T_END_SUB || strncmp(token->data, "done", 4))
        {
            free_token(token);
            token = get_next_token(&lexer);
            if (token->type != T_DONE && token->type != T_END_SUB)
            {
                errx(2, "NO DONE");
            }
            free_token(token);
            token = peek_lexer(lexer);
        }

        struct redir *redir = NULL;
        if (token->type != T_EOF && token->type != T_END_SUB)
        {
            if (token->type == T_SEP || token->type == T_EOF)
            {
                free_token(token);
                token = get_next_token(&lexer);
            }
            else if (is_redir(token))
            {
                free_token(token);
                redir = parse_redir(lexer);
                token = get_next_token(&lexer);
            }
            else if (token->type != T_PIPE && token->type != T_OR
                     && token->type != T_AND)
                errx(2, "error in if");
        }

        struct ast *current_for_tree = ast_create(NODE_FOR, a_data);
        current_for_tree->redir = redir;
        for_tree = add_ast(for_tree, current_for_tree);
    }
    free_token(token);
    if (went_in)
        return for_tree;
    else
        errx(2, "Error while parsing for");
}

struct ast *parse_variable(struct lexer *lexer)
{
    struct token *token = get_next_token(&lexer);
    struct ast *ast_assign = NULL;
    struct token *peekle = peek_lexer(lexer);
    if (peekle->type == T_SEP || peekle->type == T_D_VAR
        || peekle->type == T_EOF || peekle->type == T_PIPE
        || peekle->type == T_AND || token->type == T_OR)
    {
        if (token->var == NULL)
            errx(2, "issue while parsing variable");

        char *real_value = NULL;
        if (strlen(token->data))
            real_value = strdup(token->data);
        char *dupp = strdup(token->var);
        union ast_data a_data;
        a_data.ast_assign.name = dupp;
        a_data.ast_assign.value = real_value;
        struct ast *current_ast_assign = ast_create(NODE_ASSIGN, a_data);
        ast_assign = add_ast(ast_assign, current_ast_assign);
    }

    free_token(token);
    token = peek_lexer(lexer);
    if (token->type != T_EOF && token->type != T_SEP && token->type != T_PIPE
        && token->type != T_OR && token->type != T_AND)
        errx(2, "error in syntax parse_var");
    else if (token->type != T_PIPE && token->type != T_OR
             && token->type != T_AND)
    {
        free_token(token);
        token = get_next_token(&lexer);
    }
    free_token(token);
    free_token(peekle);
    return ast_assign;
}

struct ast *get_last_ast(struct ast **ast)
{
    if (!(*ast))
        return NULL;
    if (!(*ast)->next)
    {
        struct ast *to_ret = *ast;
        *ast = NULL;
        return to_ret;
    }
    struct ast *tmp = *ast;
    while (tmp->next->next)
    {
        tmp = tmp->next;
    }
    struct ast *to_ret = tmp->next;
    tmp->next = NULL;
    return to_ret;
}

struct ast *parse_pipe(struct lexer *lexer, struct ast **ast_gb)
{
    struct token *token = get_next_token(&lexer);
    free_token(token);
    token = peek_lexer(lexer);
    if (token->type != T_WHILE && token->type != T_UNTIL && token->type != T_FOR
        && token->type != T_WORD && token->type != T_IF
        && token->type != T_D_VAR)
        errx(2, "Syntax Erro, wrong token after '|'");
    free_token(token);
    struct ast *last = get_last_ast(ast_gb);
    if (!last)
        errx(2, "Syntax Error, Pipe without left");
    struct ast *second = parse_cond(lexer, 5, true);
    if (!second)
        errx(2, "Syntax Error, Pipe without right");
    union ast_data pipe_data;
    pipe_data.ast_pipe.left = last;
    pipe_data.ast_pipe.right = second;
    struct ast *ast_pipe = ast_create(NODE_PIPE, pipe_data);
    return ast_pipe;
}

struct ast *parse_or(struct lexer *lexer, struct ast **ast_gb)
{
    struct token *token = get_next_token(&lexer);
    free_token(token);
    token = peek_lexer(lexer);
    if (token->type != T_WHILE && token->type != T_UNTIL && token->type != T_FOR
        && token->type != T_WORD && token->type != T_IF
        && token->type != T_D_VAR && token->type != T_NEG)
        errx(2, "Syntax Erro, wrong token after '||'");
    free_token(token);
    struct ast *last = get_last_ast(ast_gb);
    if (!last)
        errx(2, "Syntax Error, || without left");
    struct ast *second = parse_cond(lexer, 6, true);
    if (!second)
        errx(2, "Syntax Error, || without right");
    union ast_data or_data;
    or_data.ast_or.left = last;
    or_data.ast_or.right = second;
    struct ast *ast_or = ast_create(NODE_OR, or_data);
    return ast_or;
}

struct ast *parse_and(struct lexer *lexer, struct ast **ast_gb)
{
    struct token *token = get_next_token(&lexer);
    free_token(token);
    token = peek_lexer(lexer);
    if (token->type != T_WHILE && token->type != T_UNTIL && token->type != T_FOR
        && token->type != T_WORD && token->type != T_IF
        && token->type != T_D_VAR && token->type != T_NEG)
        errx(2, "Syntax Erro, wrong token after '&&'");
    free_token(token);
    struct ast *last = get_last_ast(ast_gb);
    if (!last)
        errx(2, "Syntax Error, && without left");
    struct ast *second = parse_cond(lexer, 6, true);
    if (!second)
        errx(2, "Syntax Error, && without right");
    union ast_data and_data;
    and_data.ast_and.left = last;
    and_data.ast_and.right = second;
    struct ast *ast_and = ast_create(NODE_AND, and_data);
    return ast_and;
}

struct ast *parse_blk(struct lexer *lexer)
{
    struct token *token = get_next_token(&lexer);
    free_token(token);
    token = peek_lexer(lexer);
    struct ast *ast_blk = NULL;
    union ast_data blk_data;
    if (token->type == T_E_BLK)
        errx(2, "Syntxa Error: blk: unexpected '}'");
    blk_data.ast_blk.ast = parse_cond(lexer, 7, false);
    free_token(token);
    token = get_next_token(&lexer);
    free_token(token);
    token = peek_lexer(lexer);
    if (token->type == T_SEP)
    {
        free_token(token);
        token = get_next_token(&lexer);
    }
    free_token(token);
    ast_blk = ast_create(NODE_BLK, blk_data);
    return ast_blk;
}

struct ast *parse_fun(struct lexer *lexer)
{
    union ast_data fun_data;
    struct ast *ast_fun = NULL;
    struct token *token = get_next_token(&lexer);
    char *name = NULL;
    if (strlen(token->data) > 2 && token->data[strlen(token->data) - 2] == '(')
    {
        name = strdup(token->data);
        name[strlen(name) - 2] = 0;
        free_token(token);
        token = peek_lexer(lexer);
        if (token->type == T_PARA)
            errx(2, "Syntax error bad '(' in fun");
    }
    else
    {
        name = strdup(token->data);
        free_token(token);
        token = get_next_token(&lexer);
        if (token->type != T_PARA)
            errx(2, "Syntax error: fun dec without para");
    }
    free_token(token);
    token = peek_lexer(lexer);
    if (token->type != T_B_BLK)
        errx(2, "Syntax error: func dec without blk");
    free_token(token);
    fun_data.ast_fun.name = name;
    fun_data.ast_fun.blk = parse_blk(lexer);
    ast_fun = ast_create(NODE_FUN, fun_data);
    return ast_fun;
}

int is_all_op(struct token *token)
{
    for (unsigned i = 0; i < strlen(token->data); ++i)
    {
        if (token->data[i] != '(')
            return 0;
    }
    return 1;
}

struct ast *parse_subshell(struct lexer *lexer)
{
    if (!lexer)
        return NULL;
    unsigned nb_par_opened = 0;
    unsigned nb_par_closed = 0;

    unsigned nb_curr_par_opened = 0;
    unsigned nb_curr_par_closed = 0;
    unsigned ik = 0;
    struct ast *ast = NULL;
    struct ast *sub_ast = NULL;

    struct token *token = peek_lexer(lexer);

    while (true)
    {
        switch (token->type)
        {
        case T_START_SUB:
            while (is_all_op(token))
            {
                free_token(token);
                token = get_next_token(&lexer);
                for (; nb_curr_par_opened < strlen(token->data)
                     && token->data[nb_curr_par_closed] == '(';
                     ++nb_curr_par_opened)
                    continue;

                free_token(token);
                token = peek_lexer(lexer);
                nb_par_opened += nb_curr_par_opened;
                nb_curr_par_opened = 0;
            }
            while (token->data[nb_curr_par_opened] == '(')
                ++nb_curr_par_opened;

            if (nb_curr_par_opened)
            {
                nb_par_opened += nb_curr_par_opened;
                lexer->pos -= (strlen(token->data) - nb_curr_par_opened);
                free_token(token);
                lexer->pos--;
                token = get_next_token(&lexer);
                free_token(token);
                token = peek_lexer(lexer);

                nb_curr_par_opened = 0;
            }
            continue;
        case T_END_SUB:
            if (ast != NULL)
            {
                free_token(token);
                token = get_next_token(&lexer);
                for (; token->data[ik] != ')'; ++ik)
                    continue;

                nb_par_closed += (strlen(token->data) - ik);
                free_token(token);
                token = peek_lexer(lexer);

                while (token->type == T_END_SUB)
                {
                    free_token(token);
                    token = get_next_token(&lexer);
                    nb_par_closed += strlen(token->data);
                    free_token(token);
                    token = peek_lexer(lexer);
                }

                if (nb_par_opened != nb_par_closed)
                {
                    errx(2, "1: bad number of parenthesis");
                }
                free_token(token);
                token = get_next_token(&lexer);
                if (token->type != T_EOF && token->type != T_SEP)
                    errx(2, "wasnt expecting that here");
                free_token(token);
                union ast_data data;
                data.ast_sub.ast = ast;
                sub_ast = ast_create(NODE_SUB, data);
            }
            else
            {
                for (; token->data[ik] != ')'; ++ik)
                    continue;

                nb_par_closed += (strlen(token->data) - ik);

                lexer->pos -= ik;
                strncpy(lexer->input + lexer->pos, token->data, ik);
                lexer->pos--;
                token = get_next_token(&lexer);
                free_token(token);

                ast = parse_cond(lexer, 8, 0);
                token = peek_lexer(lexer);

                while (token->type == T_END_SUB)
                {
                    free_token(token);
                    token = get_next_token(&lexer);
                    nb_par_closed += strlen(token->data);
                    free_token(token);
                    token = peek_lexer(lexer);
                }

                if (nb_par_opened != nb_par_closed)
                {
                    errx(2, "1: bad number of parenthesis");
                }
                union ast_data data2;
                data2.ast_sub.ast = ast;
                sub_ast = ast_create(NODE_SUB, data2);
            }
            return sub_ast;
        case T_SUB:
            for (; nb_curr_par_opened < strlen(token->data)
                 && token->data[nb_curr_par_opened] == '(';
                 ++nb_curr_par_opened)
                continue;

            for (unsigned nb_curr_par_closed_tmp = strlen(token->data) - 1;
                 nb_curr_par_closed_tmp > 0
                 && token->data[nb_curr_par_closed_tmp] == ')';
                 --nb_curr_par_closed_tmp)
                ++nb_curr_par_closed;

            if (nb_curr_par_opened != nb_curr_par_closed)
                errx(2, "2: bad number of parenthesis");

            if ((nb_curr_par_opened + nb_curr_par_closed)
                == strlen(token->data))
                errx(2, "unexpected ')'");

            lexer->pos -=
                (strlen(token->data) - nb_curr_par_opened - nb_curr_par_closed);
            lexer->pos--;
            strncpy(
                lexer->input + lexer->pos + 1, token->data + nb_curr_par_opened,
                strlen(token->data) - nb_curr_par_closed - nb_curr_par_opened);
            free_token(token);
            token = get_next_token(&lexer);
            free_token(token);
            token = peek_lexer(lexer);
            ast = parse_cond(lexer, 8, 0);
            union ast_data data2;
            data2.ast_sub.ast = ast;
            sub_ast = ast_create(NODE_SUB, data2);
            free_token(token);
            return sub_ast;
        default:
            free_token(token);
            ast = add_ast(ast, parse_cond(lexer, 8, 0));
            break;
        }
        token = peek_lexer(lexer);
    }
}

struct list_ast *parse_case_cond(struct lexer *lexer)
{
    struct list_ast *list_cond = NULL;
    struct token *token = get_next_token(&lexer);
    while (token->type != T_ESAC)
    {
        char *var = strndup(token->data, strlen(token->data) - 1);
        while (token->type == T_SEP && strcmp(token->data, "\n") == 0)
        {
            free_token(token);
            token = get_next_token(&lexer);
        }
        if (token->type != T_CASE_COND)
            errx(2, "parse_case_conde: token unexpected");
        struct ast *cond = parse_cond(lexer, 9, false);
        list_cond = list_ast_add(list_cond, var, cond);
        ast_destroy(cond);
        cond = NULL;
        free_token(token);
        token = get_next_token(&lexer);
        while (token->type == T_SEP && strcmp(token->data, "\n") == 0)
        {
            free_token(token);
            token = get_next_token(&lexer);
        }
        if (token->type == T_SEP)
        {
            free_token(token);
            token = get_next_token(&lexer);
            if (token->type == T_SEP)
            {
                free_token(token);
                token = get_next_token(&lexer);
            }
            else
                errx(2, ";; expacted after case end");
        }
        else if (token->type != T_ESAC)
            errx(2, "case: no ;  at end of condition");
    }
    free_token(token);
    return list_cond;
}

struct ast *parse_case(struct lexer *lexer)
{
    struct token *token = get_next_token(&lexer);
    free_token(token);
    struct ast *ast_case = NULL;
    union ast_data case_data;
    token = get_next_token(&lexer);
    case_data.ast_case.var = strdup(token->data);
    free_token(token);
    token = get_next_token(&lexer);
    if (token->type != T_IN)
        errx(2, "no in in case");
    free_token(token);
    token = peek_lexer(lexer);
    if (token->type == T_SEP)
        errx(2, "case: sep unexpected after in");
    case_data.ast_case.list_cond = parse_case_cond(lexer);
    free_token(token);
    token = get_next_token(&lexer);
    if (token->type != T_SEP && token->type != T_EOF)
        errx(2, "No Separator after esac");
    free_token(token);
    ast_case = ast_create(NODE_CASE, case_data);
    return ast_case;
}

struct ast *parse_cond(struct lexer *lexer, int parse_code, bool in_pipe)
{
    if (!lexer)
        return NULL;
    struct ast *cond_tree = NULL;
    struct token *token = NULL;
    int is_neg = 0;
    while (true)
    {
        free_token(token);
        token = peek_lexer(lexer);
        if (parse_code == 1 && token->type == T_THEN)
            break;
        else if (token->type == T_END_SUB)
        {
            if (!(strncmp(token->data, "then", 4)))
                break;
        }

        if (parse_code == 2
            && (!(token->type != T_FI && token->type != T_ELIF
                  && token->type != T_ELSE)))
            break;
        else if (token->type == T_END_SUB)
        {
            if (!(strncmp(token->data, "fi", 2)))
                break;
            if (!(strncmp(token->data, "elif", 4)))
                break;
            if (!(strncmp(token->data, "else", 4)))
                break;
        }

        if (parse_code == 3 && token->type == T_DO)
            break;
        else if (token->type == T_END_SUB)
        {
            if (!(strncmp(token->data, "do", 2)))
                break;
        }

        if (parse_code == 4 && token->type == T_DONE)
            break;
        else if (token->type == T_END_SUB)
        {
            if (!(strncmp(token->data, "done", 4)))
                break;
        }

        if (parse_code == 5
            && (token->type == T_PIPE || token->type == T_SEP
                || token->type == T_EOF || token->type == T_AND
                || token->type == T_OR))
        {
            if (token->type != T_PIPE && token->type != T_AND
                && token->type != T_OR)
            {
                free_token(token);
                token = get_next_token(&lexer);
            }
            break;
        }
        else if (token->type == T_END_SUB)
        {
            if (!(strncmp(token->data, "pipe", 4)))
                break;
            if (!(strncmp(token->data, ";", 1)))
                break;
            if (!(strncmp(token->data, "&&", 2)))
                break;
            if (!(strncmp(token->data, "||", 2)))
                break;
        }

        if (parse_code == 6
            && (token->type == T_OR || token->type == T_AND
                || token->type == T_SEP || token->type == T_EOF
                || token->type == T_PIPE))
        {
            if (token->type != T_OR && token->type != T_AND
                && token->type != T_PIPE)
            {
                free_token(token);
                token = get_next_token(&lexer);
            }
            if (token->type != T_PIPE)
                break;
        }
        else if (token->type == T_END_SUB)
        {
            if (!(strncmp(token->data, "pipe", 4)))
                break;
            if (!(strncmp(token->data, ";", 1)))
                break;
            if (!(strncmp(token->data, "&&", 2)))
                break;
            if (!(strncmp(token->data, "||", 2)))
                break;
        }

        if (parse_code == 7 && token->type == T_E_BLK)
            break;
        else if (token->type == T_END_SUB)
        {
            if (!(strncmp(token->data, "{", 1)))
                break;
        }

        if (parse_code == 8
            && (token->type == T_END_SUB || token->type == T_EOF))
            break;
        if (parse_code == 9 && (token->type == T_SEP || token->type == T_ESAC))
            break;

        free_token(token);
        token = peek_lexer(lexer);

        struct ast *ast_if = NULL;
        struct ast *ast_cmd = NULL;
        struct ast *ast_while = NULL;
        struct ast *ast_until = NULL;
        struct ast *ast_for = NULL;
        struct ast *ast_assign = NULL;
        struct ast *ast_pipe = NULL;
        struct ast *ast_and = NULL;
        struct ast *ast_or = NULL;
        struct ast *ast_blk = NULL;
        struct ast *ast_fun = NULL;
        struct ast *ast_sub = NULL;
        struct ast *ast_case = NULL;
        switch (token->type)
        {
        case T_NEG:
            if (is_neg)
                errx(2, "two neg next to each others");
            is_neg = 1;
            free_token(token);
            token = get_next_token(&lexer);
            break;

        case T_IF:
            ast_if = parse_if(lexer);
            if (is_correct_if(ast_if))
            {
                ast_if->is_neg = is_neg;
                cond_tree = add_ast(cond_tree, ast_if);
            }
            else
                errx(2, "Syntaxhere Error.");
            break;
        case T_WHILE:
            ast_while = parse_while(lexer);
            ast_while->is_neg = is_neg;
            cond_tree = add_ast(cond_tree, ast_while);
            break;
        case T_UNTIL:
            ast_until = parse_until(lexer);
            ast_until->is_neg = is_neg;
            cond_tree = add_ast(cond_tree, ast_until);
            break;
        case T_FOR:
            ast_for = parse_for(lexer);
            ast_for->is_neg = is_neg;
            cond_tree = add_ast(cond_tree, ast_for);
            break;
        case T_WORD:
            ast_cmd = parse_cmd(lexer, in_pipe);
            if (is_correct_cmd(ast_cmd))
            {
                ast_cmd->is_neg = is_neg;
                cond_tree = add_ast(cond_tree, ast_cmd);
            }
            else
                errx(2, "Syntaxal Error.");
            break;
        case T_END_SUB:
            errx(2, "Not expected then");
        case T_START_SUB:
        case T_SUB:
            ast_sub = parse_subshell(lexer);
            cond_tree = add_ast(cond_tree, ast_sub);
            break;
        case T_D_VAR:
            ast_assign = parse_variable(lexer);
            ast_assign->is_neg = is_neg;
            cond_tree = add_ast(cond_tree, ast_assign);
            break;
        case T_EOF:
            errx(2, "No00 then");
        case T_PIPE:
            ast_pipe = parse_pipe(lexer, &cond_tree);
            ast_pipe->is_neg = is_neg;
            cond_tree = add_ast(cond_tree, ast_pipe);
            break;
        case T_AND:
            ast_and = parse_and(lexer, &cond_tree);
            ast_and->is_neg = is_neg;
            cond_tree = add_ast(cond_tree, ast_and);
            break;
        case T_OR:
            ast_or = parse_or(lexer, &cond_tree);
            ast_or->is_neg = is_neg;
            cond_tree = add_ast(cond_tree, ast_or);
            break;
        case T_B_BLK:
            ast_blk = parse_blk(lexer);
            ast_blk->is_neg = is_neg;
            cond_tree = add_ast(cond_tree, ast_blk);
            break;
        case T_FUNC:
            ast_fun = parse_fun(lexer);
            ast_fun->is_neg = is_neg;
            cond_tree = add_ast(cond_tree, ast_fun);
            break;
        case T_CASE:
            ast_case = parse_case(lexer);
            ast_case->is_neg = is_neg;
            cond_tree = add_ast(cond_tree, ast_case);
            break;
        default:
            errx(2, "Syntaxi Error.");
        }
    }
    free_token(token);
    return cond_tree;
}

struct global *parse_input(struct lexer *lexer)
{
    if (!lexer)
        return NULL;
    global = calloc(1, sizeof(struct global));
    struct token *token = NULL;
    int is_neg = 0;
    while (true)
    {
        free_token(token);
        token = peek_lexer(lexer);
        struct ast *ast_if = NULL;
        struct ast *ast_cmd = NULL;

        struct ast *ast_while = NULL;
        struct ast *ast_until = NULL;
        struct ast *ast_for = NULL;
        struct ast *ast_assign = NULL;
        struct ast *ast_pipe = NULL;
        struct ast *ast_or = NULL;
        struct ast *ast_and = NULL;
        struct ast *ast_blk = NULL;
        struct ast *ast_fun = NULL;
        struct ast *ast_sub = NULL;
        struct ast *ast_case = NULL;
        switch (token->type)
        {
        case T_NEG:
            if (is_neg)
                errx(2, "two neg next to each others");
            is_neg = 1;
            free_token(token);
            token = get_next_token(&lexer);
            break;
        case T_IF:
            ast_if = parse_if(lexer);
            if (is_correct_if(ast_if))
            {
                ast_if->is_neg = is_neg;
                global->ast = add_ast(global->ast, ast_if);
                is_neg = 0;
            }
            else
                errx(2, "Siiyntaxhere Error.");
            break;
        case T_WHILE: {
            ast_while = parse_while(lexer);
            ast_while->is_neg = is_neg;
            global->ast = add_ast(global->ast, ast_while);
            is_neg = 0;
        }
        break;
        case T_UNTIL: {
            ast_until = parse_until(lexer);
            ast_until->is_neg = is_neg;
            global->ast = add_ast(global->ast, ast_until);
            is_neg = 0;
        }
        break;
        case T_FOR: {
            ast_for = parse_for(lexer);
            ast_for->is_neg = is_neg;
            global->ast = add_ast(global->ast, ast_for);
            is_neg = 0;
        }
        break;
        case T_WORD:
            ast_cmd = parse_cmd(lexer, false);
            if (is_correct_cmd(ast_cmd))
            {
                ast_cmd->is_neg = is_neg;
                global->ast = add_ast(global->ast, ast_cmd);
                is_neg = 0;
            }
            else
                errx(2, "Syntaxiii Error.");
            break;
        case T_D_VAR:
            ast_assign = parse_variable(lexer);
            ast_assign->is_neg = is_neg;
            is_neg = 0;
            global->ast = add_ast(global->ast, ast_assign);
            break;
        case T_EOF:
            free_token(token);
            return global;
        case T_PIPE:
            ast_pipe = parse_pipe(lexer, &(global->ast));
            ast_pipe->is_neg = is_neg;
            is_neg = 0;
            global->ast = add_ast(global->ast, ast_pipe);
            break;
        case T_AND:
            ast_and = parse_and(lexer, &(global->ast));
            ast_and->is_neg = is_neg;
            is_neg = 0;
            global->ast = add_ast(global->ast, ast_and);
            break;
        case T_OR:
            ast_or = parse_or(lexer, &(global->ast));
            ast_or->is_neg = 0;
            global->ast = add_ast(global->ast, ast_or);
            break;
        case T_B_BLK:
            ast_blk = parse_blk(lexer);
            ast_blk->is_neg = is_neg;
            is_neg = 0;
            global->ast = add_ast(global->ast, ast_blk);
            break;
        case T_FUNC:
            ast_fun = parse_fun(lexer);
            ast_fun->is_neg = is_neg;
            is_neg = 0;
            global->ast = add_ast(global->ast, ast_fun);
            break;
        case T_START_SUB:
        case T_END_SUB:
        case T_SUB:
            ast_sub = parse_subshell(lexer);
            ast_sub->is_neg = is_neg;
            is_neg = 0;
            global->ast = add_ast(global->ast, ast_sub);
            break;
        case T_CASE:
            ast_case = parse_case(lexer);
            ast_case->is_neg = is_neg;
            is_neg = 0;
            global->ast = add_ast(global->ast, ast_case);
            break;
        default:
            if (is_redir(token))
            {
                ast_cmd = parse_cmd(lexer, false);
                ast_cmd->is_neg = is_neg;
                global->ast = add_ast(global->ast, ast_cmd);
                is_neg = 0;
            }
            else
            {
                errx(2, "Synsdiddddftaxhere Error.");
            }
            break;
        }
    }
    free_token(token);
    return global;
}
