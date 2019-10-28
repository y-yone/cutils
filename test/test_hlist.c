#include <stdio.h>
#include "hash/hash.h"

int main(int argc, char *argv[])
{
    int ret;
    hash_t *hash;
    hash_elem_t *elem;
    
    char test1[] = "test1";
    char test2[] = "test2";
    char test3[] = "test1";

    hash = hash_new(256, 0);
    if (hash == NULL) {
        printf("hash_new fail\n");
    }

    elem = hash_add(hash, test1, sizeof(test1), NULL);
    elem = hash_add(hash, test2, sizeof(test2), NULL);

    elem = hash_search(hash, test3, sizeof(test3), NULL);
    if (elem->hash_data != test1) {
        printf("hash search fail\n");
    }

    hash_free(hash);
    return 0;
}