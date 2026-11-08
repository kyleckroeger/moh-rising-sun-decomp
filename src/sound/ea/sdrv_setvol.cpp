/* SNDPLATFORM_setvol (a fragment of the sound driver, snddrv.c): applies a voice's
   volume to each of its channels: through the DSP mixer for hardware voices, else as
   dry gains of the software mixer per output. sndgs and snddrv are named by their
   symbols; their views are inferred. */
#include "SoundGlobals.h"
struct _AXVPB;

void SNDDRV_DSPMixerSetVol(_AXVPB*, int);
extern "C" void SNDMIX_setdrygain(int, int, float);

/* inferred: the driver's per-channel record */
struct SNDDRVVOICE {
    float speakers[15];
    _AXVPB* axvoice;
};

struct SNDDRVVIEW {
    SNDDRVVOICE voices[48];
};

extern SNDDRVVIEW snddrv;

void SNDPLATFORM_setvol(int index) {
    SNDVOICEVIEW* voice = &sndgs.voices[index];
    SNDDRVVIEW* driver = &snddrv;
    int i;
    float gain;

    if (voice->flags & 0x200) {
        gain = voice->outvolume * (float)voice->field5f;
        for (i = 0; i < voice->count; i++)
            SNDDRV_DSPMixerSetVol(driver->voices[voice->channels[i]].axvoice, (int)(gain * 258.0f));
    } else {
        gain = voice->outvolume * (float)voice->field5f * (1.0f / 127.0f);
        int channels = sndgs.channels;
        int outputs;
        int j;

        if (channels == 1)
            outputs = 2;
        else
            outputs = channels;
        for (i = 0; i < voice->count; i++) {
            SNDDRVVOICE* channel = &snddrv.voices[voice->channels[i]];

            for (j = 0; j < outputs; j++)
                SNDMIX_setdrygain(voice->channels[i] - sndgs.firstvoice, j, gain * channel->speakers[j]);
        }
    }
}
