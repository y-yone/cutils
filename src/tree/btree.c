/*
 * simple binary tree struct
 */

#include <stdlib.h>
#include <string.h>
#include "tree/btree.h"


btree_root_t *btree_new(unsigned int user_size,
    btree_compare_handler compare_handler)
{
    btree_root_t *root;
    int msize;

    if (compare_handler == NULL) {
        return NULL;
    }

    msize = sizeof(btree_root_t) + user_size;
    root = malloc(msize);
    if (root == NULL) {
        return NULL;
    }

    memset(root, 0, msize);

    root->user_size = user_size;
    root->tree_compare = compare_handler;

    return root;
}


void btree_free(btree_root_t *root)
{
}


btree_elem_t *btree_add(btree_root_t *root, void *add_data)
{
    btree_compare_handler compare;
    btree_elem_t *insert_elem, *add_elem;
    int ret, comp;

    add_elem = malloc(sizeof(btree_elem_t) + root->user_size);
    if (add_elem == NULL) {
        return NULL;
    }

    ret = root->tree_user(add_elem->user_data, add_data);
    if (ret < 0) {
        free(add_elem);
        return NULL;
    }

    add_elem->node.left = NULL;
    add_elem->node.right = NULL;

    insert_elem = root->root_node;

    if (insert_elem == NULL) {
        root->root_node = add_elem;
        add_elem->parent = NULL;
        return add_elem;
    }

    compare = root->tree_compare;

    for (;;) {
        comp = compare(insert_elem->user_data, add_data);

        if (comp < 0) {
            /* add left tree */
            if (insert_elem->node.left == NULL) {
                insert_elem->node.left = add_elem;
                add_elem->parent = insert_elem;
                break;
            }

            insert_elem = insert_elem->node.left;
        } else {
            /* add right tree */
            if (insert_elem->node.right == NULL) {
                insert_elem->node.right = add_elem;
                add_elem->parent = insert_elem;
                break;
            }

            insert_elem = insert_elem->node.right;
        }
    }

    return add_elem;
}


void btree_del(btree_root_t *root, btree_elem_t *telem)
{
    btree_elem_t *left_d0, *right, *new_node, *parent;

    parent = telem->parent;
    telem->parent = NULL;
    new_node = NULL;

    if (telem->node.left != NULL) {
        left_d0 = telem->node.left;

        right = left_d0->node.right;
        while (right != NULL) {
            if (right->node.right == NULL) {
                break;
            }
            right = right->node.right;
        }

        if (right == NULL) {
            /* left_d0 is new node */
            new_node = left_d0;
            new_node->node.right = telem->node.right;
            new_node->parent = parent;

            telem->node.left = NULL;
            telem->node.right = NULL;

            if (parent != NULL) {
                if (parent->node.left == telem) {
                    parent->node.left = new_node;
                } else {
                    parent->node.right = new_node;
                }
            } else {
                root->root_node = new_node;
            }
            
        } else {
            if (right->node.left) {
                right->parent->node.right = right->node.left;
                right->node.left->parent = right->parent;
                right->node.left = NULL;
            }

            new_node = right;
            new_node->node.right = telem->node.right;
            new_node->node.left = telem->node.left;
            new_node->parent = parent;

            telem->node.left = NULL;
            telem->node.right = NULL;

            if (parent != NULL) {
                if (parent->node.left == telem) {
                    parent->node.left = new_node;
                } else {
                    parent->node.right = new_node;
                }
            } else {
                root->root_node = new_node;
            }
        }
    } else if (telem->node.right != NULL) {
        /* right is new node */
        new_node = telem->node.right;
        new_node->parent = parent;

        telem->node.left = NULL;
        telem->node.right = NULL;

        if (parent != NULL) {
            if (parent->node.left == telem) {
                parent->node.left = new_node;
            } else {
                parent->node.right = new_node;
            }
        } else {
            root->root_node = new_node;
        }
    } else {
        /* delete last element */
        root->root_node = NULL;
    }

    if (new_node != NULL) {
        if (new_node->node.left != NULL) {
            new_node->node.left->parent = new_node;
        }
        if (new_node->node.right != NULL) {
            new_node->node.right->parent = new_node;
        }
    }

    root->tree_free(telem->user_data);
    free(telem);

    return;
}


btree_elem_t *btree_search(btree_root_t *root, void *search_data)
{
    btree_compare_handler compare;
    btree_elem_t *search_elem;
    int comp;
    
    search_elem = root->root_node;
    compare = root->tree_compare;

    while (search_elem != NULL) {
        comp = compare(search_elem->user_data, search_data);

        if (comp == 0) {
            return search_elem;
        }

        if (comp < 0) {
            search_elem = search_elem->node.left;
        } else {
            search_elem = search_elem->node.right;
        }
    }

    return NULL;
}
