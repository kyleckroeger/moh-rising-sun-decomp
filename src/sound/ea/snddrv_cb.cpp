/* The sound driver's audio frame callback, which feeds the software mixer's
   output to AX, and the mixer's creation and restore (a fragment of snddrv.c).
   sndgs and snddrv are named by their symbols; the views of them are inferred
   from offsets. */
#include "SoundGlobals.h"
struct _AXVPB;

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
    char fieldb70[0x1de0 - 48 * 64];
    OSThreadQueue threadqueue;
    char field1de8[0x9de8 - 0x1de8];
    int lastdma;
    int requests;
    char field9df0[0x9e30 - 0x9df0];
    SNDDRVDMA dma[64];
    char fieldae30[0xc048 - 0xae30];
    signed char mixerinited;
    unsigned char activevoices;
    char fieldc04a[6];
    void* mixalloc[6];
    void* mixbuffer[4];
    };

extern SNDDRVVIEW snddrv;

struct SNDMIXOPTSVIEW;
extern "C" {
int OSEnableInterrupts(void);
int OSRestoreInterrupts(int);
void OSWakeupThread(OSThreadQueue*);
float* MIX_getwetbuffer(void);
void SNDI_memcpy(void*, const void*, int);
void SNDSYS_linkmaincpumixer(void);
void SNDSYS_entercritical(void);
void SNDSYS_leavecritical(void);
extern void (*MIXinitfn)(SNDMIXOPTSVIEW*);
extern void (*MIXrestorefn)(void);
}
void* SNDMEMI_allocz(int);
void SNDMEMI_free(void*);
void SNDDRV_mixvoicefree(int);

static int everyother = 0;

void SNDDRV_audiocallback(void* buffers, void* context) {
    int enabled = OSEnableInterrupts();

    if (snddrv.mixerinited) {
        static float* ptr;
        int* src;
        float* dst;
        int i;

        dst = MIX_getwetbuffer();
        src = ((int**)buffers)[0];
        ptr = dst;
        for (i = 0; i < 160; i++)
            dst[i] = (float)src[i];
        if (sndgs.channels <= 3) {
            SNDI_memcpy(((void**)buffers)[1], snddrv.mixbuffer[0], 640);
            SNDI_memcpy(((void**)buffers)[0], snddrv.mixbuffer[1], 640);
        } else {
            SNDI_memcpy(((void**)buffers)[1], snddrv.mixbuffer[0], 640);
            SNDI_memcpy(((void**)buffers)[2], snddrv.mixbuffer[2], 640);
            SNDI_memcpy(((void**)buffers)[0], snddrv.mixbuffer[3], 640);
            SNDI_memcpy(((void**)buffers)[3], snddrv.mixbuffer[1], 640);
        }
    }
    OSWakeupThread(&snddrv.threadqueue);
    everyother++;
    OSRestoreInterrupts(enabled);
}

/* inferred: the mixer's creation parameters */
struct SNDMIXOPTSVIEW {
    int rate;
    unsigned char field04;
    unsigned char channels;
    unsigned char field06;
    void (*voicefree)(int);
};

void SNDDRV_initmixer() {
    SNDMIXOPTSVIEW params;
    int i;

    SNDSYS_linkmaincpumixer();
    params.field04 = sndgs.field2e;
    if (sndgs.channels != 1)
        params.channels = sndgs.channels;
    else
        params.channels = 2;
    params.rate = sndgs.samplerate;
    params.voicefree = SNDDRV_mixvoicefree;
    params.field06 = 0;
    MIXinitfn(&params);
    SNDSYS_entercritical();
    for (i = 0; i < params.channels; i++) {
        snddrv.mixalloc[i] = SNDMEMI_allocz(672);
        snddrv.mixbuffer[i] = (void*)(((unsigned long)snddrv.mixalloc[i] + 31) & ~31);
    }
    SNDSYS_leavecritical();
    snddrv.mixerinited = 1;
}

void SNDDRV_restoremixer() {
    int i;

    snddrv.mixerinited = 0;
    SNDSYS_entercritical();
    for (i = 0; i < sndgs.channels; i++)
        SNDMEMI_free(snddrv.mixalloc[i]);
    SNDSYS_leavecritical();
    MIXrestorefn();
}
