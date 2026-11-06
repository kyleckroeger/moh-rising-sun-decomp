// MIX_setlowpass: set a mixer channel's low-pass cutoff (a fraction of the
// sample rate), creating its RC filter stage on first use, or remove the stage
// for a cutoff of 1.0 or more. sndmix and sndgs layouts are not known; their
// fields are accessed through inferred offsets. LPFRCSTATE is named by the
// mangled symbols; this view of it is inferred.
extern "C" char sndmix[];
#include "SoundGlobals.h"

struct LPFRCSTATE {
    int field0;
    int field4;
    char field8[16];
    short field18;
    char field1a;
};

void* SNDMEMI_allocz(int);
void SNDMEMI_free(void*);
void SFILTER_createLPFRC(LPFRCSTATE*);
void SFILTER_modifyLPFRC(LPFRCSTATE*, int*);
extern "C" void SFILTER_add(void*, void*);
extern "C" void SFILTER_remove(void*, void*);

extern "C" void MIX_setlowpass(int channel, float cutoff) {
    char* entry = *(char**)(sndmix + 472) + channel * 96;

    if (!(cutoff >= 1.0f)) {
        int params[3];

        if (!*(LPFRCSTATE**)(entry + 88)) {
            *(LPFRCSTATE**)(entry + 88) = (LPFRCSTATE*)SNDMEMI_allocz(40);
            (*(LPFRCSTATE**)(entry + 88))->field4 = 0;
            (*(LPFRCSTATE**)(entry + 88))->field18 = 40;
            (*(LPFRCSTATE**)(entry + 88))->field1a = 0;
            SFILTER_createLPFRC(*(LPFRCSTATE**)(entry + 88));
            SFILTER_add(entry + 64, *(LPFRCSTATE**)(entry + 88));
        }
        params[1] = sndgs.samplerate << 8;
        float frequency = cutoff * sndgs.samplerate;
        int cut = (int)(!(frequency < 0.0f) ? frequency + 0.5f : frequency - 0.5f);
        params[0] = cut << 7;
        params[2] = 256;
        SFILTER_modifyLPFRC(*(LPFRCSTATE**)(entry + 88), params);
    } else if (*(LPFRCSTATE**)(entry + 88)) {
        SFILTER_remove(entry + 64, *(LPFRCSTATE**)(entry + 88));
        SNDMEMI_free(*(LPFRCSTATE**)(entry + 88));
        *(LPFRCSTATE**)(entry + 88) = 0;
    }
}
