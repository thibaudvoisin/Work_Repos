#include "exec.h"

#include <err.h>
#include <fnmatch.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

#include "builtin.h"
#include "parser/parser.h"
#include "utils/alloc.h"
#include "utils/ast.h"
#include "utils/dict.h"
#include "utils/dict_fun.h"

// explanation : in a shell command with multiples command, we just return the
// code of the last command, even if there is an error while executing
int last_code = 0;

char *itoa(int nbargs)
{
    if (!nbargs)
    {
        char *str = calloc(2, 1);
        str[0] = '0';
        return str;
    }
    int dup = nbargs;
    int len = 0;
    while (dup > 0)
    {
        len++;
        dup /= 10;
    }
    char *nb_args = calloc(len + 1, sizeof(char));
    while (nbargs > 0)
    {
        nb_args[len - 1] = (nbargs % 10) + '0';
        nbargs /= 10;
        len--;
    }
    return nb_args;
}

char *expand_args(char *arg, struct dict *dict)
{
    char *word = strdup(arg);
    size_t begin_var = 0;
    size_t size_word = strlen(word);
    bool expand = true;
    if (size_word >= 2)
    {
        if (word[0] == '\'' || word[0] == '\"')
        {
            if (word[0] == '\'')
                expand = false;
            if (size_word == 2)
            {
                free(word);
                word = strdup("");
            }
            else
            {
                char c = word[0];
                char *word2 = calloc(size_word - 1, sizeof(char));
                size_t i = 0;
                for (; i < size_word - 1 && word[i + 1] != c; i++)
                    word2[i] = word[i + 1];
                for (; i < size_word - 2; i++)
                    word2[i] = word[i + 2];
                word2[i] = 0;
                free(word);
                word = word2;
            }
            size_word -= 2;
        }
    }
    while (begin_var < size_word && expand)
    {
        bool random = false;
        if (word[begin_var] == '$')
        {
            size_t end_var = ++begin_var;
            while (end_var < strlen(word) && word[end_var] != '$'
                   && word[end_var] != ' ')
            {
                if (word[end_var] == '{')
                {
                    while (end_var < strlen(word) && word[end_var] != '}')
                        end_var++;
                    if (end_var >= strlen(word))
                        errx(2, "Missing '}'");
                    size_t i = begin_var;
                    for (; i < end_var; i++)
                        word[i] = word[i + 1];
                    i--;
                    for (; i < size_word - 2; i++)
                        word[i] = word[i + 2];
                    word[i] = 0;
                    size_word -= 2;
                    end_var -= 1;
                    break;
                }
                end_var++;
            }
            char *name = strndup(word + begin_var, end_var - begin_var);
            if (end_var == begin_var && end_var < size_word
                && word[end_var] == '$')
            {
                free(name);
                name = strdup("$");
                end_var++;
            }
            struct dict *val;
            if (!strcmp(name, "RANDOM"))
            {
                val = zalloc(sizeof(struct dict));
                val->value = itoa((rand() % 32767));
                random = true;
            }
            else
            {
                val = get_value(dict, name);
            }
            if (!val || !val->value)
            {
                begin_var--;
                size_t v = 0;
                for (; v + end_var < strlen(word); v++)
                    word[v + begin_var] = word[v + end_var];
                word[v + begin_var] = 0;
                free(name);
                continue;
            }
            size_t ind_value = 0;
            size_t ind_word = begin_var - 1;
            size_t size_value = strlen(val->value);
            while (ind_word < end_var && ind_value < size_value)
            {
                word[ind_word] = val->value[ind_value];
                ind_word++;
                ind_value++;
            }
            if (ind_word >= end_var)
            {
                while (ind_value < size_value)
                {
                    size_word++;
                    word = realloc(word, (size_word + 1) * sizeof(char));
                    for (size_t m = size_word; m >= ind_word; m--)
                        word[m] = word[m - 1];
                    word[ind_word] = val->value[ind_value];
                    ind_word++;
                    ind_value++;
                    word[size_word] = 0;
                }
            }
            else if (ind_value >= size_value)
            {
                size_t diff = end_var - ind_word;
                size_t v = ind_word;
                for (; v + diff < strlen(word); v++)
                    word[v] = word[v + diff];
                word[v] = 0;
            }
            free(name);
            begin_var = ind_word - 1;
            if (random)
            {
                free(val->value);
                free(val);
            }
        }
        begin_var++;
    }
    word = realloc(word, size_word + 1);
    word[size_word] = 0;
    return word;
}

