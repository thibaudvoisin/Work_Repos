#ifndef BUILTIN_H
#define BUILTIN_H
#include "utils/dict.h"

void exec_echo(char **argv);
void exec_exit(char **argv, int last_code);
void exec_cd(char **argv, struct dict **dict);
void exec_unset(char **argv, struct dict **dict);

#endif /* !BUILTIN_H */
