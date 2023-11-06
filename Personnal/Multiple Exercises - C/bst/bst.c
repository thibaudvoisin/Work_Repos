#include "bst.h"

#include <stdlib.h>

struct bst_node *create_node(int value)
{
    struct bst_node *bst = malloc(sizeof(struct bst_node));

    bst->data = value;
    bst->left = NULL;
    bst->right = NULL;
    return bst;
}

struct bst_node *add_node(struct bst_node *tree, int value)
{
    if (tree == NULL)
        return (create_node(value));
    if (value > tree->data)
    {
        if (tree->right == NULL)
        {
            tree->right = create_node(value);
        }
        else
            tree->right = (add_node(tree->right, value));
        return tree;
    }
    else if (value < tree->data)
    {
        if (tree->left == NULL)
            tree->left = create_node(value);
        else
            tree->left = (add_node(tree->left, value));
        return tree;
    }
    else
        return tree;
}

struct bst_node *maxValueNode(struct bst_node *node)
{
    struct bst_node *current = node;

    while (current && current->right != NULL)
        current = current->right;

    return current;
}

struct bst_node *delete_node(struct bst_node *root, int key)
{
    if (root == NULL)
        return root;

    if (key < root->data)
        root->left = delete_node(root->left, key);

    else if (key > root->data)
        root->right = delete_node(root->right, key);

    else
    {
        if (root->left == NULL)
        {
            struct bst_node *temp = root->right;
            free(root);
            return temp;
        }
        else if (root->right == NULL)
        {
            struct bst_node *temp = root->left;
            free(root);
            return temp;
        }

        struct bst_node *temp = maxValueNode(root->left);

        root->data = temp->data;

        root->left = delete_node(root->left, temp->data);
    }
    return root;
}

const struct bst_node *find(const struct bst_node *tree, int value)
{
    if (tree == NULL)
        return NULL;
    if (tree->data == value)
        return tree;
    const struct bst_node *res_2 = find(tree->left, value);
    const struct bst_node *res_1 = find(tree->right, value);
    if (res_2 != NULL)
        return res_2;
    if (res_1 != NULL)
        return res_1;
    return NULL;
}

void free_bst(struct bst_node *tree)
{
    if (tree == NULL)
        return;
    free_bst(tree->right);
    free_bst(tree->left);
    free(tree);
}
