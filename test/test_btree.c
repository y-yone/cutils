#include <stdio.h>
#include "tree/btree.h"

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
    btree_root_t *root;
    btree_elem_t *elem;
    
    int a = 1;
    int b = 10;
    int c = 5;

    root = btree_new(sizeof(int), compare);
    if (root == NULL) {
        printf("btree_new fail\n");
    }

    root->tree_user = tree_build;
    root->tree_free = tree_free;

    elem = btree_add(root, &a);
    elem = btree_add(root, &b);

    elem = btree_search(root, &a);
    if (elem != NULL) {
        printf("elem data %d\n", *(int*)elem->user_data);
    }

    btree_del(root, elem);
    
    elem = btree_search(root, &a);
    if (elem == NULL) {
        printf("elem data not found\n");
    }

    elem = btree_search(root, &b);
    if (elem != NULL) {
        printf("elem data %d\n", *(int*)elem->user_data);
    }

    btree_del(root, elem);

    return 0;
}