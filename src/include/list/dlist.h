/*
 * doubly-linked struct
 */

typedef struct _dlink_list {
    struct _dlink_list *prev;
    struct _dlink_list *next;
} dlist_t;

void dlist_init(dlist_t *elem);
void dlist_link_head(dlist_t *head, dlist_t *elem);
void dlist_link_tail(dlist_t *head, dlist_t *elem);
void dlist_link_insert_next(dlist_t *insrt, dlist_t *elem);
void dlist_link_insert_prev(dlist_t *insrt, dlist_t *elem);
void dlist_unlink(dlist_t *elem);
dlist_t *dlist_link_pop_head(dlist_t *head);
dlist_t *dlist_link_pop_tail(dlist_t *head);
dlist_t *dlist_link_next(dlist_t *elem);
dlist_t *dlist_link_prev(dlist_t *elem);

int dlist_is_empty(dlist_t *head);