int redir(struct ast *ast_input, struct glob_dict **gb_dict)
{
    if (ast_input->redir)
    {
        int pid = fork();
        if (pid == -1)
            errx(2, "fork failed");
        else if (pid == 0)
        {
            struct ast *ast = ast_copy(ast_input, false);
            ast->next = NULL;
            FILE *fd = NULL;
            int fd_num = 0;
            int n = 0;
            int dr = 0;
            switch (ast->redir->redir_type)
            {
            case (T_REDIR_INPUT):
                fd = fopen(ast->redir->file_name, "r");
                if (fd == NULL)
                    errx(2, "error in redir, file does not exists");
                fd_num = fileno(fd);
                dr = dup2(fd_num, ast->redir->fd);
                if (dr == -1)
                    errx(2, "dup down <");
                free(ast->redir->file_name);
                free(ast->redir);
                ast->redir = NULL;
                n = exec(ast, gb_dict, -1, NULL);
                exit(n);
                break;

            case (T_REDIR_OUTPUT):
                fd = fopen(ast->redir->file_name, "w");
                fd_num = fileno(fd);
                dr = dup2(fd_num, ast->redir->fd);
                if (dr == -1)
                    errx(2, "dup down >");
                free(ast->redir->file_name);
                free(ast->redir);
                ast->redir = NULL;
                n = exec(ast, gb_dict, -1, NULL);
                exit(n);
                break;

            case (T_REDIR_APPEND_OUTPUT):
                fd = fopen(ast->redir->file_name, "a");
                fd_num = fileno(fd);
                dr = dup2(fd_num, ast->redir->fd);
                if (dr == -1)
                    errx(2, "dup down >>");
                free(ast->redir->file_name);
                free(ast->redir);
                ast->redir = NULL;
                n = exec(ast, gb_dict, -1, NULL);
                exit(n);
                break;

            case (T_REDIR_DUPLI_OUTPUT):
            case (T_REDIR_DUPLI_INPUT):
                if (!(strcmp(ast->redir->file_name, "-")))
                {
                    close(ast->redir->fd);
                }
                int atoi_res = atoi(ast->redir->file_name);
                if ((atoi_res <= 0 || atoi_res >= 10)
                    || ((!atoi_res) && (strcmp(ast->redir->file_name, "0"))))
                    errx(2, "Error in redir, not a valid file descriptor");
                dr = dup2(atoi_res, ast->redir->fd);
                if (dr == -1)
                    errx(2, "dup down >& <&");
                free(ast->redir->file_name);
                free(ast->redir);
                ast->redir = NULL;
                n = exec(ast, gb_dict, -1, NULL);
                exit(n);
                break;

            case (T_REDIR_OPEN_FILE):
                fd = fopen(ast->redir->file_name, "w+");
                fd_num = fileno(fd);
                dr = dup2(fd_num, ast->redir->fd);
                if (dr == -1)
                    errx(2, "dup down <>");
                free(ast->redir->file_name);
                free(ast->redir);
                ast->redir = NULL;
                n = exec(ast, gb_dict, -1, NULL);
                exit(n);
                break;

            default:
                errx(2, "strange");
            }
            ast_destroy(ast);
        }
        else
        {
            int wstatus;
            waitpid(pid, &wstatus, 0);
            if (WEXITSTATUS(wstatus) == 2)
                errx(2, "Error in fork redir");
            else
                return WEXITSTATUS(wstatus);
        }
    }
    return 300;
}

struct dict *reinit(struct dict *to, struct dict *from)
{
    char *c = calloc(2, sizeof(char));
    c[0] = '0';
    c[1] = 0;
    for (; c[0] <= '9'; c[0]++)
    {
        char *value = get_value(from, c)->value;
        if (value)
            value = strdup(value);
        to = dict_change_value(to, strdup(c), value);
    }
    free(c);
    return to;
}

