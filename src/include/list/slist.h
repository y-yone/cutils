/*
 * singly-linked struct
 */

typedef struct _slink_list {
    struct _slink_list *next;
} slist_t;

#define INIT_SLIST(slist) slist->next = NULL

void slist_link_insert(slist_t *insrt, slist_t *elem);
slist_t *slist_link_pop(slist_t *head);

