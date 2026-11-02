// MIX_sethighpass: set a mixer channel's high-pass cutoff, creating its FIR
// stage on first use, or remove the stage for a cutoff of 0. sndmix and sndgs
// layouts are not known; their fields are accessed through inferred offsets.
// HPFSTATE is named by the mangled symbols.
extern "C" char sndmix[];
#include "SoundGlobals.h"

struct HPFSTATE {
    int field0;
    int field4;
    char field8[16];
    short field18;
    char field1a;
};

void* SNDMEMI_allocz(int);
void SNDMEMI_free(void*);
void SFILTER_createHPFFIR8(HPFSTATE*);
void SFILTER_modifyHPFFIR8(HPFSTATE*, int*);
extern "C" void SFILTER_add(void*, void*);
extern "C" void SFILTER_remove(void*, void*);

extern "C" void MIX_sethighpass(int channel, int frequency) {
    char* entry = *(char**)(sndmix + 472) + channel * 96;

    if (frequency > 0) {
        int params[2];

        if (!*(HPFSTATE**)(entry + 92)) {
            *(HPFSTATE**)(entry + 92) = (HPFSTATE*)SNDMEMI_allocz(88);
            (*(HPFSTATE**)(entry + 92))->field4 = 0;
            (*(HPFSTATE**)(entry + 92))->field18 = 80;
            (*(HPFSTATE**)(entry + 92))->field1a = 0;
            SFILTER_createHPFFIR8(*(HPFSTATE**)(entry + 92));
            SFILTER_add(entry + 64, *(HPFSTATE**)(entry + 92));
        }
        params[0] = frequency << 8;
        params[1] = sndgs.samplerate << 8;
        SFILTER_modifyHPFFIR8(*(HPFSTATE**)(entry + 92), params);
    } else if (*(HPFSTATE**)(entry + 92)) {
        SFILTER_remove(entry + 64, *(HPFSTATE**)(entry + 92));
        SNDMEMI_free(*(HPFSTATE**)(entry + 92));
        *(HPFSTATE**)(entry + 92) = 0;
    }
}
