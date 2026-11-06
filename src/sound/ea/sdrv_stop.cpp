/* SNDPLATFORM_stop (a fragment of the sound driver, snddrv.c): an AX voice's
   packet player is detached and its AX voices stopped and released; a mixer
   voice's channels are stopped and freed. sndgs and snddrv are named by their
   symbols; their views and the voice records are inferred. */
struct _AXVPB;

extern "C" {
#include "SoundGlobals.h"
extern void (*MIXstopfn)(int);
void AXSetVoiceState(_AXVPB*, unsigned short);
}
void SNDDRV_freeaxvoice(int, int);
void SNDVOICEI_free(int);

/* inferred: the driver's per-channel record (48 bytes) */
struct SNDDRVVOICE {
    unsigned char unknown00[24];
    int start;
    signed char packet;
    unsigned char unknown1d[7];
    unsigned char allocated;
    unsigned char unknown15[23];
    _AXVPB* axvoice;
};

/* inferred: the packet player's platform record */
struct SNDDRVPACKETVIEW {
    unsigned char unknown0000[9624];
    signed char state;
};

struct SNDDRVVIEW {
    SNDDRVVOICE voices[48];
    unsigned char unknownb70[5144 - 48 * 64];
    SNDDRVPACKETVIEW* packets[1];
};

extern SNDDRVVIEW snddrv;

static inline void stoppacket(SNDDRVVOICE* driver) {
    if (driver->packet >= 0) {
        snddrv.packets[driver->packet]->state = -1;
        driver->packet = -1;
    }
}

int SNDPLATFORM_stop(int voice) {
    SNDVOICEVIEW* sound = &(sndgs.voices)[voice];
    SNDDRVVOICE* driver = &snddrv.voices[voice];
    int i;

    if (sound->flags & 0x200) {
        stoppacket(driver);
        for (i = 0; i < sound->count; i++) {
            if (!snddrv.voices[sound->channels[i]].axvoice)
                return -8;
            AXSetVoiceState(snddrv.voices[sound->channels[i]].axvoice, 0);
            if (snddrv.voices[sound->channels[i]].allocated) {
                SNDDRV_freeaxvoice(sound->channels[i], 0);
                snddrv.voices[sound->channels[i]].allocated = 0;
            }
        }
    } else {
        for (i = 0; i < sound->count; i++) {
            MIXstopfn(sound->channels[i] - sndgs.firstvoice);
            SNDVOICEI_free(sound->channels[i]);
        }
    }
    return 0;
}