struct dict *init_func_args(struct dict *dict, int argc, char **argv)
{
    char *c = calloc(2, sizeof(char));
    c[0] = '0';
    c[1] = 0;
    for (; c[0] - '0' < argc; c[0]++)
    {
        dict = dict_change_value(dict, strdup(c), strdup(argv[c[0] - '0']));
    }
    for (; c[0] <= '9'; c[0]++)
        dict = dict_change_value(dict, strdup(c), NULL);
    free(c);
    return dict;
}
// exec_cmd and check if cmd are builtins or not
int exec_cmd(struct ast *ast, struct glob_dict **gb_dict)
{
    int rd = redir(ast, gb_dict);
    if (rd != 300)
        return rd;
    int code = 0;
    char **argv2 = ast->data.ast_cmd.argv;
    char **argv = calloc(1, sizeof(char *));
    size_t size = 0;
    argv[0] = NULL;
    for (int i = 0; argv2 && argv2[i]; i++)
    {
        size++;
        argv = realloc(argv, (size + 1) * sizeof(char *));
        argv[i] = expand_args(argv2[i], (*gb_dict)->dict_var);
        argv[size] = NULL;
    }
    if (argv[0])
    {
        struct dict_fun *func = get_fun_value((*gb_dict)->dict_fun, argv[0]);
        if (!strcmp(argv[0], "echo"))
            exec_echo(argv);
        else if (!strcmp(argv[0], "exit"))
            exec_exit(argv, last_code);
        else if (!strcmp(argv[0], "cd"))
            exec_cd(argv, &(*gb_dict)->dict_var);
        else if (!strcmp(argv[0], "unset"))
            exec_unset(argv, &(*gb_dict)->dict_var);
        else if (func)
        {
            int argc = 0;
            while (argv[argc])
                argc++;
            struct glob_dict *copy_dict = calloc(1, sizeof(struct glob_dict));
            copy_dict->dict_var = dict_copy((*gb_dict)->dict_var);
            copy_dict->dict_var =
                init_func_args(copy_dict->dict_var, argc, argv);
            copy_dict->dict_fun = dict_fun_copy((*gb_dict)->dict_fun);
            func->ast->in_func = 1;
            code = exec(func->ast, &copy_dict, argc, argv);
            copy_dict->dict_var =
                reinit(copy_dict->dict_var, (*gb_dict)->dict_var);
            dict_fun_destroy((*gb_dict)->dict_fun);
            dict_destroy((*gb_dict)->dict_var);
            free(*gb_dict);
            *gb_dict = copy_dict;
        }
        else // when add other builtins, annex function may be required
        {
            int pid = fork();
            if (pid == 0)
            {
                execvp(argv[0], argv);
                warn("%s: command not found", argv[0]);
                exit(127); //_exit --> syscall that work for week 1
            }
            else
            {
                int wstatus;
                waitpid(pid, &wstatus, 0);
                code = WEXITSTATUS(wstatus);
            }
        }
        for (size_t i = 0; argv[i] != NULL; i++)
            free(argv[i]);
        fflush(stdout); /*
                           if (ast->type == NODE_NOT)
                           return code > 0 ? 0 : 1;*/
    }
    free(argv);
    return code;
}

// check specific case like if, while, for..., and run the right function
static int check_node(struct ast *ast, int in_cond, int first_call,
                      struct glob_dict **gb_dict)
{
    int code = 0;
    if (ast->type == NODE_IF)
    {
        code = exec_if(ast, in_cond, first_call, gb_dict);
        if (!in_cond)
            code = last_code;
    }
    if (ast->type == NODE_CMD)
        code = exec_cmd(ast, gb_dict);

    if (ast->type == NODE_WHILE)
    {
        code = exec_while(ast, in_cond, first_call, gb_dict);
        if (!in_cond)
            code = last_code;
    }

    if (ast->type == NODE_UNTIL)
    {
        code = exec_until(ast, in_cond, first_call, gb_dict);
        if (!in_cond)
            code = last_code;
    }

    if (ast->type == NODE_FOR)
    {
        code = exec_for(ast, in_cond, first_call, gb_dict);
        if (!in_cond)
            code = last_code;
    }
    if (ast->type == NODE_ASSIGN)
        exec_assign(ast, &(*gb_dict)->dict_var);

    if (ast->type == NODE_PIPE)
    {
        struct ast *pipe_copy = ast_copy(ast, false);
        code = exec_pipe(pipe_copy, *gb_dict);
        ast_destroy(pipe_copy);
    }

    if (ast->type == NODE_AND)
        code = !exec_and(ast, in_cond, first_call, gb_dict);

    if (ast->type == NODE_OR)
        code = !exec_or(ast, in_cond, first_call, gb_dict);

    if (ast->type == NODE_BLK)
        code = exec_blk(ast, in_cond, first_call, gb_dict);

    if (ast->type == NODE_SUB)
        code = exec_subshell(ast, in_cond, first_call, gb_dict);

    if (!in_cond && !first_call)
    {
        last_code = code;
        (*gb_dict)->dict_var = dict_change_value((*gb_dict)->dict_var,
                                                 strdup("?"), itoa(last_code));
    }
    return code;
}

