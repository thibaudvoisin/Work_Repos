#ifndef RUN_COMMAND_H
#define RUN_COMMAND_H

int add_command(const char *name, int (*command)(int, char **));
int del_command(const char *name);
int run_command(int argc, char **argv);

#endif
