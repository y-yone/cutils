/*
 * simple balanced binary tree struct
 */

#include <stdlib.h>
#include <string.h>
#include "tree/bbtree.h"


enum ROTATE_SIDE {
    NONE,
    RIGHT,
    LEFT
};

bbtree_root_t *bbtree_new(unsigned int user_size,
    bbtree_compare_handler compare_handler)
{
    bbtree_root_t *root;
    int msize;

    if (compare_handler == NULL) {
        return NULL;
    }

    msize = sizeof(bbtree_root_t) + user_size;
    root = malloc(msize);
    if (root == NULL) {
        return NULL;
    }

    memset(root, 0, msize);

    root->user_size = user_size;
    root->tree_compare = compare_handler;

    return root;
}


static void rotate_right(bbtree_root_t *root, bbtree_elem_t *node)
{
    bbtree_elem_t *new_root, *old_root;
    bbtree_elem_t *left, *right, *add_elem, *parent;

    new_root = NULL;
    old_root = node;
    right = node->node.right;
    left = node->node.left;

    /* can not rotate */
    if (left == NULL) {
        return;
    }

    /* search most right node in left tree */
    new_root = left;
    while (left != NULL) {
        new_root = left;
        left = left->node.right;
    }

    parent = new_root->parent;
    if (new_root->node.left != NULL) {
        left = new_root->node.left;
        if (parent != NULL) {
            if (parent == node) {
                parent->node.left = right;
                left->parent = new_root;
            } else {
                parent->node.right = left;
                left->parent = parent;
            }
        }

        new_root->node.left = NULL;
        new_root->parent = NULL;
        new_root->child_num = 0;
    }

    while (parent != NULL && parent != old_root) {
        parent->child_num--;
        parent = parent->parent;
    }

    /* switch root */
    new_root->child_num = old_root->child_num;
    new_root->parent = old_root->parent;
    new_root->node = old_root->node;
    if (new_root->node.left != NULL) {
        new_root->node.left->parent = new_root;
    }
    if (new_root->node.right != NULL) {
        new_root->node.right->parent = new_root;
    }

    parent = old_root->parent;
    if (parent != NULL) {
        if (parent->node.left == old_root) {
            parent->node.left = new_root;
        } else {
            parent->node.right = new_root;
        }
    } else {
        root->root_node = new_root;
    }
    
    old_root->child_num = 0;
    old_root->parent = NULL;
    old_root->node.right = NULL;
    old_root->node.left = NULL;

    /* add old root to right node most left position */
    add_elem = new_root;
    left = right;
    while (left != NULL) {
        add_elem = left;
        left = left->node.left;
    }

    if (add_elem == new_root) {
        add_elem->node.right = old_root;
    } else {
        add_elem->node.left = old_root;
    }
    old_root->parent = add_elem;

    parent = add_elem;
    while (parent != NULL && parent != new_root) {
        parent->child_num++;
        parent = parent->parent;
    }
}


static void rotate_left(bbtree_root_t *root, bbtree_elem_t *node)
{
    bbtree_elem_t *new_root, *old_root;
    bbtree_elem_t *left, *right, *add_elem, *parent;

    new_root = NULL;
    old_root = node;
    right = node->node.right;
    left = node->node.left;

    /* can not rotate */
    if (right == NULL) {
        return;
    }

    /* search most left node in right tree */
    new_root = right;
    while (right != NULL) {
        new_root = right;
        right = right->node.left;
    }

    parent = new_root->parent;
    if (new_root->node.right != NULL) {
        right = new_root->node.right;

        if (parent != NULL) {
            if (parent == node) {
                parent->node.right = right;

            } else {
                parent->node.left = right;
            }

            right->parent = parent;
        }

        new_root->node.right = NULL;
        new_root->parent = NULL;
        new_root->child_num = 0;
    }

    while (parent != NULL && parent != old_root) {
        parent->child_num--;
        parent = parent->parent;
    }

    /* switch root */
    new_root->child_num = old_root->child_num;
    new_root->parent = old_root->parent;
    new_root->node = old_root->node;
    if (new_root->node.left != NULL) {
        new_root->node.left->parent = new_root;
    }
    if (new_root->node.right != NULL) {
        new_root->node.right->parent = new_root;
    }

    parent = old_root->parent;
    if (parent != NULL) {
        if (parent->node.right == old_root) {
            parent->node.right = new_root;
        } else {
            parent->node.left = new_root;
        }
    } else {
        root->root_node = new_root;
    }
    
    old_root->child_num = 0;
    old_root->parent = NULL;
    old_root->node.right = NULL;
    old_root->node.left = NULL;

    /* add old root to right node most left position */
    add_elem = new_root;
    right = left;
    while (right != NULL) {
        add_elem = right;
        right = right->node.right;
    }

    if (add_elem == new_root) {
        add_elem->node.left = old_root;
    } else {
        add_elem->node.right = old_root;
    }
    old_root->parent = add_elem;

    parent = add_elem;
    while (parent != NULL && parent != new_root) {
        parent->child_num++;
        parent = parent->parent;
    }
}


