/* SNDPLATFORM_packetplay (a fragment of the sound driver, snddrv.c): starts a packet
   player's voice: through the software mixer's hooks when the voice is mixed, else by
   resetting the packet record and clearing and posting its DSP buffers. SNDSAMPLEFORMAT
   and SNDSAMPLEATTR are named by the mangled symbol; the views and member names are
   inferred from offsets. */
#include "SoundGlobals.h"
struct _AXVPB;

extern "C" {
void AXSetVoiceSrcType(_AXVPB*, unsigned long);
void* memset(void*, int, unsigned long);
void DCFlushRange(void*, unsigned long);
}

struct SNDSAMPLEFORMAT {
    unsigned short rate;
    unsigned char channels;
};

struct SNDSAMPLEATTR {
    char pad0[7];
    unsigned char field7;
    char pad8[16];
    void* data[6];
};

/* inferred: a packet player's platform record */
struct SNDDRVPKTCHAN {
    unsigned char buffer[9600];
    void* staging[6];
    signed char voice;
    unsigned char half;
    signed char countdown;
    unsigned char channels;
    void* dsp[6];
    int middle;
    short consumed[6];
    int field25c4;
};

/* inferred: the driver's per-channel record */
struct SNDDRVVOICE {
    char unknown0[28];
    unsigned char packetindex;
    char unknown29[7];
    volatile unsigned char allocated;
    char unknown25[23];
    _AXVPB* axvoice;
};

struct SNDDRVVIEW {
    SNDDRVVOICE voices[80];
    char pad[24];
    SNDDRVPKTCHAN* packets[1];
};

extern SNDDRVVIEW snddrv;
extern void (*MIXplayinitfn)(int, int, int, void*, void*, unsigned char*, int, int, int, int, int, int, int, int);
extern void (*MIXplayfn)(int);

int SNDPLATFORM_setpitch(int);
void SNDPLATFORM_set3dpos(int);
void SNDPLATFORM_setvol(int);
int SNDPLATFORM_setfxlevel(int, int);
void SNDPLATFORM_timemult(int, int);
void SNDPLATFORM_lowpass(int, int);
void SNDPLATFORM_highpass(int, int);
int SNDDRV_allocateaxvoice(int);
int SNDDRV_dmapost(unsigned long, void*, void*, unsigned long, int);

int SNDPLATFORM_packetplay(int index, int voiceindex, int timemult, int, int lowpass, int highpass, SNDSAMPLEFORMAT* format, SNDSAMPLEATTR* attr) {
    SNDVOICEVIEW* voice = &sndgs.voices[voiceindex];
    SNDDRVVOICE* own = &snddrv.voices[voiceindex];
    int i;

    voice->rate = format->rate;
    voice->length = 0;
    own->packetindex = index;
    voice->count = format->channels;
    if (voice->flags & 4) {
        for (i = 0; i < voice->count; i++)
            MIXplayinitfn(voice->channels[i] - sndgs.firstvoice, voice->type, 0, 0, 0, (unsigned char*)attr->data[i], voice->count, 0, -1, -1, 0, 0, attr->field7, i);
        SNDPLATFORM_setpitch(voice->channels[0]);
        SNDPLATFORM_set3dpos(voice->channels[0]);
        SNDPLATFORM_setvol(voice->channels[0]);
        SNDPLATFORM_setfxlevel(voice->channels[0], 0);
        SNDPLATFORM_timemult(voice->channels[0], timemult);
        SNDPLATFORM_lowpass(voice->channels[0], lowpass);
        SNDPLATFORM_highpass(voice->channels[0], highpass);
        for (i = 0; i < voice->count; i++)
            MIXplayfn(voice->channels[i] - sndgs.firstvoice);
    } else {
        SNDDRVPKTCHAN* packet = snddrv.packets[index];

        for (i = 0; i <= 5; i++)
            packet->consumed[i] = 0;
        packet->field25c4 = 0;
        packet->countdown = 2;
        packet->half = 1;
        packet->voice = voiceindex;
        packet->channels = format->channels;
        SNDDRV_allocateaxvoice(voiceindex);
        for (i = 0; i < format->channels; i++) {
            unsigned int start = (unsigned int)packet->dsp[i];
            unsigned int low = start >> 1;
            unsigned int high = (start + 3070) >> 1;

            if (i == 0)
                packet->middle = (low + high) >> 1;
            memset(packet->staging[i], 0, 1536);
            DCFlushRange(packet->staging[i], 1536);
            SNDDRV_dmapost(1, packet->staging[i], packet->dsp[i], 1536, -1);
            AXSetVoiceSrcType(snddrv.voices[voice->channels[i]].axvoice, 1);
            snddrv.voices[voice->channels[i]].allocated = 1;
        }
    }
    return 0;
}
