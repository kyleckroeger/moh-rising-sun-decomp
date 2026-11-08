// Big-file headers (locatbig.cpp): BIG_typeofheader recognises a header from its
// big-endian first bytes (0xc0fb, "BIGF" or "BIG" plus any fourth byte) and
// BIG_sizeofheader reads its big-endian size. The function names come from the
// symbols; the header layouts are inferred.
extern "C" int BIG_typeofheader(void* header) {
    const unsigned char* p = (const unsigned char*)header;
    int type = 0;

    if ((p[0] << 8 | p[1]) == 0xc0fb) {
        type = 1;
    } else {
        int magic = p[0] << 24 | p[1] << 16 | p[2] << 8 | p[3];

        if (magic == 0x42494746)
            type = 2;
        else if ((magic & ~0xff) == 0x42494700)
            type = 3;
    }
    return type;
}

extern "C" int BIG_sizeofheader(void* header) {
    const unsigned char* p = (const unsigned char*)header;
    int size = 0;

    switch (BIG_typeofheader(header)) {
    case 1:
        size = (p[2] << 8 | p[3]) + 4;
        break;
    case 2:
    case 3:
        {
            const unsigned char* q = p + 12;

            size = q[0] << 24 | q[1] << 16 | q[2] << 8 | q[3];
        }
        break;
    }
    return size;
}
