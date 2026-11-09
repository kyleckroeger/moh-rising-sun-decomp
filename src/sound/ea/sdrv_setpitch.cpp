/* SNDPLATFORM_setpitch (a fragment of the sound driver, snddrv.c): sets a voice's
   pitch from its rate over the output rate and its scaled pitch: through the
   AX voice's source ratio for each channel of a hardware voice (-8 when a channel
   has no AX voice) or through the mixer's hook otherwise. sndgs, snddrv and
   MIXsetpitchfn are named by their symbols; the views are inferred. */
#include "SoundGlobals.h"
struct _AXVPB;

extern "C" void AXSetVoiceSrcRatio(_AXVPB*, float);

/* inferred: the driver's per-channel record */
struct SNDDRVVOICE {
    char unknown0[60];
    _AXVPB* axvoice;
};

struct SNDDRVVIEW {
    SNDDRVVOICE voices[48];
};

extern SNDDRVVIEW snddrv;
extern void (*MIXsetpitchfn)(int, int);

int SNDPLATFORM_setpitch(int index) {
    SNDVOICEVIEW* voice = &sndgs.voices[index];
    int i;
    unsigned int pitch;

    if (voice->flags & 0x200) {
        pitch = (voice->rate << 12) / sndgs.samplerate;
        pitch = pitch * voice->scaled >> 12;
        float ratio = (float)pitch * (1.0f / 4096.0f);
        for (i = 0; i < voice->count; i++) {
            _AXVPB* ax = snddrv.voices[voice->channels[i]].axvoice;
            if (!ax)
                return -8;
            AXSetVoiceSrcRatio(ax, ratio);
        }
    } else {
        pitch = (voice->rate << 15) / sndgs.samplerate;
        pitch = pitch * voice->scaled >> 11;
        for (i = 0; i < voice->count; i++)
            MIXsetpitchfn(voice->channels[i] - sndgs.firstvoice, pitch);
    }
    return 0;
}
