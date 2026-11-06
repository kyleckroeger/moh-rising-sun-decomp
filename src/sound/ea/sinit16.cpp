// MIXI_initunpack16: register the 16-bit PCM unpackers (init function and
// state size) with the mixer. sndmix's layout is not known; the slots are
// written through inferred offsets.
struct UNPACKINITPARAMS;
typedef void (*UNPACKINIT)(void*, UNPACKINITPARAMS*);

extern "C" char sndmix[];
void SFILTER_unpackfinit(void*, UNPACKINITPARAMS*);
void SFILTER_unpacklfinit(void*, UNPACKINITPARAMS*);
void SFILTER_unpackpfinit(void*, UNPACKINITPARAMS*);

extern "C" void MIXI_initunpack16(void) {
    char* mix = sndmix;

    *(UNPACKINIT*)(mix + 20) = SFILTER_unpackfinit;
    *(int*)(mix + 92) = 44;
    *(UNPACKINIT*)(mix + 24) = SFILTER_unpacklfinit;
    *(int*)(mix + 96) = 48;
    *(UNPACKINIT*)(mix + 28) = SFILTER_unpackpfinit;
    *(int*)(mix + 100) = 52;
}
