/* SNDPLATFORM_set3dpos (a fragment of the sound driver, snddrv.c): computes each channel's
   speaker levels for a voice. Mono output gives equal front levels; otherwise a mono voice
   uses its azimuth, and a multichannel voice its channel azimuths offset by a per-layout
   table, whose gains then scale the levels. The levels are handed to the DSP mixer or the
   voice's volume is reapplied. sndgs and snddrv are named by their symbols; the views and
   member names are inferred from offsets. */
#include "SoundGlobals.h"
struct _AXVPB;

/* inferred: the driver's per-channel record */
struct SNDDRVVOICE {
    float speakers[15];
    _AXVPB* axvoice;
};

struct SNDDRVVIEW {
    SNDDRVVOICE voices[48];
};

extern SNDDRVVIEW snddrv;
extern unsigned short SNDDRV_dolbypl2balances[256][4];

void SNDPLATFORM_setvol(int);
void SNDDRV_DSPMixerSetPan(_AXVPB*, float*, unsigned short, int);
void SNDI_aztospkrvol(int, float*);

void SNDPLATFORM_set3dpos(int index) {
    SNDVOICEVIEW* voice = &sndgs.voices[index];
    SNDDRVVOICE* own = &snddrv.voices[index];
    int outputs = sndgs.channels;
    int layout = 0;
    int i;

    if (outputs == 1) {
        for (i = 0; i < voice->count; i++) {
            snddrv.voices[voice->channels[i]].speakers[0] = 0.707106829f;
            snddrv.voices[voice->channels[i]].speakers[1] = 0.707106829f;
        }
        if (voice->count == 6) {
            snddrv.voices[voice->channels[5]].speakers[0] = 0.0f;
            snddrv.voices[voice->channels[5]].speakers[1] = 0.0f;
        }
        if (voice->flags & 0x200) {
            for (i = 0; i < voice->count; i++)
                SNDDRV_DSPMixerSetPan(snddrv.voices[voice->channels[i]].axvoice, snddrv.voices[voice->channels[i]].speakers, voice->azimuth, i > 4);
        } else {
            SNDPLATFORM_setvol(index);
        }
        return;
    }
    if ((unsigned int)(voice->count - 5) <= 1) {
        if (outputs == 4)
            layout = 1;
        else if (outputs == 2)
            layout = 3;
        else if (outputs == 1)
            layout = 4;
    } else if (voice->count == 4) {
        if (outputs == 2)
            layout = 5;
        else if (outputs == 1)
            layout = 6;
    }
    if (voice->count == 1) {
        if (voice->flags & 0x200) {
            if (sndgs.channels == 2)
                SNDI_aztospkrvol((unsigned short)voice->azimuth, own->speakers);
        } else if (sndgs.channels == 2) {
            SNDI_aztospkrvol((unsigned short)voice->azimuth, own->speakers);
        } else {
            snddrv.voices[voice->channels[0]].speakers[0] = (float)SNDDRV_dolbypl2balances[(unsigned short)voice->azimuth >> 8][1] * (1.0f / 32767.0f);
            snddrv.voices[voice->channels[0]].speakers[1] = (float)SNDDRV_dolbypl2balances[(unsigned short)voice->azimuth >> 8][3] * (1.0f / 32767.0f);
            snddrv.voices[voice->channels[0]].speakers[2] = (float)SNDDRV_dolbypl2balances[(unsigned short)voice->azimuth >> 8][2] * (1.0f / 32767.0f);
            snddrv.voices[voice->channels[0]].speakers[3] = (float)SNDDRV_dolbypl2balances[(unsigned short)voice->azimuth >> 8][0] * (1.0f / 32767.0f);
        }
    } else {
        for (i = 0; i < voice->count; i++) {
            if (voice->flags & 0x200) {
                if (sndgs.channels == 2)
                    SNDI_aztospkrvol(voice->azimuths[i] + sndgs.speakerpan[layout][i].pan, snddrv.voices[voice->channels[i]].speakers);
                else
                    voice->azimuths[i] += sndgs.speakerpan[layout][i].pan;
            } else if (sndgs.channels == 2) {
                SNDI_aztospkrvol(voice->azimuths[i] + sndgs.speakerpan[layout][i].pan, snddrv.voices[voice->channels[i]].speakers);
            } else {
                int balance = (voice->azimuths[i] + sndgs.speakerpan[layout][i].pan) / 256;

                snddrv.voices[voice->channels[i]].speakers[0] = (float)SNDDRV_dolbypl2balances[balance][1] * (1.0f / 32767.0f);
                snddrv.voices[voice->channels[i]].speakers[1] = (float)SNDDRV_dolbypl2balances[balance][3] * (1.0f / 32767.0f);
                snddrv.voices[voice->channels[i]].speakers[2] = (float)SNDDRV_dolbypl2balances[balance][2] * (1.0f / 32767.0f);
                snddrv.voices[voice->channels[i]].speakers[3] = (float)SNDDRV_dolbypl2balances[balance][0] * (1.0f / 32767.0f);
            }
        }
    }
    for (i = 0; i < voice->count; i++) {
        int j;

        for (j = 0; j < sndgs.channels; j++)
            snddrv.voices[voice->channels[i]].speakers[j] = snddrv.voices[voice->channels[i]].speakers[j] * (1.0f / 32767.0f) * (float)sndgs.speakerpan[layout][i].gain;
    }
    if (voice->flags & 0x200) {
        for (i = 0; i < voice->count; i++) {
            int rear = i > 4;

            if (sndgs.channels == 4 && voice->count > 1 && !rear)
                SNDDRV_DSPMixerSetPan(snddrv.voices[voice->channels[i]].axvoice, snddrv.voices[voice->channels[i]].speakers, voice->azimuths[i], rear);
            else
                SNDDRV_DSPMixerSetPan(snddrv.voices[voice->channels[i]].axvoice, snddrv.voices[voice->channels[i]].speakers, voice->azimuth, rear);
        }
    } else {
        SNDPLATFORM_setvol(index);
    }
}
