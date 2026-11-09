// A fragment of EA's sound memory manager (smemman.c): SNDMEMI_allocz takes the
// first gap between allocation records large enough for the 16-byte-rounded
// size, moving the lower records down to make room for its record. The table
// grows downward, so the count is negative. The function name comes from the
// symbol; the pool and record views are inferred. The records are reached
// through byte offsets, which puts the table base first in the additions.
struct SNDMEMRECORDVIEW {
    unsigned int offset;
    int size;
};

/* inferred: the sound memory pool header */
struct SNDMEMPOOLVIEW {
    char* base;
    SNDMEMRECORDVIEW* records;
    int size;
    unsigned int free;
    int lowest;
    int count;
};

struct SNDGSVIEW {
    unsigned char unknown000[544];
    SNDMEMPOOLVIEW* pool;
};

extern SNDGSVIEW sndgs;

void SNDMEMI_constrain(unsigned int* start, int* gap);

void* SNDMEMI_allocz(int size) {
    int i = 0;
    int aligned = size + 15;
    unsigned int start;
    int gap;
    SNDMEMRECORDVIEW* prev;

    aligned &= ~15;
    if (sndgs.pool->count == 0) {
        start = 0;
        gap = sndgs.pool->free;
        SNDMEMI_constrain(&start, &gap);
        if (aligned <= gap)
            goto found;
        goto fail;
    } else {
        for (; i > sndgs.pool->count; i--) {
            int index = i * 8;
            char* records = (char*)sndgs.pool->records;

            if (i == 0) {
                start = i;
                gap = *(unsigned int*)(records + index);
            } else {
                int previous = index + 8;

                prev = (SNDMEMRECORDVIEW*)(records + previous);
                start = *(unsigned int*)(records + previous) + prev->size;
                gap = *(unsigned int*)(records + index) - start;
            }
            SNDMEMI_constrain(&start, &gap);
            if (aligned <= gap) {
                int j;

                for (j = sndgs.pool->count; j < i; j++)
                    sndgs.pool->records[j] = sndgs.pool->records[j + 1];
                goto found;
            }
        }
        {
            int previous = i * 8 + 8;
            char* records = (char*)sndgs.pool->records;

            prev = (SNDMEMRECORDVIEW*)(records + previous);
            start = *(unsigned int*)(records + previous) + prev->size;
        }
        gap = sndgs.pool->free - start;
        SNDMEMI_constrain(&start, &gap);
        if (aligned > gap)
            goto fail;
    }
found:
    {
        char* record = (char*)sndgs.pool->records + i * 8;

        *(unsigned int*)record = start;
        ((SNDMEMRECORDVIEW*)record)->size = aligned;
    }
    sndgs.pool->count--;
    sndgs.pool->free -= 8;
    {
        int remaining = sndgs.pool->free - (start + aligned);
        void* result = sndgs.pool->base + start;

        if (remaining < sndgs.pool->lowest)
            sndgs.pool->lowest = remaining;
        return result;
    }
fail:
    return 0;
}
