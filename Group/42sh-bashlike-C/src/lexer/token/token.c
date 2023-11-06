#include "token.h"

#include "utils/alloc.h"

struct token *new_token(enum token_type type, char *data)
{
    struct token *token = zalloc(sizeof(struct token));
    token->type = type;
    token->data = data;
    return token;
}

void free_token(struct token *token)
{
    if (!token)
        return;
    free(token->var);
    free(token->data);
    free(token);
}
