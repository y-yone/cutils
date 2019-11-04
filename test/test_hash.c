#include <stdio.h>
#include "hash/hash.h"

void print_hash(void *data, int len)
{
    int hash;

    hash = hashing_data(data, len);
    printf("%s hash %x\n", (char*)data, hash);
    return;
}

int main(int argc, char *argv[])
{
    int ret;
    char test1[] = "a";
    char test2[] = "b";
    char test3[] = "c";
    char test4[] = "d";
    char test5[] = "e";
    char test6[] = "f";

    char test7[] = "abcd1efghi";
    char test8[] = "abcd2efghi";
    char test9[] = "abcd3efghi";
    char test10[] = "abcd4efghi";
    char test11[] = "abcd5efghi";
    char test12[] = "abcd6efghi";

    print_hash(test1, 1);
    print_hash(test2, 1);
    print_hash(test3, 1);
    print_hash(test4, 1);
    print_hash(test5, 1);
    print_hash(test6, 1);
    
    print_hash(test7, 10);
    print_hash(test8, 10);
    print_hash(test9, 10);
    print_hash(test10, 10);
    print_hash(test11, 10);
    print_hash(test12, 10);
    return 0;
}