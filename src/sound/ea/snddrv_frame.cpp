/* The first functions of snddrv.c (a fragment): freeing the AX voices queued
   for release at the next frame, and the software mixer's voice-free hook.
   sndgs and snddrv are named by their symbols; the views of them are
   inferred from offsets. */
struct _AXVPB;

extern "C" {
void AXFreeVoice(_AXVPB*);
#include "SoundGlobals.h"
}
void SNDVOICEI_free(int);

/* inferred view of snddrv: 61 per-channel records, then the DMA request
   table, the mixer flags and the mix buffers */
struct SNDDRVVOICE {
    char field00[8];
    int field08;
    char field0C[19];
    unsigned char active;
    char field10[4];
    unsigned char field14;
    unsigned char field15;
    char field16[22];
    _AXVPB* axvoice;
};

struct SNDDRVDMA {
    int id;
    char field04[60];
};

struct OSThreadQueue {
    void* head;
    void* tail;
};

struct SNDDRVVIEW {
    SNDDRVVOICE voices[48];
    char fieldb70[0x1858 - 48 * 64];
    OSThreadQueue threadqueue;
    char field1860[0x9860 - 0x1860];
    int lastdma;
    char field9864[0x98a8 - 0x9864];
    SNDDRVDMA dma[64];
    char fielda8a8[0xc048 - 0xa8a8];
    signed char mixerinited;
    unsigned char activevoices;
    };

extern SNDDRVVIEW snddrv;

void SNDDRV_freenextframe() {
    int i;

    if (snddrv.activevoices) {
        for (i = 0; i < 48; i++) {
            if (snddrv.voices[i].active) {
                AXFreeVoice(snddrv.voices[i].axvoice);
                snddrv.voices[i].active = 0;
                snddrv.activevoices--;
                snddrv.voices[i].axvoice = 0;
                if (!snddrv.activevoices)
                    return;
            }
        }
    }
}

void SNDDRV_mixvoicefree(int voice) {
    SNDVOICEI_free(voice + sndgs.firstvoice);
}