int exec_elif(struct ast *ast, int in_cond, int first_call,
              struct glob_dict **gb_dict)
{
    int code = 0;
    int code_cond = 0;
    struct ast_elif elif = ast->data.ast_elif;
    while (elif.cond)
    {
        code = check_node(elif.cond, 1, 0, gb_dict);
        if (elif.cond->is_neg)
            code = code > 0 ? 0 : 1;

        code_cond = code;
        elif.cond = elif.cond->next;
    }
    if (!code) // if no error
    {
        while (elif.then)
        {
            code = check_node(elif.then, in_cond, 0, gb_dict);

            if (elif.then->is_neg)
            {
                code = code > 0 ? 0 : 1;
                last_code = code;
                (*gb_dict)->dict_var = dict_change_value(
                    (*gb_dict)->dict_var, strdup("?"), itoa(last_code));
            }
            elif.then = elif.then->next;
        }
    }
    if (!in_cond && !first_call && !code_cond)
    {
        last_code = code;
        (*gb_dict)->dict_var = dict_change_value((*gb_dict)->dict_var,
                                                 strdup("?"), itoa(last_code));
    }
    if ((in_cond || first_call) && code_cond)
        return 0;

    return code_cond;
}

// exec if that call if necessary exec_elif
// in_cond: if we are in a condition or its the first call to if (ie: = 1), we
// dont take the last code
int exec_if(struct ast *ast, int in_cond, int first_call,
            struct glob_dict **gb_dict)
{
    int rd = redir(ast, gb_dict);
    if (rd != 300)
        return rd;
    int code = 0;
    int code_cond = 0;
    struct ast_if ast_if = ast->data.ast_if;
    while (ast_if.cond)
    {
        code = check_node(ast_if.cond, 1, 0, gb_dict);
        if (ast_if.cond->is_neg)
            code = code > 0 ? 0 : 1;

        code_cond = code;
        ast_if.cond = ast_if.cond->next;
    }
    if (!code)
    {
        while (ast_if.then)
        {
            code = check_node(ast_if.then, in_cond, 0, gb_dict);
            if (ast_if.then->is_neg)
            {
                code = code > 0 ? 0 : 1;
                last_code = code;
                (*gb_dict)->dict_var = dict_change_value(
                    (*gb_dict)->dict_var, strdup("?"), itoa(last_code));
            }
            ast_if.then = ast_if.then->next;
        }
    }
    else
    {
        while (ast_if.elif && code)
        {
            code = exec_elif(ast_if.elif, in_cond, 0, gb_dict);
            if (ast_if.elif->is_neg)
                code = code > 0 ? 0 : 1;

            ast_if.elif = ast_if.elif->next;
        }
        while (ast_if.else_ && code)
        {
            code = check_node(ast_if.else_, in_cond, 0, gb_dict);
            if (ast_if.else_->is_neg)
            {
                code = code > 0 ? 0 : 1;
                last_code = code;
                (*gb_dict)->dict_var = dict_change_value(
                    (*gb_dict)->dict_var, strdup("?"), itoa(last_code));
            }
            ast_if.else_ = ast_if.else_->next;
        }
    }

    if (!in_cond && !first_call && !code_cond)
    {
        last_code = code;
        (*gb_dict)->dict_var = dict_change_value((*gb_dict)->dict_var,
                                                 strdup("?"), itoa(last_code));
    }

    if ((in_cond || first_call) && code_cond)
        return 0;
    return code;
}

int exec_while(struct ast *ast, int in_cond, int first_call,
               struct glob_dict **gb_dict)
{
    int rd = redir(ast, gb_dict);
    if (rd != 300)
        return rd;
    int code = 0;
    int code_cond = 0;
    struct ast_while ast_while = ast->data.ast_while;
    while (!code_cond)
    {
        struct ast_while tmp = ast_while;
        while (tmp.cond)
        {
            code = check_node(tmp.cond, 1, 0, gb_dict);
            if (tmp.cond->is_neg)
                code = code > 0 ? 0 : 1;

            code_cond = code;
            tmp.cond = tmp.cond->next;
        }
        if (!code)
        {
            while (tmp.do_)
            {
                code = check_node(tmp.do_, in_cond, 0, gb_dict);
                if (tmp.do_->is_neg)
                {
                    code = code > 0 ? 0 : 1;
                    last_code = code;
                    (*gb_dict)->dict_var = dict_change_value(
                        (*gb_dict)->dict_var, strdup("?"), itoa(last_code));
                }
                tmp.do_ = tmp.do_->next;
            }
        }
    }

    if (!in_cond && !first_call && !code_cond)
    {
        last_code = code;
        (*gb_dict)->dict_var = dict_change_value((*gb_dict)->dict_var,
                                                 strdup("?"), itoa(last_code));
    }

    if ((in_cond || first_call) && code_cond)
        return 0;
    return code;
}

