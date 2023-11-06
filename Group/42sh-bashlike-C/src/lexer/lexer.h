#ifndef LEXER_H
#define LEXER_H

#include <stddef.h>

#include "token/token.h"

struct lexer
{
    char *input;
    size_t pos;
    struct token *token;
};

struct lexer *new_lexer(char *input);

void free_lexer(struct lexer *lexer);

struct token *peek_lexer(struct lexer *lexer);

void lexer_destroy(struct lexer **lexer);

struct token *get_next_token(struct lexer **lexer);

#endif /* !LEXER_H */
