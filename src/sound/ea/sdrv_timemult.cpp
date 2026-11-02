/* A fragment of the sound driver (snddrv.c, 0x8016c1e8): the time
   multiplier and the low-pass cutoff (the value over half the output rate at
   +42; the constants are entries of the file's .sdata2 pool) of a mixer
   voice's channels. sndgs is named by its symbol; its
   view and the voice records are inferred. */
#include "SoundGlobals.h"


extern "C" void MIX_settimemult(int, int);

extern "C" void MIX_setlowpass(int, float);

int SNDPLATFORM_timemult(int voice, int value) {
    SNDVOICEVIEW* sound = &(sndgs.voices)[voice];
    int i;

    if (sound->flags & 4) {
        for (i = 0; i < sound->count; i++)
            MIX_settimemult(sound->channels[i] - sndgs.firstvoice, value);
    }
    return 0;
}

void SNDPLATFORM_lowpass(int voice, int value) {
    SNDVOICEVIEW* sound = &(sndgs.voices)[voice];
    int i;

    if (sound->flags & 4) {
        float cutoff = (float)value * (1.0f / ((float)sndgs.samplerate * 0.5f));
        for (i = 0; i < sound->count; i++)
            MIX_setlowpass(sound->channels[i] - sndgs.firstvoice, cutoff);
    }
}
