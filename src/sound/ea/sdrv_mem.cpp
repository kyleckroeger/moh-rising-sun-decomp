/* A fragment of the sound driver (snddrv.c, 0x8016c5f8): filter
   installation (unsupported), memory allocation and freeing through the
   installed allocator or ARAM (sizes rounded up to 32 bytes), and
   SNDPLATFORM_download, which flushes the block and posts its transfer to
   ARAM through ARQ in a new DMA slot, returning the request number, and
   SNDPLATFORM_downloadcomplete, which reports whether a request is finished
   (1 when no DMA slot holds it any more) and re-posts every slot of a
   request that has been pending for more than 200 ticks. snddrv and sndgs
   are named by their symbols; their views, the DMA slot layout and the slot,
   posting and re-posting inline helpers are inferred. A slot's id is
   volatile: the DMA callback frees the slot. */
#include "SoundGlobals.h"
struct OSMutex {
    unsigned char unknown00[24];
};

/* inferred: an ARQ request block */
struct ARQRequest {
    unsigned char unknown00[32];
};

struct SNDFILTERDEF;

extern "C" {

void OSInitMutex(OSMutex*);
void OSLockMutex(OSMutex*);
void OSUnlockMutex(OSMutex*);
int OSDisableInterrupts(void);
int OSRestoreInterrupts(...);
void DCFlushRange(void*, unsigned int);
void ARQPostRequest(ARQRequest*, unsigned int, unsigned int, unsigned int, unsigned int, unsigned int,
                    unsigned int, void (*)(unsigned long));
void ARQRemoveRequest(ARQRequest*);
}
void* SNDARAM_alloc(int);
void SNDARAM_free(unsigned int);
void SNDDRV_dmcallback(unsigned long);

/* inferred: a DMA slot (id -1 when free) */
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

struct SNDDRVVIEW {
    unsigned char unknown0000[0x9e30];
    SNDDRVDMA dma[64];
    
};

extern SNDDRVVIEW snddrv;

/* inferred: claims a free DMA slot for a request */
static inline int allocdma(int id) {
    int level = OSDisableInterrupts();
    int i;

    for (i = 0; i < 64; i++) {
        if (snddrv.dma[i].id == -1)
            break;
    }
    snddrv.dma[i].id = id;
    OSRestoreInterrupts(level);
    return i;
}

/* inferred: posts a transfer, in a new slot when slot is -1 */

extern "C" void MIX_filteradd(int, int, SNDFILTERDEF*);

int SNDPLATFORM_filteradd(int voice, SNDFILTERDEF* filter) {
    SNDVOICEVIEW* sound = &sndgs.voices[voice];
    int i;

    if (sound->flags & 4) {
        for (i = 0; i < sound->count; i++)
            MIX_filteradd(sound->channels[i] - sndgs.firstvoice, i, filter);
        return 0;
    }
    return -7;
}

void* SNDPLATFORM_memalloc(int, int size) {
    size = (size + 31) & ~31;
    if (sndgs.memallochook)
        return (sndgs.memallochook)(size);
    return SNDARAM_alloc(size);
}

int SNDDRV_dmapost(unsigned long, void*, void*, unsigned long, int);
void SNDDRV_restoredma(int);

int SNDPLATFORM_memfree(int, unsigned int memory) {
    if (!sndgs.memfreehook) {
        SNDARAM_free(memory);
    } else {
        sndgs.memfreehook(memory);
    }
    return 0;
}

int SNDPLATFORM_download(int, void* source, void* dest, int length) {
    DCFlushRange(source, length);
    return SNDDRV_dmapost(0, source, dest, length, -1);
}

int SNDPLATFORM_downloadcomplete(int id) {
    int i;

    OSDisableInterrupts();
    for (i = 0; i < 64; i++) {
        if (snddrv.dma[i].id != -1 && snddrv.dma[i].id == id) {
            if (sndgs.tick - snddrv.dma[i].time > 200) {
                OSRestoreInterrupts();
                SNDDRV_restoredma(id);
            } else {
                OSRestoreInterrupts();
            }
            return 0;
        }
    }
    OSRestoreInterrupts();
    return 1;
}
