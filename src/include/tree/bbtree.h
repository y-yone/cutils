/*
 * simple balanced binary tree struct
 */

#ifndef _CUTILS_BBTREE_
#define _CUTILS_BBTREE_

typedef struct _bbtree_elem_t bbtree_elem_t;

typedef int (*bbtree_compare_handler)(void *node, void *comp);
typedef void (*bbtree_free_handler)(void *user_data);
typedef int (*bbtree_user_handler)(void *user_ctx, void *user_data);


typedef struct _bbtree_node_t {
    bbtree_elem_t *left;
    bbtree_elem_t *right;
} bbtree_node_t;


typedef struct _bbtree_root_t {
    bbtree_elem_t *root_node;
    unsigned int user_size;
    bbtree_compare_handler tree_compare;
    bbtree_free_handler tree_free;
    bbtree_user_handler tree_user;
} bbtree_root_t;


struct _bbtree_elem_t {
    bbtree_node_t node;
    bbtree_elem_t *parent;
    unsigned int child_num;
    unsigned char user_data[0];
};

bbtree_root_t *bbtree_new(unsigned int user_size,
    bbtree_compare_handler compare_handler);
bbtree_elem_t *bbtree_add(bbtree_root_t *root, void *add_data);
void bbtree_del(bbtree_root_t *root, bbtree_elem_t *telem);
bbtree_elem_t *bbtree_search(bbtree_root_t *root, void *search_data);

#endif
