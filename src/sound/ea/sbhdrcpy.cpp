// SNDbankheadercopy: copy a bank's header to new memory and use that copy.
// sndgs's layout is not known; the slot table is read through inferred
// offsets.
extern "C" {
extern char sndgs[];
int SNDbankheadersize(int);
void* memmove(void*, const void*, unsigned long);
}

struct SNDBANKSLOT {
    void* header;
    int field4;
    int field8;
};

extern "C" int SNDbankheadercopy(void* buffer, int bank) {
    int result = 0;
    int size = SNDbankheadersize(bank);

    if (size < 0) {
        result = size;
    } else {
        char* gs = sndgs;

        memmove(buffer, (*(SNDBANKSLOT**)(gs + 500))[bank].header, size);
        (*(SNDBANKSLOT**)(gs + 500))[bank].header = buffer;
    }
    return result;
}
