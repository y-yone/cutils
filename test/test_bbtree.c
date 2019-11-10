#include <stdio.h>
#include "tree/bbtree.h"

int compare(void *ctx, void *data)
{
    int a, b;

    a = *(int *)ctx;
    b = *(int *)data;

    return b - a;
}

void tree_free(void *udata)
{
    return;
}


int tree_build(void *uctx, void *udata)
{
    int *usr, *build;

    usr = uctx;
    build = udata;

    *usr = *build;
    return 0;
}


int main(int argc, char *argv[])
{
    int ret;
    bbtree_root_t *root;
    bbtree_elem_t *elem;
    
    int a = 1;
    int b = 2;
    int c = 3;
    int d = 4;
    int e = 5;
    int f = 6;
    int g = 7;
    int h = 8;
    int i = 9;
    int j = 10;

    root = bbtree_new(sizeof(int), compare);
    if (root == NULL) {
        printf("bbtree_new fail\n");
    }

    root->tree_user = tree_build;
    root->tree_free = tree_free;

    elem = bbtree_add(root, &a);
    elem = bbtree_add(root, &b);
    elem = bbtree_add(root, &c);
    elem = bbtree_add(root, &d);
    elem = bbtree_add(root, &e);
    elem = bbtree_add(root, &f);
    elem = bbtree_add(root, &g);
    elem = bbtree_add(root, &h);
    elem = bbtree_add(root, &i);
    elem = bbtree_add(root, &j);

    elem = bbtree_search(root, &a);
    if (elem != NULL) {
        printf("elem data %d\n", *(int*)elem->user_data);
    }

    bbtree_del(root, elem);
    
    elem = bbtree_search(root, &a);
    if (elem == NULL) {
        printf("elem data not found\n");
    }

    elem = bbtree_search(root, &b);
    if (elem != NULL) {
        printf("elem data %d\n", *(int*)elem->user_data);
    }

    bbtree_del(root, elem);

    return 0;
}