/*
 * simple hash struct
 */

#include "list/dlist.h"

typedef struct _hash_elem_t hash_elem_t;

typedef unsigned int (*hash_calc_handler)(void *data, unsigned int len);
typedef void (*hash_free_handler)(void *hash_data, void *user_data);
typedef int (*hash_match_handler)(void *comp_data, void *hash_data,
    unsigned int len, void *user_data);
typedef int (*hash_user_handler)(void *user_ctx, void *user_data);

typedef struct _hash_t {
    unsigned int hash_size;
    unsigned int user_size;
    hash_calc_handler hash_calc;
    hash_free_handler hash_free;
    hash_match_handler hash_match;
    hash_user_handler hash_user;
    dlist_t hash_strage[0];
} hash_t;


struct _hash_elem_t {
    dlist_t list;
    unsigned int hash_value;
    unsigned int hash_len;
    void *hash_data;
    unsigned char user_data[0];
};


hash_t *hash_new(unsigned int hash_size, unsigned int user_size);
void hash_free(hash_t *hash);
int hash_add(hash_t *hash, void *hash_data, unsigned int len, void *user_ctx);
void hash_del(hash_t *hash, hash_elem_t *hash_elem);
void hash_unlink(hash_t *hash, hash_elem_t *hash_elem);
void *hash_search(hash_t *hash, void *hash_data, unsigned int len);
