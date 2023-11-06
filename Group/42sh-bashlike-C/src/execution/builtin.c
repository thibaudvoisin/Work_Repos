#include "builtin.h"

#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "utils/dict.h"

// case when -e is enabled, echo interpret the \n, \t and \\ escapes
void print_escape_character(char *argv)
{
    for (int i = 0; argv[i] != '\0'; i++)
    {
        if (argv[i] == '\\')
        {
            i++;
            if (argv[i] == 'n')
                printf("\n");
            if (argv[i] == 't')
                printf("\t");
            if (argv[i] == '\\')
                printf("\\");
        }
        else
            printf("%c", argv[i]);
    }
}

void exec_echo(char **argv)
{
    int n_option = 0;
    int e_option = 0;
    int word = 0;

    for (int i = 1; argv[i] != NULL; i++)
    {
        if (!strcmp(argv[i], "-n") && !word)
        {
            n_option = 1;
            continue;
        }
        else if (!strcmp(argv[i], "-e") && !word)
        {
            e_option = 1;
            continue;
        }
        else
            word = 1;
        if (!e_option)
            printf("%s", argv[i]);
        else
            print_escape_character(argv[i]);

        if (argv[i + 1] != NULL)
            printf(" ");
    }
    if (!n_option)
        printf("\n");
}

void exec_exit(char **argv, int last_code)
{
    if (!argv[1])
        exit(last_code);

    int max_int = 2147483647;
    long long value = atoll(argv[1]);
    if (value < 0 || value > max_int || (value == 0 && argv[1][0] != '0'))
        errx(2, "exit: Illegal number: %lld", value);
    int exit_value = atoi(argv[1]);
    exit(exit_value);
}

void exec_cd(char **argv, struct dict **dict)
{
    if (!argv[1])
        return;
    int err;
    if (!strcmp(argv[1], "-"))
    {
        struct dict *olddict = get_value(*dict, "OLDPWD");
        if (!olddict)
            return;

        char *oldpwd = olddict->value;
        if ((err = chdir(oldpwd)) == -1)
            errx(2, "cd can't find {%s}", oldpwd);

        printf("%s\n", oldpwd);

        if (getenv("PWD"))
            *dict = dict_change_value(*dict, strdup("OLDPWD"),
                                      strdup(getenv("PWD")));
        setenv("PWD", oldpwd, 1);
        return;
    }
    else
    {
        char *oldpwd = getenv("PWD");
        if ((err = chdir(argv[1])) == -1)
            errx(2, "cd can't find {%s}", argv[1]);
        if (oldpwd)
        {
            *dict = dict_change_value(*dict, strdup("OLDPWD"), strdup(oldpwd));
            char *pwd = strdup(oldpwd);
            pwd = realloc(pwd, strlen(pwd) + strlen(argv[1]) + 2);
            pwd = strcat(pwd, "/");
            pwd = strcat(pwd, argv[1]);
            setenv("PWD", pwd, 1);
            free(pwd);
        }
    }
}

void exec_unset(char **argv, struct dict **dict)
{
    if (!argv[1])
        return;
    if (!strcmp(argv[1], "-v"))
    {
        // TODO
        *dict = dict_remove(*dict, argv[2]);
    }
    else if (!strcmp(argv[1], "-f"))
        return; // need to be fixed  TODO
    else
        *dict = dict_remove(*dict, argv[1]);
}
