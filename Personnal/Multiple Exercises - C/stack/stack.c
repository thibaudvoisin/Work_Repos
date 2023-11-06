#include "stack.h"

#include <stddef.h>
#include <stdlib.h>

struct stack *stack_push(struct stack *s, int e)
{
    struct stack *b = malloc(sizeof(struct stack));

    b->data = e;
    b->next = s;

    return b;
}

struct stack *stack_pop(struct stack *s)
{
    if (s == NULL)
    {
        struct stack *a = s;
        return a;
    }
    struct stack *a = s->next;
    free(s);
    return a;
}

int stack_peek(struct stack *s)
{
    return s->data;
}
