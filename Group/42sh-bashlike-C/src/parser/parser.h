#ifndef PARSER_H
#define PARSER_H

#include <stdbool.h>

#include "lexer/lexer.h"
#include "lexer/token/token.h"
#include "utils/ast.h"
#include "utils/list.h"

unsigned *contain_dollar(char *data);
char *find_val(char *data, unsigned index);
struct global *parse_input(struct lexer *lexer);
struct ast *parse_cond(struct lexer *lexer, int parse_code, bool in_pipe);
#endif /* !PARSER_H */
