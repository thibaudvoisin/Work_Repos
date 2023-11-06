#define _POSIX_C_SOURCE 200809L
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "run_command.h"

int cat(int argc, char **argv);
int head(int argc, char **argv);
int echo(int argc, char **argv);

int main()
{
    add_command("cat", &cat);
    add_command("echo", &echo);
    add_command("head", &head);

    char *buf = calloc(10000, 1);
    size_t val = 0;

    int ret_code = 0;

    while (!feof(stdin))
    {
        char **argv_s = calloc(1000, sizeof(char *));
        unsigned int argc_s = 0;
        if (getline(&buf, &val, stdin) != -1)
        {
            char *token;
            char *rest = buf;

            while ((token = strtok_r(rest, "\n ", &rest)))
            {
                argv_s[argc_s] = token;
                argc_s++;
            }
            if (!strcmp(argv_s[0], "quit"))
                return ret_code;

            ret_code = run_command(argc_s, argv_s);
        }
    }
    return ret_code;
}

int cat(int argc, char **argv)
{
    if (argc == 1)
    {
        fprintf(stderr, "cat: Argument is missing\n");
        return 1;
    }
    int ret_cod = 0;
    for (int i = 1; i < argc; ++i)
    {
        FILE *f = fopen(argv[i], "r");
        if (f == NULL)
        {
            fprintf(stderr, "cat: %s: No such file or directory\n", argv[i]);
            return 1;
            ret_cod = 1;
        }
        else
        {
            char *buf = calloc(10000, 1);
            size_t val = 0;
            while (!feof(f))
            {
                if (getline(&buf, &val, f) != -1)
                {
                    printf("%s", buf);
                    fflush(stdout);
                }
            }
        }
    }
    return ret_cod;
}

int echo(int argc, char **argv)
{
    for (int i = 1; i < argc; ++i)
    {
        if (i != argc - 1)
            printf("%s ", argv[i]);
        else
            printf("%s", argv[i]);
    }
    printf("\n");
    fflush(stdout);
    return 0;
}

int head(int argc, char **argv)
{
    if (argc == 1)
    {
        fprintf(stderr, "head: Argument is missing\n");
        return 1;
    }
    int ret_cod = 0;
    for (int i = 1; i < argc; ++i)
    {
        FILE *f = fopen(argv[i], "r");
        if (f == NULL)
        {
            fprintf(stderr, "head: %s: No such file or directory\n", argv[i]);
            return 1;
            ret_cod = 1;
        }
        else
        {
            if (argc > 2)
            {
                if (i > 1)
                    printf("\n==> %s <==\n", argv[i]);
                else
                    printf("==> %s <==\n", argv[i]);
            }
            char *buf = calloc(10000, 1);
            size_t val = 0;
            int count = 0;
            while (!feof(f) && count < 10)
            {
                if (getline(&buf, &val, f) != -1)
                {
                    printf("%s", buf);
                    fflush(stdout);
                }
                ++count;
            }
        }
    }
    return ret_cod;
}
