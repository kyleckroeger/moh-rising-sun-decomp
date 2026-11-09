/* The sound driver's AX voice management and mixer callbacks (a fragment of
   snddrv.c): acquiring and releasing the AX voices behind a sound voice's
   channels, the DMA completion callback, the audio frame callback that feeds
   the software mixer's output to AX, and mixer creation. sndgs and snddrv are
   named by their symbols; the views of them are inferred from offsets.
   SNDDRV_callbackdropvoice, between this fragment and the one before it, is
   not reconstructed yet. */
#include "SoundGlobals.h"
struct _AXVPB;
struct OSThreadQueue;

extern "C" {
void AXFreeVoice(_AXVPB*);
_AXVPB* AXAcquireVoice(unsigned long, void (*)(void*), unsigned long);
int OSEnableInterrupts(void);
int OSRestoreInterrupts(int);
void OSWakeupThread(OSThreadQueue*);
float* MIX_getwetbuffer(void);
void SNDI_memcpy(void*, const void*, int);
void SNDSYS_linkmaincpumixer(void);
void SNDSYS_entercritical(void);
void SNDSYS_leavecritical(void);
}
void SNDVOICEI_free(int);
void* SNDMEMI_allocz(int);
void SNDDRV_mixvoicefree(int);
void SNDDRV_callbackdropvoice(void*);
void SNDDRV_DSPMixerInitChannel(_AXVPB*, unsigned long, int, unsigned short, unsigned short);
void SNDDRV_DSPMixerReleaseChannel(_AXVPB*);

/* inferred view of snddrv: 61 per-channel records, then the DMA request
   table, the mixer flags and the mix buffers */
struct SNDDRVVOICE {
    char field00[24];
    int field08;
    char field0C[3];
    unsigned char active;
    char field10[4];
    unsigned char field14;
    volatile unsigned char field15;
    char field16[22];
    _AXVPB* axvoice;
};

struct ARQRequest {
    unsigned char unknown00[32];
};
struct SNDDRVDMA {
    volatile int id;
    ARQRequest request;
    int priority;
    unsigned int source;
    unsigned int dest;
    unsigned int length;
    unsigned int time;
    unsigned char unknown38[8];
};

struct OSThreadQueue {
    void* head;
    void* tail;
};

struct SNDDRVVIEW {
    SNDDRVVOICE voices[48];
    char fieldb70[0x1858 - 48 * 64];
    OSThreadQueue threadqueue;
    char field1860[0x9de8 - 0x1860];
    int lastdma;
    int requests;
    char field9df0[0x9e30 - 0x9df0];
    SNDDRVDMA dma[64];
    char fieldae30[0xc048 - 0xae30];
    signed char mixerinited;
    unsigned char activevoices;
    char fieldc04a[6];
    void* mixalloc[4];
    void* mixbuffer[3];
    };

extern SNDDRVVIEW snddrv;

extern "C" {
void ARQPostRequest(ARQRequest*, unsigned int, unsigned int, unsigned int, unsigned int, unsigned int, unsigned int, void (*)(unsigned long));
void ARQRemoveRequest(ARQRequest*);
int OSDisableInterrupts(void);
int OSRestoreInterrupts(int);
}
void SNDDRV_dmcallback(unsigned long);

/* sndgs's voice table entries (128 bytes) */

int SNDDRV_allocateaxvoice(int voice);

void SNDDRV_freeaxvoice(int channel, int mode);

struct SNDDMAREQVIEW {
    char field0[4];
    int id;
};

int SNDDRV_dmaslotenter(int id);

void SNDDRV_dmcallback(unsigned long request);

int SNDDRV_dmapost(unsigned long priority, void* source, void* dest, unsigned long length, int slot);

void SNDDRV_restoredma(int id) {
    int level = OSDisableInterrupts();
    int i;

    for (i = 0; i < 64; i++) {
        if (snddrv.dma[i].id == id) {
            ARQRemoveRequest(&snddrv.dma[i].request);
            SNDDRV_dmapost(snddrv.dma[i].priority, (void*)snddrv.dma[i].source, (void*)snddrv.dma[i].dest, snddrv.dma[i].length, i);
        }
    }
    OSRestoreInterrupts(level);
}


void SNDDRV_audiocallback(void* buffers, void* context);

/* inferred: the mixer's creation parameters */
struct MIXINITVIEW {
    int rate;
    unsigned char field04;
    unsigned char channels;
    unsigned char field06;
    void (*voicefree)(int);
};

void SNDDRV_initmixer();
