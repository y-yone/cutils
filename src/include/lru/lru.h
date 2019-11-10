/*
 * simple LRU struct
 */

#ifndef _CUTILS_LRU_
#define _CUTILS_LRU_

#include "list/dlist.h"


typedef void (*lru_free_handler)(void *ctx);

typedef struct _lru_t {
    dlist_t head;
    unsigned int limit;
    unsigned int nelem;
    lru_free_handler lru_free;
} lru_t;

typedef struct _lru_elem_t {
    dlist_t list;
    void *ctx;
} lru_elem_t;


lru_t *lru_new(unsigned int limit);
void lru_free(lru_t *lru);
void lru_add(lru_t *lru, lru_elem_t *lru_elem);
void lru_del(lru_t *lru, lru_elem_t *lru_elem);
void lru_refresh(lru_t *lru, lru_elem_t *lru_elem);

#endif
