#include "gtree.h"

#include <stdlib.h>

struct gtree *gtree_create_node(char data)
{
    struct gtree *gtree = malloc(sizeof(struct gtree));
    gtree->data = data;
    gtree->children = NULL;
    gtree->siblings = NULL;

    return gtree;
}

int add_siblin(struct gtree *children, struct gtree *siblings)
{
    if (children == NULL)
        return 0;
    if (children->siblings != NULL)
        return (add_siblin(children->siblings, siblings));
    children->siblings = siblings;
    return 1;
}

int gtree_add_child(struct gtree *parent, struct gtree *children)
{
    if (parent == NULL || children == NULL)
        return 0;
    if (parent->children == NULL)
    {
        parent->children = children;
        return 1;
    }
    else if (add_siblin(parent->children, children))
        return 1;
    return 1;
}

struct gtree *gtree_search_node(struct gtree *root, char data)
{
    if (root == NULL)
        return NULL;
    if (root->data == data)
        return root;
    struct gtree *tmp = gtree_search_node(root->children, data);
    if (tmp != NULL)
        return tmp;
    struct gtree *tmp2 = gtree_search_node(root->siblings, data);
    if (tmp2 != NULL)
        return tmp2;

    return NULL;
}
void gtree_free_2(struct gtree *root)
{
    if (root != NULL)
    {
        if (root->children == NULL && root->siblings == NULL)
        {
            free(root);
        }
        else if (root->children == NULL)
        {
            gtree_free_2(root->siblings);
            free(root);
        }
        else
        {
            gtree_free_2(root->children);
            root->children = NULL;
            gtree_free_2(root);
        }
    }
}

void gtree_free(struct gtree *root)
{
    if (root != NULL)
    {
        if (root->children == NULL)
        {
            free(root);
        }
        else
        {
            gtree_free_2(root->children);
            root->children = NULL;
            free(root);
        }
    }
}

#include <stdio.h>

int gtree_del_node_b(struct gtree *parent, struct gtree *root, char data)
{
    if (root == NULL)
        return 0;
    if (root->data == data)
    {
        parent->children = root->siblings;
        gtree_free(root);
        return 1;
    }
    else
    {
        struct gtree *sib = root->siblings;
        struct gtree *sib_p = root;
        while (sib != NULL)
        {
            if (sib->data == data)
            {
                sib_p->siblings = sib->siblings;
                gtree_free(sib);
                return 1;
            }
            else
            {
                sib = sib->siblings;
                sib_p = sib_p->siblings;
            }
        }
    }
    return (gtree_del_node_b(root, root->children, data));
}

int gtree_del_node(struct gtree *root, char data)
{
    if (root == NULL)
        return 0;
    if (root->data == data)
        return -1;
    else
        return gtree_del_node_b(root, root->children, data);
}