int exec_until(struct ast *ast, int in_cond, int first_call,
               struct glob_dict **gb_dict)
{
    int rd = redir(ast, gb_dict);
    if (rd != 300)
        return rd;
    int code = 1;
    int code_cond = 1;
    struct ast_until ast_until = ast->data.ast_until;
    while (code_cond)
    {
        struct ast_until tmp = ast_until;
        while (tmp.cond)
        {
            code = check_node(tmp.cond, 1, 0, gb_dict);
            if (tmp.cond->is_neg)
                code = code > 0 ? 0 : 1;

            code_cond = code;
            tmp.cond = tmp.cond->next;
        }
        if (code)
        {
            while (tmp.do_)
            {
                code = check_node(tmp.do_, in_cond, 0, gb_dict);
                if (tmp.do_->is_neg)
                {
                    code = code > 0 ? 0 : 1;
                    last_code = code;
                    (*gb_dict)->dict_var = dict_change_value(
                        (*gb_dict)->dict_var, strdup("?"), itoa(last_code));
                }
                tmp.do_ = tmp.do_->next;
                code = code == 0 ? 1 : code;
            }
        }
    }

    if (!in_cond && !first_call && code_cond)
    {
        last_code = code;
        (*gb_dict)->dict_var = dict_change_value((*gb_dict)->dict_var,
                                                 strdup("?"), itoa(last_code));
    }

    if ((in_cond || first_call) && !code_cond)
        return 0;
    return code;
}

int exec_for(struct ast *ast, int in_cond, int first_call,
             struct glob_dict **gb_dict)
{
    int rd = redir(ast, gb_dict);
    if (rd != 300)
        return rd;
    int code = 0;
    struct ast_for ast_for = ast->data.ast_for;
    for (struct list *word = ast_for.words; word; word = word->next)
    {
        char *new_data = expand_args(word->data, (*gb_dict)->dict_var);
        free(word->data);
        word->data = new_data;
        // change here with strdup to avoid double free
        (*gb_dict)->dict_var = dict_change_value(
            (*gb_dict)->dict_var, strdup(ast_for.var), strdup(word->data));
        struct ast_for tmp = ast_for;
        while (tmp.do_)
        {
            code = check_node(tmp.do_, in_cond, 0, gb_dict);
            if (tmp.do_->is_neg)
            {
                code = code > 0 ? 0 : 1;
                last_code = code;
                (*gb_dict)->dict_var = dict_change_value(
                    (*gb_dict)->dict_var, strdup("?"), itoa(last_code));
            }
            tmp.do_ = tmp.do_->next;
        }
        (*gb_dict)->dict_var = dict_remove((*gb_dict)->dict_var, ast_for.var);
    }

    if (!in_cond && !first_call)
    {
        last_code = code;
        (*gb_dict)->dict_var = dict_change_value((*gb_dict)->dict_var,
                                                 strdup("?"), itoa(last_code));
    }
    return code;
}

int exec_pipe(struct ast *ast, struct glob_dict *gb_dict)
{
    int rd = redir(ast, &gb_dict);
    if (rd != 300)
        return rd;
    int filesd[2];
    int pr = pipe(filesd);
    if (pr == -1)
        exit(1);
    int pid = fork();
    if (pid == -1)
        exit(1);
    else if (pid == 0)
    {
        close(filesd[0]);
        int dr1 = dup2(filesd[1], 1);
        if (dr1 == -1)
            exit(1);
        close(filesd[1]);
        struct dict *copy1_1 = dict_copy(gb_dict->dict_var);
        struct dict_fun *copy1_2 = dict_fun_copy(gb_dict->dict_fun);
        struct glob_dict *copy1 = calloc(1, sizeof(struct glob_dict));
        copy1->dict_var = copy1_1;
        copy1->dict_fun = copy1_2;
        int k = exec(ast->data.ast_pipe.left, &copy1, 0, NULL);
        exit(k);
    }
    else
    {
        int pid2 = fork();
        if (pid2 == -1)
            exit(1);
        else if (pid2 == 0)
        {
            ast->next = NULL;
            close(filesd[1]);
            int dr2 = dup2(filesd[0], 0);
            if (dr2 == -1)
                exit(1);
            close(filesd[0]);
            struct dict *copy2_1 = dict_copy(gb_dict->dict_var);
            struct dict_fun *copy2_2 = dict_fun_copy(gb_dict->dict_fun);
            struct glob_dict *copy2 = calloc(1, sizeof(struct glob_dict));
            copy2->dict_var = copy2_1;
            copy2->dict_fun = copy2_2;
            int n =
                exec(ast->data.ast_pipe.right, &copy2, 0, NULL) == -1 ? 1 : 0;
            exit(n);
        }
        else
        {
            close(filesd[1]);
            close(filesd[0]);
            int wstatus;
            waitpid(pid, NULL, 0);
            waitpid(pid2, &wstatus, 0);
            if (WIFEXITED(wstatus))
                return WEXITSTATUS(wstatus);
            else
                return 1;
        }
    }
    return 0;
}

// exec function that initialy call the ast from parser

