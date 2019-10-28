/*
 * simple hash struct
 */

#include "list/dlist.h"

typedef struct _htalbe_elem_t htable_elem_t;

typedef unsigned int (*htable_calc_handler)(void *data, unsigned int len,
    void *user_ctx);
typedef void (*htable_free_handler)(void *hash_data, void *user_data);
typedef int (*htable_match_handler)(void *comp_data, void *hash_data,
    unsigned int len, void *user_data);
typedef int (*htable_user_handler)(void *user_ctx, void *user_data);

typedef struct _hash_t {
    unsigned int htable_size;
    unsigned int user_size;
    htable_calc_handler htable_calc;
    htable_free_handler htable_free;
    htable_match_handler htable_match;
    htable_user_handler htable_user;
    dlist_t htable_strage[0];
} htable_t;


struct _htalbe_elem_t {
    dlist_t list;
    unsigned int htable_value;
    unsigned int htable_len;
    void *htable_data;
    unsigned char user_data[0];
};


htable_t *htable_new(unsigned int htable_size, unsigned int user_size);
void htable_free(htable_t *hash);
htable_elem_t *htable_add(htable_t *htable, void *htable_data, unsigned int len,
    void *user_ctx);
void htable_del(htable_t *htable, htable_elem_t *htable_elem);
void htable_unlink(htable_elem_t *htable_elem);
htable_elem_t *htable_search(htable_t *htable, void *htable_data, unsigned int len,
    void *search_ctx);
