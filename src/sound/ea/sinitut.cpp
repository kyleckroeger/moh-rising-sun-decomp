// MIXI_initunpackmt: register the MT unpackers (init function and state size)
// with the mixer. sndmix's layout is not known; the slots are written through
// inferred offsets.
struct UNPACKINITPARAMS;

extern "C" char sndmix[];
void SFILTER_unpackmtfinit(void*, UNPACKINITPARAMS*);
void SFILTER_unpackmtlfinit(void*, UNPACKINITPARAMS*);
void SFILTER_unpackmtpfinit(void*, UNPACKINITPARAMS*);
typedef void (*UNPACKINIT)(void*, UNPACKINITPARAMS*);

extern "C" void MIXI_initunpackmt(void) {
    char* mix = sndmix;

    *(UNPACKINIT*)(mix + 44) = SFILTER_unpackmtfinit;
    *(int*)(mix + 116) = 44;
    *(UNPACKINIT*)(mix + 52) = SFILTER_unpackmtpfinit;
    *(UNPACKINIT*)(mix + 56) = SFILTER_unpackmtfinit;
    *(int*)(mix + 124) = 60;
    *(UNPACKINIT*)(mix + 48) = SFILTER_unpackmtlfinit;
    *(int*)(mix + 128) = 44;
    *(int*)(mix + 120) = 48;
    *(UNPACKINIT*)(mix + 64) = SFILTER_unpackmtpfinit;
    *(int*)(mix + 136) = 60;
    *(UNPACKINIT*)(mix + 60) = SFILTER_unpackmtlfinit;
    *(int*)(mix + 132) = 48;
}