void exec_assign(struct ast *ast, struct dict **dict)
{
    char *word = NULL;
    if (ast->data.ast_assign.value)
        word = strdup(ast->data.ast_assign.value);
    size_t begin_var = 0;
    size_t size_word = 0;
    if (word)
        size_word = strlen(word);
    bool expand = true;
    if (size_word >= 2)
    {
        if (word[0] == '\'' || word[0] == '\"')
        {
            if (word[0] == '\'')
                expand = false;
            if (size_word == 2)
            {
                free(word);
                word = strdup("");
            }
            else
            {
                char c = word[0];
                char *word2 = calloc(size_word - 1, sizeof(char));
                size_t i = 0;
                for (; i < size_word - 1 && word[i + 1] != c; i++)
                    word2[i] = word[i + 1];
                for (; i < size_word - 2; i++)
                    word2[i] = word[i + 2];
                word2[i] = 0;
                free(word);
                word = word2;
            }
            size_word -= 2;
        }
    }
    while (begin_var < size_word && expand)
    {
        if (word[begin_var] == '$')
        {
            size_t end_var = ++begin_var;
            while (end_var < strlen(word) && word[end_var] != '$')
            {
                if (word[end_var] == '{')
                {
                    while (end_var < strlen(word) && word[end_var] != '}')
                        end_var++;
                    if (end_var >= strlen(word))
                        errx(2, "Missing '}'");
                    size_t i = begin_var;
                    for (; i < end_var; i++)
                        word[i] = word[i + 1];
                    i--;
                    for (; i < size_word - 2; i++)
                        word[i] = word[i + 2];
                    word[i] = 0;
                    size_word -= 2;
                    end_var -= 1;
                    break;
                }
                end_var++;
            }
            char *name = strndup(word + begin_var, end_var - begin_var);
            if (end_var == begin_var && end_var < size_word
                && word[end_var] == '$')
            {
                free(name);
                name = strdup("$");
                end_var++;
            }
            struct dict *val = get_value(*dict, name);
            if (!val || !val->value)
            {
                begin_var--;
                size_t v = 0;
                for (; v + end_var < strlen(word); v++)
                    word[v + begin_var] = word[v + end_var];
                word[v + begin_var] = 0;
                free(name);
                continue;
            }
            size_t ind_value = 0;
            size_t ind_word = begin_var - 1;
            size_t size_value = strlen(val->value);
            while (ind_word < end_var && ind_value < size_value)
            {
                word[ind_word] = val->value[ind_value];
                ind_word++;
                ind_value++;
            }
            if (ind_word >= end_var)
            {
                while (ind_value < size_value)
                {
                    size_word++;
                    word = realloc(word, (size_word + 1) * sizeof(char));
                    for (size_t m = size_word; m >= ind_word; m--)
                        word[m] = word[m - 1];
                    word[ind_word] = val->value[ind_value];
                    ind_word++;
                    ind_value++;
                    word[size_word] = 0;
                }
            }
            else if (ind_value >= size_value)
            {
                size_t diff = end_var - ind_word;
                size_t v = ind_word;
                for (; v + diff < strlen(word); v++)
                    word[v] = word[v + diff];
                word[v] = 0;
            }
            free(name);
            begin_var = ind_word - 1;
        }
        begin_var++;
    }
    word = realloc(word, size_word + 1);
    word[size_word] = 0;
    *dict = dict_change_value(*dict, strdup(ast->data.ast_assign.name),
                              strdup(word));
    free(word);
}

char *get_args_list(int argc, char **argv)
{
    if (argc == 0 || (argc == 3 && (strcmp(argv[1], "-c") == 0)))
        return NULL;
    char *list_arg = calloc(1, sizeof(char));
    unsigned long size = 0;
    for (int i = 2; i < argc; i++)
    {
        size += (strlen(argv[i]));
        list_arg = realloc(list_arg, (size + 1) * sizeof(char));
        size_t j = 0;
        for (; j < strlen(argv[i]); j++)
        {
            list_arg[j + size - strlen(argv[i])] = argv[i][j];
        }
        list_arg[size] = 0;
        // list_arg = strcpy(list_arg + size - strlen(argv[i]), argv[i]);
    }
    list_arg = realloc(list_arg, size + 2);
    list_arg[size + 1] = 0;
    return list_arg;
}

struct dict *initialize_dict(int argc, char **argv)
{
    struct dict *gb_dict = NULL;

