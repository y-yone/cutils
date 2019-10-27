/*
 * doubly-linked struct
 */

#include <stdio.h>

#include "list/slist.h"


void slist_link_insert(slist_t *insrt, slist_t *elem)
{
    slist_t *next;

    next = insrt->next;
    insrt->next = elem;
    elem->next = next;

    return;
}


slist_t *slist_link_pop(slist_t *head)
{
    slist_t *elem;

    elem = head->next;
    head->next = elem->next;

    elem->next = NULL;
    return elem;
}
