#include <stdio.h>
#include "lru/lru.h"


int main(int argc, char *argv[])
{
    lru_t *lru;
    lru_elem_t elem1;
    lru_elem_t elem2;
    lru_elem_t elem3;
    lru_elem_t elem4;
    lru_elem_t elem5;
    lru_elem_t elem6;
    lru_elem_t elem7;

    elem1.ctx = NULL;
    elem2.ctx = NULL;
    elem3.ctx = NULL;
    elem4.ctx = NULL;
    elem5.ctx = NULL;
    elem6.ctx = NULL;
    elem7.ctx = NULL;

    lru = lru_new(5);
    lru_add(lru, &elem1);
    lru_add(lru, &elem2);
    lru_add(lru, &elem3);
    lru_add(lru, &elem4);
    lru_add(lru, &elem5);
    lru_add(lru, &elem6);

    lru_del(lru, &elem4);

    lru_refresh(lru, &elem3);

    return 0;
}