    int nbargs = 0;
    char *c = calloc(2, 1);
    int beg_arg = 2;
    if (argc >= 3 && argv[1][0] == '-')
        beg_arg = 3;
    c[0] = '0';
    for (; c[0] - '0' <= (argc - beg_arg); c[0]++, nbargs++)
        gb_dict = dict_change_value(gb_dict, strdup(c),
                                    strdup(argv[c[0] - '0' + beg_arg - 1]));
    for (; c[0] <= '9'; ++(c[0]))
        gb_dict = dict_change_value(gb_dict, strdup(c), NULL);
    free(c);
    char *list_arg = get_args_list(argc, argv);
    char *list1 = NULL;
    char *list2 = NULL;
    if (list_arg)
    {
        list1 = strdup(list_arg);
        list2 = strdup(list_arg);
    }
    gb_dict = dict_change_value(gb_dict, strdup("@"), list1);
    gb_dict = dict_change_value(gb_dict, strdup("*"), list2);

    free(list_arg);
    char *nb_args = itoa(nbargs);
    gb_dict = dict_change_value(gb_dict, strdup("#"), nb_args);

    gb_dict = dict_change_value(gb_dict, strdup("?"), strdup("0")); // TODO

    gb_dict = dict_change_value(gb_dict, strdup("$"), itoa(getpid()));

    srand(time(NULL));

    gb_dict = dict_change_value(gb_dict, strdup("UID"), itoa(getuid()));

    // check return getenv
    if (getenv("OLDPWD"))
        gb_dict = dict_change_value(gb_dict, strdup("OLDPWD"),
                                    strdup(getenv("OLDPWD")));

    gb_dict = dict_change_value(gb_dict, strdup("IFS"), strdup("")); // TODO

    return gb_dict;
}

int exec_and(struct ast *ast, int in_cond, int first_call,
             struct glob_dict **gb_dict)
{
    int rd = redir(ast, gb_dict);
    if (rd != 300)
        return rd;
    struct ast *left = ast->data.ast_and.left;
    struct ast *right = ast->data.ast_and.right;
    int code1 = !check_node(left, in_cond, first_call, gb_dict);
    int code2 = !check_node(right, in_cond, first_call, gb_dict);
    if (left->is_neg)
        code1 = !code1;
    if (right->is_neg)
        code2 = !code2;
    return code1 && code2;
}

int exec_or(struct ast *ast, int in_cond, int first_call,
            struct glob_dict **gb_dict)
{
    int rd = redir(ast, gb_dict);
    if (rd != 300)
        return rd;
    struct ast *left = ast->data.ast_and.left;
    struct ast *right = ast->data.ast_and.right;
    int code1 = !check_node(left, in_cond, first_call, gb_dict);
    int code2 = !check_node(right, in_cond, first_call, gb_dict);
    if (left->is_neg)
        code1 = !code1;
    if (right->is_neg)
        code2 = !code2;
    return code1 || code2;
}

int exec_subshell(struct ast *ast, int in_cond, int first_call,
                  struct glob_dict **gb_dict)
{
    int code = 0;
    int pid = fork();
    if (pid == -1)
        errx(2, "fork failed");
    else if (pid == 0)
    {
        struct ast *sub = ast->data.ast_blk.ast;
        // TODO TODO
        while (sub)
        {
            code = check_node(sub, in_cond, first_call, gb_dict);
            if (sub->is_neg)
                code = code > 0 ? 0 : 1;
            sub = sub->next;
        }
        if (ast->is_neg)
            code = code > 0 ? 0 : 1;
        exit(code);
    }
    else
    {
        int wstatus;
        waitpid(pid, &wstatus, 0);
        if (WEXITSTATUS(wstatus) == 2)
            errx(2, "Error in fork subshell");
        else
            code = WEXITSTATUS(wstatus);
    }
    // if (!in_cond && !first_call)
    //  last_code = code;
    return code;
}

int exec_blk(struct ast *ast, int in_cond, int first_call,
             struct glob_dict **gb_dict)
{
    struct ast *to_exec = ast->data.ast_blk.ast;
    int code = 0;
    while (to_exec)
    {
        code = check_node(to_exec, in_cond, first_call, gb_dict);
        if (to_exec->is_neg)
            code = code > 0 ? 0 : 1;
        to_exec = to_exec->next;
    }

    // if (!in_cond && !first_call)
    //  last_code = code;
    return code;
}

struct dict_fun *def_fun(struct ast *ast, struct dict_fun *dict)
{
    struct ast *to_exec = ast->data.ast_fun.blk->data.ast_blk.ast;
    to_exec->in_func = 1;
    struct dict_fun *new =
        dict_fun_change_value(dict, strdup(ast->data.ast_fun.name), to_exec);
    return new;
}

