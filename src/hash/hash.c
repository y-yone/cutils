/*
 * simple hash struct
 */

#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "hash/hash.h"


static unsigned int default_hash_calc(void *data, unsigned int len)
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


static int default_hash_match(void *comp_data, void *hash_data,
    unsigned int len, void *user_data)
{
    return memcmp(comp_data, hash_data, len) == 0;
}


hash_t *hash_new(unsigned int hash_size, unsigned int user_size)
{
    hash_t *hash;
    int msize;
    unsigned int i;
    dlist_t *hlist;

    if (hash_size == 0) {
        return NULL;
    }

    msize = sizeof(hash_t) + (sizeof(dlist_t)*(hash_size));
    hash = malloc(msize);
    if (hash == NULL) {
        return NULL;
    }

    memset(hash, 0, msize);

    hash->hash_size = hash_size;
    hash->hash_calc = default_hash_calc;
    hash->hash_free = NULL;
    hash->hash_match = default_hash_match;
    hash->hash_user = NULL;
    hash->user_size = user_size;

    for (i = 0; i < hash_size; i++) {
        hlist = &hash->hash_strage[i];
        dlist_init(hlist);
    }

    return hash;
}


void hash_free(hash_t *hash)
{
    unsigned int i;
    unsigned int hsize;
    dlist_t *head;
    hash_elem_t *helem;

    hsize = hash->hash_size;
    
    for (i = 0; i < hsize; i++) {
        head = &hash->hash_strage[i];
        while (!dlist_is_empty(head)) {
            helem = (hash_elem_t*)dlist_link_pop_head(head);
            if (helem == NULL) {
                continue;
            }

            if (hash->hash_free != NULL) {
                hash->hash_free(helem->hash_data, helem->user_data);
            }

            dlist_unlink(&helem->list);
            free(helem);
        }
    }

    free(hash);
}


int hash_add(hash_t *hash, void *hash_data, unsigned int len, void *user_ctx)
{
    int ret;
    unsigned int hvalue, ihash;
    hash_elem_t *helem;
    dlist_t *head;

    hvalue = hash->hash_calc(hash_data, len);

    helem = malloc(sizeof(hash_elem_t) + hash->user_size);
    if (helem == NULL) {
        return -ENOMEM;
    }

    dlist_init(&helem->list);
    helem->hash_value = hvalue;
    helem->hash_data = hash_data;
    helem->hash_len = len;

    if (hash->hash_user != NULL) {
        ret = hash->hash_user(user_ctx, helem->user_data);
        if (ret != 0) {
            free(helem);
            return ret;
        }
    }

    ihash = hvalue % hash->hash_size;
    head = &hash->hash_strage[ihash];
    dlist_link_tail(head, &helem->list);

    return 0;
}


void hash_del(hash_t *hash, hash_elem_t *hash_elem)
{
    dlist_unlink(&hash_elem->list);

    if (hash->hash_free != NULL) {
        hash->hash_free(hash_elem->hash_data, hash_elem->user_data);
    }
            
    free(hash_elem);
    return;
}


void hash_unlink(hash_t *hash, hash_elem_t *hash_elem)
{
    dlist_unlink(&hash_elem->list);
    return;
}




void *hash_search(hash_t *hash, void *hash_data, unsigned int len)
{
    unsigned int vhash, ihash;
    dlist_t *head;
    hash_elem_t *elem;

    vhash = hash->hash_calc(hash_data, len);
    ihash = vhash % hash->hash_size;
    head = &hash->hash_strage[ihash];
    elem = (hash_elem_t*)dlist_link_next(head);

    while ((void*)elem != (void*)head) {
        if (elem->hash_value == vhash) {
            if (elem->hash_len == len
                && hash->hash_match(hash_data, elem->hash_data, len,
                                    elem->user_data))
            {
                break;
            }
        }

        elem = (hash_elem_t*)dlist_link_next(&elem->list);
    }

    if ((void*)elem == (void*)head) {
        return NULL;
    }

    return elem->hash_data;
}