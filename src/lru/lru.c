/*
 * simple LRU struct
 */

#include <stdlib.h>
#include "lru/lru.h"


static void dummy_free(void *ctx)
{
    return;
}


lru_t *lru_new(unsigned int limit)
{
    lru_t *lru;
    size_t msize;

    msize = sizeof(lru_t);
    lru = malloc(msize);
    if (lru == NULL) {
        return NULL;
    }

    dlist_init(&lru->head);
    lru->limit = limit;
    lru->nelem = 0;
    lru->lru_free = dummy_free;

    return lru;
}


void lru_free(lru_t *lru)
{
    lru_elem_t *lru_elem;

    while(!dlist_is_empty(&lru->head)) {
        lru_elem = (lru_elem_t*)dlist_link_pop_tail(&lru->head);
        lru->lru_free(lru_elem->ctx);
    }

    return;
}


void lru_add(lru_t *lru, lru_elem_t *lru_elem)
{
    lru_elem_t *pop = NULL;

    if (lru->limit <= lru->nelem) {
        lru->nelem--;
        pop = (lru_elem_t*)dlist_link_pop_tail(&lru->head);
        lru->lru_free(pop->ctx);
    }

    dlist_link_head(&lru->head, &lru_elem->list);
    lru->nelem++;
    return;
}


void lru_del(lru_t *lru, lru_elem_t *lru_elem)
{
    lru->nelem--;
    dlist_unlink(&lru_elem->list);
    lru->lru_free(lru_elem->ctx);

    return;
}


void lru_refresh(lru_t *lru, lru_elem_t *lru_elem)
{
    dlist_unlink(&lru_elem->list);
    dlist_link_head(&lru->head, &lru_elem->list);
    return;
}