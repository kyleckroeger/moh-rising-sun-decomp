// The ARAM sample-memory manager's allocator (saramman.c): SNDARAM_constrain
// clips a free range to the pool, SNDARAM_alloc takes the first free range
// (before, between or after the sorted allocated ranges) that fits, inserting
// its record, and returns its byte address (0 when full), and SNDARAM_free
// removes a block's record. The function names come from the symbols; sndaram's
// members and the range record are inferred. The records are reached through
// byte offsets, which puts the table base first in the additions.
/* inferred: an allocated range (start and length, in 32-byte units) */
struct SNDARAMBLOCKVIEW {
    unsigned int start;
    unsigned int length;
};

struct SNDARAMVIEW {
    unsigned short count;
    unsigned short max;
    unsigned int start;
    unsigned int end;
    SNDARAMBLOCKVIEW* table;
};

extern "C" SNDARAMVIEW sndaram;

void SNDARAM_constrain(unsigned int* start, int* length) {
    if (*start < sndaram.start) {
        unsigned int skip = sndaram.start - *start;

        *start = sndaram.start;
        *length -= skip;
    }
    if (*start + *length > sndaram.end)
        *length = sndaram.end - *start;
}

int SNDARAM_alloc(int size) {
    int i = 0;
    unsigned int start;
    int length;
    SNDARAMBLOCKVIEW* prev;

    if (sndaram.count >= sndaram.max)
        return 0;
    size += 31;
    size >>= 5;
    if (sndaram.count == 0) {
        start = sndaram.start;
        length = sndaram.end - start;
        SNDARAM_constrain(&start, &length);
        if (size > length)
            return 0;
    } else {
        for (; i < sndaram.count; i++) {
            int index = i * 8;
            char* table = (char*)sndaram.table;

            if (i == 0) {
                start = sndaram.start;
                length = *(unsigned int*)(table + index) - start;
            } else {
                int previous = index - 8;

                prev = (SNDARAMBLOCKVIEW*)(table + previous);
                start = *(unsigned int*)(table + previous) + prev->length;
                length = *(unsigned int*)(table + index) - start;
            }
            SNDARAM_constrain(&start, &length);
            if (size <= length) {
                int j;

                for (j = sndaram.count; j > i; j--)
                    sndaram.table[j] = sndaram.table[j - 1];
                goto found;
            }
        }
        {
            int previous = i * 8 - 8;
            char* table = (char*)sndaram.table;

            prev = (SNDARAMBLOCKVIEW*)(table + previous);
            start = *(unsigned int*)(table + previous) + prev->length;
        }
        length = sndaram.end - start;
        SNDARAM_constrain(&start, &length);
        if (size > length)
            return 0;
    }
found:
    {
        char* block = (char*)sndaram.table + i * 8;

        *(unsigned int*)block = start;
        ((SNDARAMBLOCKVIEW*)block)->length = size;
    }
    sndaram.count++;
    return start << 5;
}

void SNDARAM_free(unsigned int address) {
    unsigned int block = address >> 5;
    int i;

    for (i = 0; i < sndaram.count; i++) {
        int index = i * 8;

        if (*(unsigned int*)((char*)sndaram.table + index) == block) {
            sndaram.count--;
            for (; i < sndaram.count; i++)
                sndaram.table[i] = sndaram.table[i + 1];
            return;
        }
    }
}
