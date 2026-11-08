/* iSNDserve (a fragment of the sound driver, snddrv.c): the driver's service pass.
   For each packet player: when its start countdown runs out, sets up its AX voices
   as looping 16-bit PCM over the channel buffers and starts them; when the playback
   position crosses to the other half of the buffer, frees the frames consumed from
   that half and refills it. Then frees the AX voices of finished voices. SNDDRVPKTCHAN
   and CHANPUB are named by the mangled symbols; the views and member names are inferred. */
#include "SoundGlobals.h"

/* inferred: the parts of the AX parameter block read here */
struct _AXVPB {
    char pad0[326];
    unsigned short state;
    char pad148[106];
    unsigned short currentHi;
    unsigned short currentLo;
};

struct AXPBADDRVIEW {
    unsigned short loopFlag;
    unsigned short format;
    unsigned short loopAddressHi;
    unsigned short loopAddressLo;
    unsigned short endAddressHi;
    unsigned short endAddressLo;
    unsigned short currentAddressHi;
    unsigned short currentAddressLo;
};

struct AXPBADPCMVIEW {
    unsigned short a[8][2];
    unsigned short gain;
    unsigned short predScale;
    unsigned short yn1;
    unsigned short yn2;
};

struct AXPBADPCMLOOPVIEW {
    unsigned short predScale;
    unsigned short yn1;
    unsigned short yn2;
};

extern "C" {
void AXSetVoiceAdpcm(_AXVPB*, AXPBADPCMVIEW*);
void AXSetVoiceAdpcmLoop(_AXVPB*, AXPBADPCMLOOPVIEW*);
void AXSetVoiceAddr(_AXVPB*, AXPBADDRVIEW*);
void AXSetVoiceState(_AXVPB*, unsigned short);
void* memset(void*, int, unsigned long);
}

/* inferred: a packet player's platform record */
struct SNDDRVPKTCHAN {
    unsigned char buffer[9600];
    unsigned char* staging[6];
    signed char voice;
    unsigned char half;
    signed char countdown;
    char pad259b[1];
    unsigned int dsp[6];
    int middle;
    short consumed[2];
};

struct CHANPUB;

/* inferred: the driver's per-channel record */
struct SNDDRVVOICE {
    char unknown0[24];
    int start;
    char unknown28[8];
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

int SNDPLATFORM_setpitch(int);
void SNDPLATFORM_set3dpos(int);
void SNDPLATFORM_setvol(int);
void SNDPKTPLAYI_freeframes(int, int, int);
void SNDDRV_fillbufwithpackets(int, SNDDRVPKTCHAN*, CHANPUB*);
void SNDPKTPLAYI_flushcallbackdata();
void SNDDRV_freeaxvoice(int, int);
void SNDDRV_freenextframe();
void SNDDRV_DSPMixerUpdate();

void iSNDserve() {
    int i;
    int j;
    SNDVOICEVIEW* voice;
    SNDDRVVOICE* own;
    AXPBADPCMVIEW adpcm;
    AXPBADPCMLOOPVIEW loop;
    AXPBADDRVIEW addr;

    for (i = 0; i < sndgs.packetplayercount; i++) {
        SNDDRVPKTCHAN* packet = snddrv.packets[i];

        if (packet && packet->voice >= 0) {
            int half;

            voice = &sndgs.voices[packet->voice];
            own = &snddrv.voices[packet->voice];

            if (!(packet->countdown & 0x80)) {
                if (--packet->countdown & 0x80) {
                    addr.format = 10;
                    adpcm.gain = 2048;
                    addr.loopFlag = 1;
                    memset(&loop, 0, 6);
                    SNDPLATFORM_setpitch(packet->voice);
                    SNDPLATFORM_set3dpos(packet->voice);
                    SNDPLATFORM_setvol(packet->voice);
                    for (j = 0; j < voice->count; j++) {
                        unsigned int start = packet->dsp[j];
                        unsigned int end = start + 3070;
                        unsigned int startlow = start >> 1;
                        unsigned int endlow = end >> 1;

                        AXSetVoiceAdpcm(snddrv.voices[voice->channels[j]].axvoice, &adpcm);
                        AXSetVoiceAdpcmLoop(snddrv.voices[voice->channels[j]].axvoice, &loop);
                        unsigned int starthi = start >> 17;
                        unsigned int endhi = end >> 17;

                        addr.endAddressHi = endhi;
                        addr.endAddressLo = endlow;
                        addr.currentAddressHi = starthi;
                        addr.currentAddressLo = startlow;
                        addr.loopAddressHi = starthi;
                        addr.loopAddressLo = startlow;
                        AXSetVoiceAddr(snddrv.voices[voice->channels[j]].axvoice, &addr);
                        AXSetVoiceState(snddrv.voices[voice->channels[j]].axvoice, 1);
                        snddrv.voices[voice->channels[j]].allocated = 1;
                    }
                }
            }
            half = (own->axvoice->currentHi << 16) + own->axvoice->currentLo > packet->middle;
            if (half != packet->half) {
                if (packet->consumed[packet->half]) {
                    for (j = 0; j < voice->count; j++)
                        SNDPKTPLAYI_freeframes(i, j, packet->consumed[packet->half]);
                    packet->consumed[packet->half] = 0;
                }
                SNDDRV_fillbufwithpackets(i, packet, (CHANPUB*)voice);
                packet->half = half;
            }
        }
    }
    SNDPKTPLAYI_flushcallbackdata();
    for (i = 0; i < sndgs.firstvoice; i++) {
        voice = &sndgs.voices[i];
        own = &snddrv.voices[i];

        if (own->axvoice && own->allocated && !own->axvoice->state && own->start >= 0 && own->allocated == 1) {
            for (j = 0; j < sndgs.voices[i].count; j++) {
                if (snddrv.voices[voice->channels[j]].allocated) {
                    SNDDRV_freeaxvoice(voice->channels[j], 0);
                    snddrv.voices[voice->channels[j]].allocated = 0;
                }
            }
        }
    }
    SNDDRV_freenextframe();
    SNDDRV_DSPMixerUpdate();
}
