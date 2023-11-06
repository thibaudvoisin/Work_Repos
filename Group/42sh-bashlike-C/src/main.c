#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#include "execution/exec.h"
#include "lexer/lexer.h"
#include "parser/parser.h"
#include "utils/ast.h"
#include "utils/dict.h"
#include "utils/dict_fun.h"
#include "utils/list.h"

void print_dict(struct dict *dict)
{
    while (dict != NULL)
    {
        printf("Dict:%s->%s\n", dict->key, dict->value);
        dict = dict->next;
    }
}

void print_ast(struct ast *ast)
{
    while (ast)
    {
        switch (ast->type)
        {
        case NODE_IF:
            printf("IF: \n");
            printf("cond: ");
            print_ast(ast->data.ast_if.cond);
            printf("then: ");
            print_ast(ast->data.ast_if.then);
            print_ast(ast->data.ast_if.elif);
            if (ast->data.ast_if.else_ != NULL)
            {
                printf("else: ");
                print_ast(ast->data.ast_if.else_);
            }
            puts("FI");
            break;

        case NODE_ELIF:
            printf("ELIF: \n");
            printf("cond: ");
            print_ast(ast->data.ast_elif.cond);
            printf("then: ");
            print_ast(ast->data.ast_elif.then);
            break;

        case NODE_CMD:
            printf("CMD: \n");
            char **args = ast->data.ast_cmd.argv;
            if (!args)
                break;
            while (*args)
            {
                printf("%s ", *args);
                args++;
            }
            break;

        case NODE_FOR:
            printf("FOR \n");
            printf("var: %s\n", ast->data.ast_for.var);
            printf("cond: ");
            char *stres;
            while ((stres = list_pop(&(ast->data.ast_for.words))) != NULL)
            {
                printf("%s-", stres);
                free(stres);
            }
            printf("\n");
            printf("then: ");
            print_ast(ast->data.ast_for.do_);
            puts("DONE");
            break;

        case NODE_UNTIL:
            printf("UNTIL \n");
            printf("cond: ");
            print_ast(ast->data.ast_until.cond);
            printf("then: ");
            print_ast(ast->data.ast_until.do_);
            puts("DONE");
            break;

        case NODE_WHILE:
            printf("WHILE \n");
            printf("cond: ");
            print_ast(ast->data.ast_while.cond);
            printf("then: ");
            print_ast(ast->data.ast_while.do_);
            puts("DONE");
            break;
        case NODE_ASSIGN:
            printf("ASSIGN\n");
            printf("name: %s, value: %s\n", ast->data.ast_assign.name,
                   ast->data.ast_assign.value);
            break;
        case NODE_PIPE:
            printf("(");
            print_ast(ast->data.ast_pipe.left);
            printf(" | ");
            print_ast(ast->data.ast_pipe.right);
            printf(")\n");
            break;
        case NODE_AND:
            printf("(");
            print_ast(ast->data.ast_and.left);
            printf(" && ");
            print_ast(ast->data.ast_and.right);
            printf(")\n");
            break;
        case NODE_OR:
            printf("(");
            print_ast(ast->data.ast_or.left);
            printf(" || ");
            print_ast(ast->data.ast_or.right);
            printf(")\n");
            break;
        case NODE_BLK:
            printf("{ BLK:\n");
            print_ast(ast->data.ast_blk.ast);
            printf(" END_BLK }\n");
            break;
        case NODE_FUN:
            printf(" {FUN:\n");
            printf("%s : ", ast->data.ast_fun.name);
            print_ast(ast->data.ast_fun.blk);
            printf(" END_FUN }\n");
            break;
        case NODE_SUB:
            printf("SUB \n");
            print_ast(ast->data.ast_sub.ast);
            break;
        case NODE_CASE:
            printf("[ CASE\nvar: ");
            puts(ast->data.ast_case.var);
            int i = 1;
            for (struct list_ast *list = ast->data.ast_case.list_cond; list;
                 list = list->next)
            {
                printf("case %d:\ncond: %s\nast :", i, list->cond);
                print_ast(list->ast);
                i++;
            }
            printf("END_CASE ]\n");
        }
        if (ast->redir)
        {
            printf("\nredir: ");
            printf("%d - %d - %s\n", ast->redir->redir_type, ast->redir->fd,
                   ast->redir->file_name);
        }

        ast = ast->next;
    }
    printf("\n");
}

void print_lexer(struct lexer *lexer)
{
    struct token *tok = get_next_token(&lexer);
    int went_in = 1;
    while (tok->type != T_EOF)
    {
        if (tok->var && went_in)
        {
            printf("%s-", tok->var);
            went_in = 0;
        }
        printf("%s ---->> %u\n", tok->data, tok->type);
        tok = get_next_token(&lexer);
    }
}

void remove_spaces(char *input)
{
    int i = 0;
    while (input[i] != 0)
    {
        if (input[i] == '\n')
        {
            int j = i - 1;
            while (j >= 0 && input[j] == ' ')
            {
                j--;
            }
            int diff = i - j - 1;
            int k = j + 1;
            for (; input[k + diff] != 0; k++)
            {
                input[k] = input[k + diff];
            }
            input[k] = 0;
            i -= diff;
        }
        i++;
    }
}

