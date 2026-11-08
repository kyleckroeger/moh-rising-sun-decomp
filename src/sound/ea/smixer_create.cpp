// A fragment of the software mixer (smixer.c): MIX_create sets the coda allocation
// hooks, copies the creation options, allocates the two 64-byte aligned work
// buffers and the per-output mix buffers and the channel records (96 bytes each,
// cleared) inside the sound-system critical section, initialises the unpackers,
// resets the mixer, and gives every output a float-to-integer stage (priority 0)
// on its filter chain and a cleared mix buffer. sndmix and the functions are named
// by their symbols; the views (option and mixer layouts, and the filter-stage
// priority at +24 as in sfilter.c) are inferred.
/* inferred: a float-to-integer stage (FT24_32STATE) as a filter record */
struct FT24_32STATE {
    unsigned char unknown00[24];
    unsigned short priority;
    unsigned char value1a;
    unsigned char unknown1b;
};

/* inferred: the software mixer's state, as far as it is used here */
/* inferred: the creation options */
struct SNDMIXOPTSVIEW {
    unsigned int value00;
    unsigned char channels;
    unsigned char outputs;
    unsigned char value06;
    unsigned char value07;
    unsigned int value08;
};
struct SNDMIXVIEW {
    SNDMIXOPTSVIEW opts;
    unsigned char unknown00c[3];
    unsigned char value0f[1];
    unsigned char unknown010[148];
    void* work[2];
    int* alignedWork[2];
    void* mix[6];
    int* alignedMix[6];
    unsigned char unknown0e4[28];
    void* filters[6];
    unsigned char unknown118[24];
    FT24_32STATE ft[6];
    void* channelRecords;
};

namespace Snd {
void CODASetNew(void* (*)(unsigned int));
void CODASetDelete(void (*)(void*));
}

void* SNDI_New(unsigned int);
void SNDI_Delete(void*);
void* SNDMEMI_allocz(int);
void SFILTER_ft24_32init(FT24_32STATE*);

extern "C" {
extern SNDMIXVIEW sndmix;
void SNDSYS_entercritical(void);
void SNDSYS_leavecritical(void);
void MIXI_initunpack16(void);
void MIXI_initunpackxa(void);
void MIXI_initunpackmt(void);
void MIX_reset(void);
void SFILTER_add(void*, void*);
void* memset(void*, int, unsigned long);
}

extern "C" void MIX_create(SNDMIXOPTSVIEW* opts) {
    int i;

    Snd::CODASetNew(SNDI_New);
    Snd::CODASetDelete(SNDI_Delete);
    for (i = 0; i < 1; i++)
        sndmix.value0f[i] = 0;
    sndmix.opts = *opts;
    SNDSYS_entercritical();
    for (i = 0; i < 2; i++) {
        sndmix.work[i] = SNDMEMI_allocz(5308);
        sndmix.alignedWork[i] = (int*)((char*)sndmix.work[i] + 8);
        while ((unsigned long)sndmix.alignedWork[i] & 63)
            sndmix.alignedWork[i]++;
    }
    for (i = 0; i < sndmix.opts.outputs; i++) {
        sndmix.mix[i] = SNDMEMI_allocz(704);
        sndmix.alignedMix[i] = (int*)sndmix.mix[i];
        while ((unsigned long)sndmix.alignedMix[i] & 63)
            sndmix.alignedMix[i]++;
    }
    if (sndmix.opts.channels) {
        sndmix.channelRecords = SNDMEMI_allocz(sndmix.opts.channels * 96);
        memset(sndmix.channelRecords, 0, sndmix.opts.channels * 96);
    }
    SNDSYS_leavecritical();
    MIXI_initunpack16();
    MIXI_initunpackxa();
    MIXI_initunpackmt();
    MIX_reset();
    for (i = 0; i < sndmix.opts.outputs; i++) {
        sndmix.filters[i] = 0;
        SFILTER_ft24_32init(&sndmix.ft[i]);
        {
            SNDMIXVIEW* shifted = (SNDMIXVIEW*)((char*)&sndmix + i * sizeof(FT24_32STATE));

            shifted->ft[0].priority = 0;
            shifted->ft[0].value1a = 0;
        }
        SFILTER_add(&sndmix.filters[i], &sndmix.ft[i]);
        memset(sndmix.alignedMix[i], 0, 640);
    }
}
