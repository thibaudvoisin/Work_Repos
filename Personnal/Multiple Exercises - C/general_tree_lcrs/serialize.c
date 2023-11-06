#include "serialize.h"

#include <err.h>
#include <stdio.h>
#include <stdlib.h>

#include "gtree.h"
#include "print.h"

int strlen_(char *str)
{
    int i = 0;
    while (str[i])
        ++i;
    return i;
}

int nb_node(struct gtree *tree)
{
    if (tree == NULL)
        return 0;
    int i = nb_node(tree->children);
    i += nb_node(tree->siblings);
    return i + 1;
}

char *build_str(char *str, struct gtree *parent, struct gtree *root);

char *build_str_b(char *str, struct gtree *parent, struct gtree *root)
{
    if (root == NULL)
        return str;

    str[strlen_(str)] = parent->data;
    str[strlen_(str)] = ' ';
    str[strlen_(str)] = '-';
    str[strlen_(str)] = '>';
    str[strlen_(str)] = ' ';
    str[strlen_(str)] = root->data;
    str[strlen_(str)] = ';';
    str[strlen_(str)] = '\n';

    str = build_str_b(str, parent, root->siblings);
    return str;
}
char *build_str_3(char *str, struct gtree *parent, struct gtree *root)
{
    if (root == NULL)
        return str;

    str = build_str(str, root, root->children);
    str = build_str_3(str, parent, root->siblings);
    return str;
}
char *build_str(char *str, struct gtree *parent, struct gtree *root)
{
    if (root == NULL)
        return str;

    str[strlen_(str)] = parent->data;
    str[strlen_(str)] = ' ';
    str[strlen_(str)] = '-';
    str[strlen_(str)] = '>';
    str[strlen_(str)] = ' ';
    str[strlen_(str)] = root->data;
    str[strlen_(str)] = ';';
    str[strlen_(str)] = '\n';

    str = build_str_b(str, parent, root->siblings);
    str = build_str(str, root, root->children);
    str = build_str_3(str, root, root->siblings);
    return str;
}

int gtree_serialize(const char *filename, struct gtree *root)
{
    FILE *f = fopen(filename, "w");

    if (f == NULL)
        return (-1);

    int nb_nodes = nb_node(root);

    char *str = malloc(((nb_nodes * nb_nodes) * 10) + 50);

    for (int i = 0; i < (((nb_nodes * nb_nodes) * 10) + 50); ++i)
        str[i] = 0;
    char *strtmp = "digraph tree {\n";
    for (int i = 0; strtmp[i]; ++i)
        str[i] = strtmp[i];

    if (nb_nodes > 1)
        str = build_str(str, root, root->children);
    else if (nb_nodes)
    {
        char tmpp = root->data;
        int strmp = strlen_(str);
        str[strmp] = tmpp;
        str[strlen_(str)] = ';';
        str[strlen_(str)] = '\n';
    }

    str[strlen_(str)] = '}';
    str[strlen_(str)] = '\n';
    fprintf(f, str);
    free(str);
    fclose(f);
    return 0;
}

struct gtree *gtree_deserialize(const char *filename)
{
    FILE *f = fopen(filename, "r");

    if (f == NULL)
        return (NULL);

    int mode = 0;
    char buf;
    char par;
    char child;

    char *buf_ = malloc(3);
    buf_[0] = 0;
    buf_[1] = 0;

    struct gtree *glob = NULL;

    while ((fgets(buf_, 2, f)) != NULL)
    {
        buf = buf_[0];
        switch (mode)
        {
        case 0:
            if (buf == '\n')
                mode = 1;
            break;
        case 1:
            par = buf;
            mode = 2;
            break;
        case 2:
            if (buf == ';')
            {
                struct gtree *tmp = gtree_create_node(par);
                fclose(f);
                free(buf_);
                return tmp;
            }
            mode = 3;
            break;
        case 3:
            if (buf == ' ')
                mode = 4;
            break;
        case 4:
            child = buf;
            struct gtree *tmp = gtree_search_node(glob, par);
            if (tmp == NULL)
                tmp = gtree_create_node(par);

            struct gtree *tmp2 = gtree_search_node(glob, child);
            if (tmp2 == NULL)
                tmp2 = gtree_create_node(child);
            gtree_add_child(tmp, tmp2);

            if (glob == NULL)
                glob = tmp;

            mode = 5;
            break;
        default:
            if (buf == '\n')
                mode = 1;
            break;
        }
    }
    fclose(f);
    free(buf_);
    return glob;
}
/*
int main()
{
    struct gtree *a = gtree_create_node('a');
    struct gtree *b = gtree_create_node('b');
    struct gtree *c = gtree_create_node('c');
    struct gtree *d = gtree_create_node('d');
    struct gtree *e = gtree_create_node('e');
    struct gtree *f = gtree_create_node('f');
    struct gtree *g = gtree_create_node('g');
    gtree_add_child(d, g);
    gtree_add_child(c, d);
    gtree_add_child(b, c);
    gtree_add_child(e, f);
    gtree_add_child(b, e);
    gtree_add_child(a, b);
        printf("%d\n", gtree_serialize("filename", a));
        gtree_print_preorder(a);
    gtree_del_node(a, 'c');
        gtree_print_preorder(a);
    //    struct gtree *tmp = gtree_search_node(a, 'u');

        printf("%d\n", gtree_serialize("filename3", a));

//        struct gtree *res__s = gtree_deserialize("filename");
 //       if (res__s == NULL)
  //          errx(1, "nul");
  //      printf("%d\n", gtree_serialize("filename2", res__s));

    gtree_free(a);
   // gtree_free(res__s);
    return 0;
}
*/
