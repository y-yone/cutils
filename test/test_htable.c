#include <stdio.h>
#include "htable/htable.h"

int main(int argc, char *argv[])
{
    int ret;
    htable_t *htable;
    htable_elem_t *elem;
    
    char test1[] = "test1";
    char test2[] = "test2";
    char test3[] = "test1";

    htable = htable_new(256, 0);
    if (htable == NULL) {
        printf("htable_new fail\n");
    }

    elem = htable_add(htable, test1, sizeof(test1), NULL);
    elem = htable_add(htable, test2, sizeof(test2), NULL);

    elem = htable_search(htable, test3, sizeof(test3), NULL);
    if (elem->htable_data != test1) {
        printf("htable search fail\n");
    }

    htable_free(htable);
    return 0;
}