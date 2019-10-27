/*
 * doubly-linked struct
 */

#include <stdio.h>

#include "list/dlist.h"

void dlist_init(dlist_t *elem)
{
    elem->next = elem;
    elem->prev = elem;
    return;
}


void dlist_link_head(dlist_t *head, dlist_t *elem)
{
    dlist_t *first;

    first = head->next;

    elem->prev = head;
    elem->next = first;

    first->prev = elem;
    head->next = elem;

    return;
}


void dlist_link_tail(dlist_t *head, dlist_t *elem)
{
    dlist_t *last;

    last = head->prev;

    elem->prev = last;
    elem->next = head;

    head->prev = elem;
    last->next = elem;

    return;
}


void dlist_link_insert_next(dlist_t *insrt, dlist_t *elem)
{
    dlist_t *next;

    next = insrt->prev;

    elem->prev = next;
    elem->next = insrt;

    insrt->prev = elem;
    next->next = elem;

    return;
}


void dlist_link_insert_prev(dlist_t *insrt, dlist_t *elem)
{
    dlist_t *prev;

    prev = insrt->prev;

    elem->prev = prev;
    elem->next = insrt;

    insrt->prev = elem;
    prev->next = elem;

    return;
}

void dlist_unlink(dlist_t *elem)
{
    dlist_t *prev, *next;

    prev = elem->prev;
    next = elem->next;

    prev->next = next;
    next->prev = prev;

    dlist_init(elem);

    return;
}


dlist_t *dlist_link_pop_head(dlist_t *head)
{
    dlist_t *elem;

    if (head->next == head) {
        return NULL;
    }

    elem = head->next;

    head->next = elem->next;
    elem->next->prev = head;

    return elem;
}


dlist_t *dlist_link_pop_tail(dlist_t *head)
{
    dlist_t *elem;

    if (head->prev == head) {
        return NULL;
    }

    elem = head->prev;

    head->prev = elem->prev;
    elem->prev->next = head;

    return elem;
}


dlist_t *dlist_link_next(dlist_t *elem)
{
    return elem->next;
}


dlist_t *dlist_link_prev(dlist_t *elem)
{
    return elem->prev;
}


int dlist_is_empty(dlist_t *head)
{
    if (head->next == head) {
        return 1;
    }

    return 0;
}