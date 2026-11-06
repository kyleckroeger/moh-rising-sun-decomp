/* A fragment of the sound driver (snddrv.c): the high-pass
   setting of a mixer voice's channels, and the current frame
   of a voice (for an AX voice, its playback address minus the start, scaled
   by 7/8 for type 18 samples; otherwise the software mixer's frame).
   SNDPLATFORM_stop stops a voice: an AX voice's packet player is detached
   (through an inline helper) and its AX voices stopped and released; a mixer
   voice's channels are stopped and freed.
   sndgs and snddrv are named by their symbols; their views, the voice
   records and the AX voice block's members are inferred. */
struct _AXVPB;

extern "C" {
#include "SoundGlobals.h"
extern void (*MIXstopfn)(int);
int MIX_getframe(int);
void AXSetVoiceState(_AXVPB*, unsigned short);
}
void SNDDRV_freeaxvoice(int, int);
void SNDVOICEI_free(int);
extern "C" void MIX_sethighpass(int, int);

/* inferred: an AX voice parameter block, as far as it is read here */
struct _AXVPB {
    unsigned char unknown000[434];
    unsigned short currentHi;
    unsigned short currentLo;
};


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
    unsigned char unknown0000[4812];
    signed char state;
};

struct SNDDRVVIEW {
    SNDDRVVOICE voices[48];
    unsigned char unknownb70[5144 - 48 * 64];
    SNDDRVPACKETVIEW* packets[1];
};

extern SNDDRVVIEW snddrv;

void SNDPLATFORM_highpass(int voice, int value) {
    SNDVOICEVIEW* sound = &(sndgs.voices)[voice];
    int i;

    if (sound->flags & 4) {
        for (i = 0; i < sound->count; i++)
            MIX_sethighpass(sound->channels[i] - sndgs.firstvoice, value);
    }
}

int SNDPLATFORM_getcurframe(int voice) {
    SNDDRVVOICE* driver = &snddrv.voices[voice];
    SNDVOICEVIEW* sound = &(sndgs.voices)[voice];

    if (sound->flags & 0x200) {
        int position;

        if (!driver->axvoice)
            return -8;
        position = (driver->axvoice->currentHi << 16) + driver->axvoice->currentLo;
        if (sound->type == 18)
            return ((position - driver->start) * 7) >> 3;
        return position - driver->start;
    }
    return MIX_getframe(voice - sndgs.firstvoice);
}

/* inferred: detaches a voice from its packet player */
static inline void stoppacket(SNDDRVVOICE* driver) {
    if (driver->packet >= 0) {
        snddrv.packets[driver->packet]->state = -1;
        driver->packet = -1;
    }
}

