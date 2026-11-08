// The start of EA's sound memory manager (smemman.c). The pool is laid out in
// caller memory with a table of (offset, size) allocation records at its end,
// indexed downward. SNDMEMI_constrain limits a gap to what the pool's free
// size allows, SNDMEMI_init lays the pool out (data 16-byte aligned after a
// 24-byte header) and SNDMEMI_restore asks for the high-water mark. The
// function names come from the symbols; the pool and record views are inferred.
// SNDMEMI_allocz, which follows, is in smemman_allocz.cpp.
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

extern "C" int SNDMEM_gethighwater();

void SNDMEMI_constrain(unsigned int* start, int* gap) {
    if (*start + *gap > sndgs.pool->free)
        *gap = sndgs.pool->free - *start;
}

void SNDMEMI_init(void* memory, int size) {
    sndgs.pool = (SNDMEMPOOLVIEW*)memory;
    ((SNDMEMPOOLVIEW*)memory)->size = size;
    sndgs.pool->records = (SNDMEMRECORDVIEW*)((char*)memory + size - 8);
    sndgs.pool->base = (char*)memory + 24;
    sndgs.pool->base += 15;
    sndgs.pool->base = (char*)((int)sndgs.pool->base & ~15);
    size -= 15;
    sndgs.pool->free = size - 32;
    sndgs.pool->lowest = size;
}

int SNDMEMI_restore() {
    return SNDMEM_gethighwater();
}
