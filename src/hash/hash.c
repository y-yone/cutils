

#define GOLDEN_RATIO 0x61C8864680B583EBull

unsigned int hashing_data(void *data, unsigned int data_bytes)
{
    unsigned long long hash, hdata;
    unsigned int uhash, lhash;
    int u32_len, u8_len;
    int i;
    unsigned int *idata;
    unsigned char *cdata;

    u32_len = data_bytes / 4;
    u8_len = data_bytes % 4;

    hash = 0;
    idata = (unsigned int*)data;

    for (i = 0; i < u32_len; i++) {
        hdata = *idata;
        hdata *= GOLDEN_RATIO;
        uhash = hdata & 0xFFFFFFFF;
        lhash = hdata >> 32;
        hdata = uhash ^ lhash;
        hash ^= hdata;
        idata++;
    }

    cdata = (unsigned char*)idata;
    hdata = 0;

    for (i = 0; i < u8_len; i++) {
        hdata = hdata << 8;
        hdata += *cdata;
        cdata++;
    }

    hdata *= GOLDEN_RATIO;
    uhash = hdata & 0xFFFFFFFF;
    lhash = hdata >> 32;
    hdata = uhash ^ lhash;
    hash ^= hdata;

    return hash;
}
