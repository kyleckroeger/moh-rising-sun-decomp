// Platform effect handling (sdfx.c): SNDDRV_setfx initialises the mixer reverb
// when bus 0's type is 1 and restores it otherwise, SNDPLATFORM_fxinit installs
// that as the driver's effect hook, and SNDPLATFORM_setfxlevel applies a voice's
// send level either as a DSP aux bus level (hardware voices, flag 0x200) or as
// the software mixer's wet gain (flag 4). The driver's and voices' views are
// inferred; _AXVPB is named by the mangled symbols.
#include "SoundGlobals.h"

struct _AXVPB;

/* inferred: a driver voice record (64 bytes) */
struct SNDDRVVOICEVIEW {
    char field00[60];
    _AXVPB* axvoice;
};

/* inferred: the sound driver's state, as far as it is used here */
struct SNDDRVVIEW {
    SNDDRVVOICEVIEW voices[48];
    char field0C00[2064];
    void (*setfx)(int);
    char field1414[44084];
    signed char mixerinited;
};

extern "C" {
extern SNDDRVVIEW snddrv;
void SNDSYS_entercritical(void);
void SNDSYS_leavecritical(void);
void MIX_restorereverb(void);
void MIX_initreverb(int rate, void* params);
void SNDMIX_setwetgain(int channel, int output, float gain);
}
void SNDDRV_initmixer();
void SNDDRV_DSPMixerSetAuxBus(_AXVPB* voice, unsigned short level, int bus);

void SNDDRV_setfx(int) {
    SNDSYS_entercritical();
    if (sndgs.fxreverb[0].type == 1)
        MIX_initreverb(sndgs.samplerate, (void*)sndgs.fxreverb[0].param1);
    else
        MIX_restorereverb();
    SNDSYS_leavecritical();
}

int SNDPLATFORM_fxinit(int, int bus) {
    snddrv.setfx = SNDDRV_setfx;
    if (!snddrv.mixerinited)
        SNDDRV_initmixer();
    SNDDRV_setfx(bus);
    return 0;
}

int SNDPLATFORM_setfxlevel(int voice, int bus) {
    SNDVOICEVIEW* entry = &sndgs.voices[voice];
    int i;

    if (entry->flags & 0x200) {
        float level = entry->outvolume * (float)entry->fxlevel[bus];

        for (i = 0; i < entry->count; i++) {
            if (i == 5)
                level = 0.0f;
            SNDDRV_DSPMixerSetAuxBus(snddrv.voices[entry->channels[i]].axvoice, (int)level, 1);
        }
    } else if (entry->flags & 4) {
        float gain = entry->outvolume * (float)entry->fxlevel[bus] * (1.0f / 32767.0f);

        for (i = 0; i < entry->count; i++) {
            if (i == 5)
                gain = 0.0f;
            SNDMIX_setwetgain(entry->channels[i] - sndgs.firstvoice, bus, gain);
        }
    } else {
        return -15;
    }
    return 0;
}
