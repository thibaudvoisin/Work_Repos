#include <stddef.h>
#include <stdlib.h>
#include <string.h>

struct command
{
    const char *name;
    int (*fct_ptr)(int, char **);
};

struct command_list
{
    struct command **list;
    size_t size;
};

struct command_list *glob_command_list = NULL;

int add_command(const char *name, int (*command)(int, char **))
{
    if (command == NULL || name == NULL)
        return -1;

    if (glob_command_list != NULL && glob_command_list->size)
    {
        for (size_t i = 0; i < glob_command_list->size; ++i)
        {
            if (!strcmp(glob_command_list->list[i]->name, name))
                return -1;
        }
    }

    struct command *com = calloc(1, sizeof(struct command));
    com->name = name;
    com->fct_ptr = command;
    if (glob_command_list == NULL)
    {
        glob_command_list = calloc(1, sizeof(struct command_list));
        glob_command_list->list = calloc(1, sizeof(struct command));
        glob_command_list->size = 1;
        glob_command_list->list[0] = com;
    }
    else
    {
        glob_command_list->list =
            realloc(glob_command_list->list,
                    (glob_command_list->size + 1) * sizeof(struct command));
        glob_command_list->list[glob_command_list->size] = com;
        ++(glob_command_list->size);
    }
    return 0;
}

int del_command(const char *name)
{
    if (name == NULL || glob_command_list == NULL)
        return -1;

    if (glob_command_list->size)
    {
        for (size_t i = 0; i < glob_command_list->size; ++i)
        {
            if (!strcmp(glob_command_list->list[i]->name, name))
            {
                free(glob_command_list->list[i]);
                for (size_t j = i; j < glob_command_list->size - 1; ++j)
                    glob_command_list->list[j] = glob_command_list->list[j + 1];
                --(glob_command_list->size);
                return 0;
            }
        }
    }
    return -1;
}

int run_command(int argc, char **argv)
{
    if (!argc || argv == NULL || argv[0] == NULL)
        return 127;

    if (glob_command_list->size)
    {
        for (size_t i = 0; i < glob_command_list->size; ++i)
        {
            if (!strcmp(glob_command_list->list[i]->name, argv[0]))
            {
                return ((glob_command_list->list[i]->fct_ptr)(argc, argv));
            }
        }
    }
    return 127;
}
