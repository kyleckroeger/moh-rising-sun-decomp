/* SNDDRV_allocateaxvoice (a fragment of the sound driver, snddrv.c): acquires
   an AX voice for each channel of a sound voice and sets up its mixer channel;
   when an acquire fails, the channels acquired so far are released. sndgs and
   snddrv are named by their symbols; their views and the voice records are
   inferred. */
#include "SoundGlobals.h"
struct _AXVPB;

extern "C" {
_AXVPB* AXAcquireVoice(unsigned long, void (*)(void*), unsigned long);
void AXFreeVoice(_AXVPB*);
}
void SNDVOICEI_free(int);
void SNDI_aztospkrvol(int, float*);
void SNDDRV_callbackdropvoice(void*);
void SNDDRV_DSPMixerInitChannel(_AXVPB*, unsigned long, int, unsigned short, unsigned short, float*, unsigned short, int);

/* inferred: the driver's per-channel record */
struct SNDDRVVOICE {
    float speakers[9];
    unsigned char unknown24;
    unsigned char dropped;
    char unknown26[22];
    _AXVPB* axvoice;
};

struct SNDDRVVIEW {
    SNDDRVVOICE voices[48];
};

extern SNDDRVVIEW snddrv;

int SNDDRV_allocateaxvoice(int voice) {
    SNDVOICEVIEW* entry = &(sndgs.voices)[voice];
    int volume = (int)(entry->outvolume * (float)entry->field5f);
    int fxlevel = (int)(entry->outvolume * (float)entry->fxlevel[0]);
    int i;

    for (i = 0; i < entry->count; i++) {
        int priority;

        if (entry->priority == 101)
            priority = 31;
        else
            priority = (unsigned char)(entry->priority / 3);
        if (priority <= 0)
            priority = 1;
        else if (priority > 31)
            priority = 30;
        if ((snddrv.voices[entry->channels[i]].axvoice = AXAcquireVoice(priority, SNDDRV_callbackdropvoice, entry->channels[i])) != 0) {
            int rear;

            snddrv.voices[entry->channels[i]].dropped = 0;
            if (i > 4) {
                rear = 1;
                fxlevel = 0;
            } else {
                rear = 0;
            }
            SNDI_aztospkrvol((unsigned short)entry->azimuth, snddrv.voices[entry->channels[i]].speakers);
            SNDDRV_DSPMixerInitChannel(snddrv.voices[entry->channels[i]].axvoice, 0, (int)((float)volume * 258.0f), fxlevel, 0, snddrv.voices[entry->channels[i]].speakers, (unsigned short)entry->azimuth, rear);
        } else {
            snddrv.voices[entry->channels[i]].dropped = 1;
            if (i > 0) {
                int j;

                for (j = i; j >= 0; j--) {
                    if (snddrv.voices[entry->channels[j]].axvoice) {
                        AXFreeVoice(snddrv.voices[entry->channels[j]].axvoice);
                        snddrv.voices[entry->channels[j]].dropped = 1;
                        snddrv.voices[entry->channels[j]].axvoice = 0;
                    }
                    if ((sndgs.voices)[entry->channels[j]].state)
                        SNDVOICEI_free(entry->channels[j]);
                }
            }
            return 0;
        }
    }
    return 1;
}
