/*
 * simple tree struct
 */

#ifndef _CUTILS_BTREE_
#define _CUTILS_BTREE_

typedef struct _btree_elem_t btree_elem_t;

typedef int (*btree_compare_handler)(void *node, void *comp);
typedef void (*btree_free_handler)(void *user_data);
typedef int (*btree_user_handler)(void *user_ctx, void *user_data);


typedef struct _btree_node_t {
    btree_elem_t *left;
    btree_elem_t *right;
} btree_node_t;


typedef struct _btree_root_t {
    btree_elem_t *root_node;
    unsigned int user_size;
    btree_compare_handler tree_compare;
    btree_free_handler tree_free;
    btree_user_handler tree_user;
} btree_root_t;


struct _btree_elem_t {
    btree_node_t node;
    btree_elem_t *parent;
    unsigned char user_data[0];
};

btree_root_t *btree_new(unsigned int user_size,
    btree_compare_handler compare_handler);
btree_elem_t *btree_add(btree_root_t *root, void *add_data);
void btree_del(btree_root_t *root, btree_elem_t *telem);
btree_elem_t *btree_search(btree_root_t *root, void *search_data);

#endif
