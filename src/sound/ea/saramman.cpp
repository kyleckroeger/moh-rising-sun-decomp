// Fragment of the ARAM sample-memory manager: set-up, pool bounds (in 32-byte
// units) and teardown, then the allocator: SNDARAM_alloc takes the first
// free range (before, between or after the sorted allocated ranges, clipped
// to the pool) that fits, inserting its record, and returns its byte
// address (0 when full). SNDARAM_free after it is not part of this unit.
// sndaram's members, the range record and the clipping helper are inferred
// and are not original.
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

extern "C" {
extern SNDARAMVIEW sndaram;
void SNDSYS_entercritical(void);
void SNDSYS_leavecritical(void);
}
void* SNDMEMI_allocz(int);
void SNDMEMI_free(void*);

void SNDARAM_init(int blocks) {
    SNDSYS_entercritical();
    sndaram.table = (SNDARAMBLOCKVIEW*)SNDMEMI_allocz(blocks * 8);
    SNDSYS_leavecritical();
    sndaram.count = 0;
    sndaram.max = blocks;
}

void SNDARAM_setpool(unsigned int base, int size) {
    sndaram.end = (base + size) & ~4;
    if (base < 32)
        base = 32;
    sndaram.start = base;
    if (base & 31)
        sndaram.start = (base + 31) & ~31;
    sndaram.start >>= 5;
    sndaram.end >>= 5;
}

void SNDARAM_restore(void) {
    if (sndaram.table) {
        SNDSYS_entercritical();
        SNDMEMI_free(sndaram.table);
        SNDSYS_leavecritical();
        sndaram.table = 0;
    }
}

/* inferred inline helper: clip a free range to the pool */
static inline void clipfree(unsigned int& start, int& length) {
    if (start < sndaram.start) {
        unsigned int skip = sndaram.start - start;
        start += skip;
        length -= skip;
    }
    if (start + length > sndaram.end)
        length = sndaram.end - start;
}

int SNDARAM_alloc(int size);
