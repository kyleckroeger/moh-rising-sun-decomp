// Unpacker for MicroTalk (UTALK) compressed audio: decode blocks of 432
// samples into a float buffer and copy them out, padding with silence after
// the end. UNPACKINITPARAMS and UTALKSTATE are named by the mangled symbols;
// the state record (UNPACKMTSTATE, not an original name) has members inferred
// from offsets.
struct UTALKSTATE {
    char data[1664];
};

struct UNPACKINITPARAMS {
    char* data;
    int field4;
    int field8;
    unsigned int length;
    int field10[5];
    int (*getframe)(void*);
};

struct UNPACKMTSTATE {
    int (*unpack)(void*, int, void*, void*, int);
    char field4[24];
    unsigned int length;
    unsigned int position;
    int available;
    UTALKSTATE utalk;
    float samples[432];
};

extern "C" void SNDI_memcpy(void*, const void*, int);
void decodemut(UTALKSTATE*);
void initmut(char*, UTALKSTATE*);

int SFILTER_unpackgetframemtf(void* data) {
    return ((UNPACKMTSTATE*)data)->position;
}

int SFILTER_unpackmtf(void* data, int count, void* unused, void* output, int channels);

void SFILTER_unpackmtfinit(void* data, UNPACKINITPARAMS* params);
