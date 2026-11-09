// MEM_free, the whole file: frees a block, merging it with free neighbours on
// both sides, and when the block before it is in use moves the freed block's
// header down to just after that block's (aligned) data, then returns it to
// the class's free list. Two class hooks, before and after, receive a record
// describing the block. MEMBLOCK and MEMCLASS are named by the mangled symbols;
// their members and the record are inferred from offsets, and the names are not
// original.
struct MEMBLOCK {
    unsigned short magic;
    unsigned short flags;
    int size;
    MEMBLOCK* next;
    MEMBLOCK* prev;
};

/* inferred: what the free hooks are given */
struct MEMHOOKINFO {
    const char* name;
    int size;
    int field08;
    int field0c;
    int flags;
    void* address;
};

struct MEMCLASS {
    char name[8];
    MEMBLOCK* low;
    MEMBLOCK* high;
    MEMBLOCK free;
    MEMBLOCK* field20;
    MEMBLOCK* field24;
    int field28;
    int alignment;
    int field30;
    int flags;
    int locked;
    int mutex[8];
    void (*prealloc)(MEMHOOKINFO*);
    void (*postalloc)(MEMHOOKINFO*);
    void (*prefree)(MEMHOOKINFO*);
    void (*postfree)(MEMHOOKINFO*);
};

static const int MB_FREE = 0x4000;
static const int MB_FIXED = 0x8000;

extern MEMCLASS* memclass[64];

extern "C" {
void MUTEX_lock(void*);
void MUTEX_unlock(void*);
const char* MEM_name(void*);
}
int MEM_tailsize(const char*, int);
void FREE_add(MEMCLASS*, MEMBLOCK*);
void FREE_remove(MEMBLOCK*);

static inline MEMHOOKINFO* MakeInfo(MEMHOOKINFO* p, const char* name, int size, int flags, void* address) {
    p->name = name;
    p->size = size;
    p->field08 = 0;
    p->field0c = 0;
    p->flags = flags;
    p->address = address;
    return p;
}
extern "C" int MEM_free(void* address) {
    if (address) {
        MEMBLOCK* block = (MEMBLOCK*)((char*)address - 16);
        MEMCLASS* mc = memclass[block->flags & 0x3F];
        MEMBLOCK* next;
        MEMBLOCK* prev;
        MEMHOOKINFO info;
        MEMHOOKINFO* pinfo;

        if (!mc)
            return 0;
        if (mc->locked)
            MUTEX_lock(mc->mutex);
        pinfo = 0;
        if (mc->prefree) {
            pinfo = MakeInfo(&info, MEM_name(address), block->size, block->flags, address);
            mc->prefree(pinfo);
        }
        prev = block->prev;
        next = block->next;
        if (prev->flags & MB_FREE) {
            FREE_remove(prev);
            block = prev;
            prev = block->prev;
            block->next = next;
            prev->next = block;
            next->prev = block;
        }
        if (next->flags & MB_FREE) {
            FREE_remove(next);
            next = next->next;
            block->prev = prev;
            block->next = next;
            next->prev = block;
        }
        if (!(prev->flags & (MB_FREE | MB_FIXED))) {
            int size = prev->size + MEM_tailsize(0, prev->flags);
            MEMBLOCK* moved;
            int smallest = 8;
            if (size < smallest)
                size = smallest;
            moved = (MEMBLOCK*)((char*)prev + ((size + mc->field28 + 15) & -mc->field28));
            if (block != moved) {
                moved->flags = block->flags;
                block = moved;
                moved->next = next;
                moved->prev = prev;
                prev->next = moved;
                next->prev = moved;
            }
        }
        FREE_add(mc, block);
        if (mc->postfree) {
            if (!pinfo) {
                pinfo = MakeInfo(&info, 0, 0, 0, address);
            }
            mc->postfree(pinfo);
        }
        if (mc->locked)
            MUTEX_unlock(mc->mutex);
    }
    return 1;
}
