// The end of EA's sound memory manager (smemman.c): SNDMEMI_free finds the
// record holding a block's offset, drops the record and closes the table up
// (the table grows downward, so the count is negative). The function name comes
// from the symbol; the pool and record views are inferred. The table is indexed
// by a byte offset, which is what puts the base register first in the load.
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

void SNDMEMI_free(void* memory) {
    int i = 0;
    unsigned int offset = (char*)memory - sndgs.pool->base;

    for (; i > sndgs.pool->count; i--) {
        int index = i * 8;

        if (*(unsigned int*)((char*)sndgs.pool->records + index) == offset) {
            sndgs.pool->count++;
            sndgs.pool->free += 8;
            for (; i > sndgs.pool->count; i--)
                sndgs.pool->records[i] = sndgs.pool->records[i - 1];
            return;
        }
    }
}
