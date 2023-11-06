#include "print.h"

#include <stdio.h>
#include <stdlib.h>

void gtree_print_preorder_b(struct gtree *root)
{
    if (root == NULL)
        return;
    fprintf(stdout, " %c", root->data);
    if (root->children != NULL)
        gtree_print_preorder_b(root->children);
    gtree_print_preorder_b(root->siblings);
}

void gtree_print_preorder(struct gtree *root)
{
    if (root == NULL)
        return;
    fprintf(stdout, "%c", root->data);
    gtree_print_preorder_b(root->children);
    fprintf(stdout, "\n");
}
