#include <stdio.h>
#include "hash/hash.h"

int main(int argc, char *argv[])
{
    int ret;
    hash_t *hash;
    
    char test1[] = "test1";
    char test2[] = "test2";
    char test3[] = "test1";
    char *test;

    hash = hash_new(256, 0);
    if (hash == NULL) {
        printf("hash_new fail\n");
    }

    ret = hash_add(hash, test1, sizeof(test1), NULL);
    ret = hash_add(hash, test2, sizeof(test2), NULL);

    test = hash_search(hash, test3, sizeof(test3));
    if (test != test1) {
        printf("hash search fail %d\n", ret);
    }

    hash_free(hash);
    return 0;
}