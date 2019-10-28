/*
 * simple hash struct
 */

#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "htable/htable.h"


static unsigned int default_htable_calc(void *data, unsigned int len,
    void *user_ctx)
{
    unsigned int i;
    unsigned int hash_value = 0;

    // temporary
    for (i = 0; i < len; i++) {
        hash_value *= 10;
        hash_value += *((unsigned char*)(data)+1);
    }

    return hash_value;
}


static int default_htable_match(void *comp_data, void *htable_data,
    unsigned int len, void *user_data)
{
    return memcmp(comp_data, htable_data, len) == 0;
}


htable_t *htable_new(unsigned int htable_size, unsigned int user_size)
{
    htable_t *htable;
    int msize;
    unsigned int i;
    dlist_t *hlist;

    if (htable_size == 0) {
        return NULL;
    }

    msize = sizeof(htable_t) + (sizeof(dlist_t)*(htable_size));
    htable = malloc(msize);
    if (htable == NULL) {
        return NULL;
    }

    memset(htable, 0, msize);

    htable->htable_size = htable_size;
    htable->htable_calc = default_htable_calc;
    htable->htable_free = NULL;
    htable->htable_match = default_htable_match;
    htable->htable_user = NULL;
    htable->user_size = user_size;

    for (i = 0; i < htable_size; i++) {
        hlist = &htable->htable_strage[i];
        dlist_init(hlist);
    }

    return htable;
}


void htable_free(htable_t *htable)
{
    unsigned int i;
    unsigned int hsize;
    dlist_t *head;
    htable_elem_t *helem;

    hsize = htable->htable_size;
    
    for (i = 0; i < hsize; i++) {
        head = &htable->htable_strage[i];
        while (!dlist_is_empty(head)) {
            helem = (htable_elem_t*)dlist_link_pop_head(head);
            if (helem == NULL) {
                continue;
            }

            if (htable->htable_free != NULL) {
                htable->htable_free(helem->htable_data, helem->user_data);
            }

            dlist_unlink(&helem->list);
            free(helem);
        }
    }

    free(htable);
}


htable_elem_t *htable_add(htable_t *htable, void *htable_data,
    unsigned int len, void *user_ctx)
{
    int ret;
    unsigned int hvalue, ihash;
    htable_elem_t *helem;
    dlist_t *head;

    hvalue = htable->htable_calc(htable_data, len, user_ctx);

    helem = malloc(sizeof(htable_elem_t) + htable->user_size);
    if (helem == NULL) {
        return NULL;
    }

    dlist_init(&helem->list);
    helem->htable_value = hvalue;
    helem->htable_data = htable_data;
    helem->htable_len = len;

    if (htable->htable_user != NULL) {
        ret = htable->htable_user(user_ctx, helem->user_data);
        if (ret != 0) {
            free(helem);
            return NULL;
        }
    }

    ihash = hvalue % htable->htable_size;
    head = &htable->htable_strage[ihash];
    dlist_link_tail(head, &helem->list);

    return helem;
}


void htable_del(htable_t *htable, htable_elem_t *htable_elem)
{
    dlist_unlink(&htable_elem->list);

    if (htable->htable_free != NULL) {
        htable->htable_free(htable_elem->htable_data, htable_elem->user_data);
    }
            
    free(htable_elem);
    return;
}


void htable_unlink(htable_elem_t *htable_elem)
{
    dlist_unlink(&htable_elem->list);
    return;
}


htable_elem_t *htable_search(htable_t *htable, void *htable_data,
    unsigned int len, void *search_ctx)
{
    unsigned int vhash, ihash;
    dlist_t *head;
    htable_elem_t *elem;

    vhash = htable->htable_calc(htable_data, len, search_ctx);
    ihash = vhash % htable->htable_size;
    head = &htable->htable_strage[ihash];
    elem = (htable_elem_t*)dlist_link_next(head);

    while ((void*)elem != (void*)head) {
        if (elem->htable_value == vhash) {
            if (elem->htable_len == len
                && htable->htable_match(htable_data, elem->htable_data, len,
                                        elem->user_data))
            {
                break;
            }
        }

        elem = (htable_elem_t*)dlist_link_next(&elem->list);
    }

    if ((void*)elem == (void*)head) {
        return NULL;
    }

    return elem;
}