int exec_case(struct ast *ast, int in_cond, int first_call,
              struct glob_dict **gb_dict)
{
    char *var = expand_args(ast->data.ast_case.var, (*gb_dict)->dict_var);
    int code = 0;
    for (struct list_ast *cond = ast->data.ast_case.list_cond; cond;
         cond = cond->next)
    {
        if (fnmatch(cond->cond, var, 0) == 0)
        {
            struct ast *ast = cond->ast;
            while (ast && !code)
            {
                code = check_node(ast, in_cond, first_call, gb_dict);
                ast = ast->next;
            }
            break;
        }
    }
    free(var);
    return code;
}

int exec(struct ast *gb_ast, struct glob_dict **gb_dict, int argc, char **argv)
{
    if (!(*gb_dict)->dict_var)
        (*gb_dict)->dict_var = initialize_dict(argc, argv);
    struct ast *ast = gb_ast;
    struct ast *pipe_copy = NULL;
    bool in_func = gb_ast->in_func == 1;
    while (ast)
    {
        switch (ast->type)
        {
        case NODE_IF:
            exec_if(ast, 0, 1, gb_dict);
            if (ast->is_neg)
                last_code = last_code > 0 ? 0 : 1;
            (*gb_dict)->dict_var = dict_change_value(
                (*gb_dict)->dict_var, strdup("?"), itoa(last_code));
            break;

        case NODE_CMD:
            last_code = exec_cmd(ast, gb_dict);
            if (ast->is_neg)
                last_code = last_code > 0 ? 0 : 1;
            (*gb_dict)->dict_var = dict_change_value(
                (*gb_dict)->dict_var, strdup("?"), itoa(last_code));
            break;

        case NODE_WHILE:
            exec_while(ast, 0, 1, gb_dict);
            if (ast->is_neg)
                last_code = last_code > 0 ? 0 : 1;
            (*gb_dict)->dict_var = dict_change_value(
                (*gb_dict)->dict_var, strdup("?"), itoa(last_code));
            break;

        case NODE_UNTIL:
            exec_until(ast, 0, 1, gb_dict);
            if (ast->is_neg)
                last_code = last_code > 0 ? 0 : 1;
            (*gb_dict)->dict_var = dict_change_value(
                (*gb_dict)->dict_var, strdup("?"), itoa(last_code));
            break;

        case NODE_FOR:
            exec_for(ast, 0, 1, gb_dict);
            if (ast->is_neg)
                last_code = last_code > 0 ? 0 : 1;
            (*gb_dict)->dict_var = dict_change_value(
                (*gb_dict)->dict_var, strdup("?"), itoa(last_code));
            break;

        case NODE_ASSIGN:
            exec_assign(ast, &(*gb_dict)->dict_var);
            break;
        case NODE_PIPE:
            pipe_copy = ast_copy(ast, false);
            pipe_copy->next = NULL;
            exec_pipe(pipe_copy, *gb_dict);
            ast_destroy(pipe_copy);
            break;
        case NODE_AND:
            last_code = !exec_and(ast, 0, 1, gb_dict);
            (*gb_dict)->dict_var = dict_change_value(
                (*gb_dict)->dict_var, strdup("?"), itoa(last_code));
            break;
        case NODE_OR:
            last_code = !exec_or(ast, 0, 1, gb_dict);
            (*gb_dict)->dict_var = dict_change_value(
                (*gb_dict)->dict_var, strdup("?"), itoa(last_code));
            break;
        case NODE_BLK:
            last_code = exec_blk(ast, 0, 1, gb_dict);
            if (ast->is_neg)
                last_code = last_code > 0 ? 0 : 1;
            (*gb_dict)->dict_var = dict_change_value(
                (*gb_dict)->dict_var, strdup("?"), itoa(last_code));
            break;
        case NODE_FUN:
            (*gb_dict)->dict_fun = def_fun(ast, (*gb_dict)->dict_fun);
            break;
        case NODE_SUB:
            last_code = exec_subshell(ast, 0, 1, gb_dict);
            if (ast->is_neg)
                last_code = last_code > 0 ? 0 : 1;
            (*gb_dict)->dict_var = dict_change_value(
                (*gb_dict)->dict_var, strdup("?"), itoa(last_code));
            break;
        case NODE_CASE:
            last_code = exec_case(ast, 0, 1, gb_dict);
            if (ast->is_neg)
                last_code = last_code > 0 ? 0 : 1;
            (*gb_dict)->dict_var = dict_change_value(
                (*gb_dict)->dict_var, strdup("?"), itoa(last_code));
            break;
        default:
            errx(1, "Node not recognized");
            break;
        }
        ast = ast->next;
    }
    if (argc != -1 && !in_func)
    {
        dict_destroy((*gb_dict)->dict_var);
        dict_fun_destroy((*gb_dict)->dict_fun);
        free(*gb_dict);
        *gb_dict = NULL;
    }
    return last_code;
}
