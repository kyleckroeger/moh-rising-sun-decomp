// The end of EA's big-file header code (locatbig.cpp): BIG_locateentry forwards
// to BIG_locateentryz, which comes before these and is not part of this unit;
// BIG_locateoffset and BIG_locateoffsetz fetch an entry's offset by name,
// BIG_locate and BIG_locatez turn that into an address, and BIG_count reads the
// big-endian entry count. The function names come from the symbols; the header
// layouts and the parameter meanings of the entry lookups are inferred.
extern "C" {
int BIG_typeofheader(void* header);
void* BIG_locateentryz(void* header, const char* name, int index, int* offset, int* size);

void* BIG_locateentry(void* header, const char* name, int index, int* offset, int* size) {
    return BIG_locateentryz(header, name, index, offset, size);
}

unsigned int BIG_locateoffset(void* header, const char* name) {
    int offset = 0;

    if (name)
        BIG_locateentry(header, name, 0, &offset, 0);
    return offset;
}

unsigned int BIG_locateoffsetz(void* header, const char* name) {
    int offset = 0;

    if (name)
        BIG_locateentryz(header, name, 0, &offset, 0);
    return offset;
}

void* BIG_locate(void* header, const char* name) {
    void* data = 0;
    int offset = BIG_locateoffset(header, name);

    if (offset)
        data = (char*)header + offset;
    return data;
}

void* BIG_locatez(void* header, const char* name) {
    void* data = 0;
    int offset = BIG_locateoffsetz(header, name);

    if (offset)
        data = (char*)header + offset;
    return data;
}

int BIG_count(void* header) {
    const unsigned char* p = (const unsigned char*)header;
    int count = 0;

    switch (BIG_typeofheader(header)) {
    case 1:
        count = p[4] << 8 | p[5];
        break;
    case 2:
    case 3:
        {
            const unsigned char* q = p + 8;

            count = q[0] << 24 | q[1] << 16 | q[2] << 8 | q[3];
        }
        break;
    }
    return count;
}
}