static void rotate_tree(bbtree_root_t *root, bbtree_elem_t *node,
    enum ROTATE_SIDE rotate)
{
    if (rotate == NONE) {
        return;
    }

    if (rotate == RIGHT) {
        rotate_right(root, node);
    } else {
        rotate_left(root, node);
    }

    return;
}


static void rebalancing_tree(bbtree_root_t *root, bbtree_elem_t *balanced_elem)
{
    bbtree_elem_t *balance_root, *last_root, *right, *left;
    unsigned int right_child, left_child, child_diff;
    enum ROTATE_SIDE rotate, last_rotate;

    rotate = NONE;
    balance_root = balanced_elem->parent;
    last_root = NULL;
    last_rotate = NONE;

    for (;;) {
        right_child = 0;
        left_child = 0;

        if (balance_root == NULL) {
            break;
        }

        if (balance_root->node.right != NULL) {
            right_child = balance_root->node.right->child_num + 1;
        }

        if (balance_root->node.left != NULL) {
            left_child = balance_root->node.left->child_num + 1;
        }

        if (right_child == left_child) {
            balance_root = balance_root->parent;
            continue;
        }

        if (right_child > left_child) {
            child_diff = right_child - left_child;
            rotate = LEFT;
        } else {
            child_diff = left_child - right_child;
            rotate = RIGHT;
        }

        if (child_diff >= 2) {
            last_root = balance_root;
            last_rotate = rotate;
        }

        balance_root = balance_root->parent;
    }

    rotate_tree(root, last_root, last_rotate);

    return;
}


static void insert_element(bbtree_root_t *root, bbtree_elem_t *elem)
{
    bbtree_compare_handler compare;
    bbtree_elem_t *insert_elem, *parent;
    int comp;

    elem->node.left = NULL;
    elem->node.right = NULL;
    elem->child_num = 0;

    insert_elem = root->root_node;

    if (insert_elem == NULL) {
        root->root_node = elem;
        elem->parent = NULL;
        return;
    }

    compare = root->tree_compare;

    /* search insert position */
    for (;;) {
        comp = compare(insert_elem->user_data, elem->user_data);

        if (comp < 0) {
            /* add left tree */
            if (insert_elem->node.left == NULL) {
                insert_elem->node.left = elem;
                elem->parent = insert_elem;
                break;
            }

            insert_elem = insert_elem->node.left;
        } else {
            /* add right tree */
            if (insert_elem->node.right == NULL) {
                insert_elem->node.right = elem;
                elem->parent = insert_elem;
                break;
            }

            insert_elem = insert_elem->node.right;
        }
    }

    /* add child num for earch elements */
    parent = elem->parent;
    while (parent != NULL) {
        parent->child_num++;
        parent = parent->parent;
    }

    return;

}


void bbtree_free(bbtree_root_t *root)
{
}


bbtree_elem_t *bbtree_add(bbtree_root_t *root, void *add_data)
{
    int ret, comp;
    bbtree_elem_t *add_elem;

    add_elem = malloc(sizeof(bbtree_elem_t) + root->user_size);
    if (add_elem == NULL) {
        return NULL;
    }

    ret = root->tree_user(add_elem->user_data, add_data);
    if (ret < 0) {
        free(add_elem);
        return NULL;
    }

    insert_element(root, add_elem);
    rebalancing_tree(root, add_elem);

    return add_elem;
}


void bbtree_del(bbtree_root_t *root, bbtree_elem_t *telem)
{
    bbtree_elem_t *left_d0, *right;
    bbtree_elem_t *new_node, *parent, *recalc_parent;

    parent = telem->parent;
    telem->parent = NULL;

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
            recalc_parent = left_d0->parent;

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
            recalc_parent = right->parent;
            
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

    if (new_node->node.left != NULL) {
        new_node->node.left->parent = new_node;
    }
    if (new_node->node.right != NULL) {
        new_node->node.right->parent = new_node;
    }

    parent = recalc_parent;
    while (parent != NULL) {
        parent->child_num--;
        parent = parent->parent;
    }

    root->tree_free(telem->user_data);
    free(telem);

    return;
}


bbtree_elem_t *bbtree_search(bbtree_root_t *root, void *search_data)
{
    bbtree_compare_handler compare;
    bbtree_elem_t *search_elem;
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
