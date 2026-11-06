// MIXI_initunpackxa: register the XA ADPCM unpackers (init function and state
// size) with the mixer. sndmix's layout is not known; the slots are written
// through inferred offsets.
struct UNPACKINITPARAMS;
typedef void (*UNPACKINIT)(void*, UNPACKINITPARAMS*);

extern "C" char sndmix[];
void SFILTER_unpackxafinit(void*, UNPACKINITPARAMS*);
void SFILTER_unpackxalfinit(void*, UNPACKINITPARAMS*);
void SFILTER_unpackxapfinit(void*, UNPACKINITPARAMS*);

extern "C" void MIXI_initunpackxa(void) {
    char* mix = sndmix;

    *(UNPACKINIT*)(mix + 32) = SFILTER_unpackxafinit;
    *(int*)(mix + 104) = 44;
    *(int*)(mix + 108) = 84;
    *(UNPACKINIT*)(mix + 36) = SFILTER_unpackxalfinit;
    *(UNPACKINIT*)(mix + 40) = SFILTER_unpackxapfinit;
    *(int*)(mix + 112) = 60;
}
