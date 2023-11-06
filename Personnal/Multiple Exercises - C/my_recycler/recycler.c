#include "recycler.h"

#include <stddef.h>
#include <stdlib.h>

struct recycler *recycler_create(size_t block_size, size_t total_size)
{
    if (total_size % block_size || (block_size % sizeof(size_t)) || !total_size
        || !block_size)
        return NULL;
    void *blocks = malloc(total_size);

    struct recycler *rec = malloc(sizeof(struct recycler));

    rec->block_size = block_size;
    rec->capacity = total_size;
    rec->chunk = blocks;
    rec->free = blocks;
    struct free_list *fll = rec->free;
    for (unsigned i = block_size; i < total_size; i += block_size)
    {
        char *blocks_t = blocks;
        void *tmp_y = i + blocks_t;
        fll->next = tmp_y;
        fll = fll->next;
    }
    fll->next = NULL;

    return rec;
}

void recycler_destroy(struct recycler *r)
{
    if (r != NULL)
    {
        free(r->chunk);
        free(r);
    }
}

void *recycler_allocate(struct recycler *r)
{
    if (r == NULL)
        return NULL;
    for (unsigned i = 0; i <= r->capacity; i += r->block_size)
    {
        struct free_list *f_l = r->free;
        struct free_list *f_l_bef = NULL;
        ;
        while (f_l != NULL)
        {
            char *tmp = r->chunk;
            void *tmp_r_2 = f_l;
            char *tmp_2 = tmp_r_2;
            if (tmp_2 == (i + tmp))
            {
                if (f_l_bef == NULL)
                    r->free = f_l->next;
                else
                    f_l_bef->next = f_l->next;

                return f_l;
            }
            f_l_bef = f_l;
            f_l = f_l->next;
        }
    }
    return NULL;
}

void recycler_free(struct recycler *r, void *block)
{
    if (r != NULL)
    {
        struct free_list *f_l = block;
        f_l->next = r->free;
        r->free = f_l;
    }
}