void print_lexer_on_file(char *argv)
{
    FILE *file = fopen(argv, "r");
    if (!file)
        errx(1, "Cannot open file\nUsage: ./42sh <file>");
    fseek(file, 0, SEEK_END);
    long size_file = ftell(file);
    fseek(file, 0, SEEK_SET);
    char *input = calloc(size_file + 1, sizeof(char));
    if (!fread(input, sizeof(char), size_file, file))
        errx(1, "File is empty");

    remove_spaces(input);
    struct lexer *lexer = new_lexer(input);

    print_lexer(lexer);

    free(input);
    free_token(lexer->token);
    free(lexer);
}

void print_ast_on_file(char *argv)
{
    FILE *file = fopen(argv, "r");
    if (!file)
        errx(1, "Cannot open file\nUsage: ./42sh <file>");
    fseek(file, 0, SEEK_END);
    long size_file = ftell(file);
    fseek(file, 0, SEEK_SET);
    char *input = calloc(size_file + 1, sizeof(char));
    if (!fread(input, sizeof(char), size_file, file))
        errx(1, "File is empty");

    remove_spaces(input);
    struct lexer *lexer = new_lexer(input);
    struct global *global = parse_input(lexer);
    print_ast(global->ast);

    free(input);
    free_token(lexer->token);
    free(lexer);
    ast_destroy(global->ast);
    dict_destroy(global->dict);
    free(global);
}

int main(int argc, char *argv[])
{
    srand(time(NULL));
    int return_code = 0;

    if (argc < 2)
    {
        // read stdin
        char *input = calloc(262144, sizeof(char));
        if (!fread(input, sizeof(char), 262144, stdin))
            exit(0);

        remove_spaces(input);
        struct lexer *lexer = new_lexer(input);

        struct global *global = parse_input(lexer);
        struct glob_dict *gb_dict = calloc(1, sizeof(struct glob_dict));
        gb_dict->dict_var = global->dict;
        gb_dict->dict_fun = NULL;
        return_code = exec(global->ast, &gb_dict, argc, argv);

        free(input);
        free_token(lexer->token);
        free(lexer);
        ast_destroy(global->ast);
        dict_destroy(global->dict);
        free(gb_dict);
        free(global);
    }
    else
    {
        //-c option
        if (!strcmp(argv[1], "-c"))
        {
            if (argc < 3)
                errx(1, "Usage: ./42sh -c \"<command>\"");

            // take argument given in parameter without double quotes
            char *input = calloc(strlen(argv[2]) + 1, sizeof(char));
            input = strncpy(input, argv[2], strlen(argv[2]));

            remove_spaces(input);

            if (!strcmp(input, ""))
                return 0;

            struct lexer *lexer = new_lexer(input);

            // test parser
            struct global *global = parse_input(lexer);
            struct glob_dict *gb_dict = calloc(1, sizeof(struct glob_dict));
            gb_dict->dict_var = global->dict;
            gb_dict->dict_fun = NULL;

            return_code = exec(global->ast, &gb_dict, argc, argv);

            free(input);
            free_token(lexer->token);
            free(lexer);
            ast_destroy(global->ast);
            dict_destroy(global->dict);
            free(gb_dict);
            free(global);
        }
        else if (!strcmp(argv[1], "-print_lexer"))
        {
            if (argc == 3)
            {
                char *input = calloc(strlen(argv[2]) + 1, sizeof(char));
                strcpy(input, argv[2]);
                remove_spaces(input);
                struct lexer *lexer = new_lexer(input);
                print_lexer(lexer);

                free(input);
                free_token(lexer->token);
                free(lexer);
            }
            else if (!strcmp(argv[2], "-file"))
                print_lexer_on_file(argv[3]);
        }
        else if (!strcmp(argv[1], "-print_ast"))
        {
            if (argc == 3)
            {
                char *input = calloc(strlen(argv[2]) + 1, sizeof(char));
                strcpy(input, argv[2]);

                remove_spaces(input);
                struct lexer *lexer = new_lexer(input);
                struct global *global = parse_input(lexer);

                print_ast(global->ast);
                print_dict(global->dict);

                free(input);
                free_token(lexer->token);
                free(lexer);
                ast_destroy(global->ast);
                dict_destroy(global->dict);
                free(global);
            }
            else if (!strcmp(argv[2], "-file"))
                print_ast_on_file(argv[3]);
        }
        else // we can add some custom options above (like pretty print...)
        {
            // read in file
            FILE *file = fopen(argv[1], "r");
            if (!file)
                errx(1, "Cannot open file\nUsage: ./42sh <file>");
            fseek(file, 0, SEEK_END);
            long size_file = ftell(file);
            fseek(file, 0, SEEK_SET);
            char *input = calloc(size_file + 1, sizeof(char));
            if (!fread(input, sizeof(char), size_file, file))
                errx(1, "File is empty");

            remove_spaces(input);

            if (!strcmp(input, ""))
                return 0;

            struct lexer *lexer = new_lexer(input);
            struct global *global = parse_input(lexer);
            struct glob_dict *gb_dict = calloc(1, sizeof(struct glob_dict));
            gb_dict->dict_var = global->dict;
            gb_dict->dict_fun = NULL;

            return_code = exec(global->ast, &gb_dict, argc, argv);

            free(input);
            free_token(lexer->token);
            free(lexer);
            ast_destroy(global->ast);
            dict_destroy(global->dict);
            free(gb_dict);
            free(global);
        }
    }
    return return_code;
}
