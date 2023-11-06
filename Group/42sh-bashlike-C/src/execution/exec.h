#ifndef EXEC_H
#define EXEC_H

#include "utils/ast.h"
#include "utils/dict.h"
#include "utils/dict_fun.h"

int exec_subshell(struct ast *ast, int in_cond, int first_call,
                  struct glob_dict **gb_dict);
int exec_blk(struct ast *ast, int in_cond, int first_call,
             struct glob_dict **gb_dict);
int exec_and(struct ast *ast, int in_cond, int first_call,
             struct glob_dict **gb_dict);
int exec_or(struct ast *ast, int in_cond, int first_call,
            struct glob_dict **gb_dict);
int exec_pipe(struct ast *ast, struct glob_dict *gb_dict);
void exec_assign(struct ast *ast, struct dict **dict);
int exec_for(struct ast *ast, int in_cond, int first_call,
             struct glob_dict **gb_dict);
int exec_until(struct ast *ast, int in_cond, int first_call,
               struct glob_dict **gb_dict);
int exec_while(struct ast *ast, int in_cond, int first_call,
               struct glob_dict **gb_dict);
int exec_if(struct ast *ast, int in_cond, int first_call,
            struct glob_dict **gb_dict);
int exec(struct ast *gb_ast, struct glob_dict **gb_dict, int argc, char **argv);

#endif /* !EXEC_H */
