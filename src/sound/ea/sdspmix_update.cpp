// A fragment of the DSP mixer (sdspmix.c): SNDDRV_DSPMixerUpdate walks the channels, moving pending volume and
// pan targets into the current values and writing them to the AX voice with deltas over 320 samples. _AXVPB is
// named by the mangled symbols; the channel record and AX voice members are inferred.
#include "SoundGlobals.h"
struct AXPBMIXVIEW {
    unsigned short vL;
    unsigned short vDeltaL;
    unsigned short vR;
    unsigned short vDeltaR;
    unsigned short vAuxAL;
    unsigned short vDeltaAuxAL;
    unsigned short vAuxAR;
    unsigned short vDeltaAuxAR;
    unsigned short vAuxBL;
    unsigned short vDeltaAuxBL;
    unsigned short vAuxBR;
    unsigned short vDeltaAuxBR;
    unsigned short vAuxBS;
    unsigned short vDeltaAuxBS;
    unsigned short vS;
    unsigned short vDeltaS;
    unsigned short vAuxAS;
    unsigned short vDeltaAuxAS;
};

struct _AXVPB {
    char field000[24];
    unsigned long index;
    unsigned long sync;
    char field020[292];
    unsigned short mixerCtrl;
    unsigned short field146;
    char field148[2];
    AXPBMIXVIEW mix;
    char field16E[46];
    unsigned short currentVolume;
    unsigned short currentDelta;
};

struct SNDDRVMIXCHANNEL {
    _AXVPB* voice;
    unsigned long mode;
    int volume;
    int field0C;
    int field10;
    char field14[16];
    unsigned short v;
    unsigned short v1;
    unsigned short vL;
    unsigned short vL1;
    unsigned short vR;
    unsigned short vR1;
    unsigned short vS;
    unsigned short vS1;
    unsigned short vAL;
    unsigned short vAL1;
    unsigned short vAR;
    unsigned short vAR1;
    unsigned short vAS;
    unsigned short vAS1;
    unsigned short vBL;
    unsigned short vBL1;
    unsigned short vBR;
    unsigned short vBR1;
    unsigned short vBS;
    unsigned short vBS1;
};


extern SNDDRVMIXCHANNEL __MIXChannel[64];
extern int numoutputchannels;
extern unsigned short SNDDRV_dolbypl2balances[256][4];


void SNDDRV_DSPMixerUpdate() {
    int i;

    for (i = 0; i < 64; i++) {
        int volumechanged = 0;
        int panchanged = 0;
        SNDDRVMIXCHANNEL* channel = &__MIXChannel[i];
        _AXVPB* voice = channel->voice;

        if (voice) {
            int ctrl = 0;

            if (channel->mode & 0x20000000) {
                channel->mode &= ~0x20000000;
                volumechanged = 1;
                channel->v = channel->v1;
            }
            if (channel->mode & 0x10000000) {
                if (channel->mode & 4)
                    channel->v1 = ctrl;
                else
                    channel->v1 = channel->volume;
                channel->mode = (channel->mode & ~0x10000000) | 0x20000000;
                volumechanged = 1;
            }
            if (channel->mode & 0x80000000) {
                channel->vL = channel->vL1;
                channel->vR = channel->vR1;
                channel->vS = channel->vS1;
                channel->vAL = channel->vAL1;
                channel->vAR = channel->vAR1;
                channel->vAS = channel->vAS1;
                channel->vBL = channel->vBL1;
                channel->vBR = channel->vBR1;
                channel->vBS = channel->vBS1;
                channel->mode &= ~0x80000000;
                panchanged = 1;
            }
            if (channel->mode & 0x40000000) {
                channel->mode = (channel->mode & ~0x40000000) | 0x80000000;
                panchanged = 1;
            }
            if (volumechanged && voice) {
                voice->currentVolume = channel->v1;
                voice->sync |= 512;
                voice->currentDelta = 0;
            }
            if (panchanged) {
                if ((voice->mix.vL = channel->vL) != 0)
                    ctrl |= 1;
                if ((voice->mix.vDeltaL = (channel->vL1 - channel->vL) / 320) != 0)
                    ctrl |= 8;
                if ((voice->mix.vR = channel->vR) != 0)
                    ctrl |= 2;
                if ((voice->mix.vDeltaR = (channel->vR1 - channel->vR) / 320) != 0)
                    ctrl |= 8;
                if ((voice->mix.vAuxAL = channel->vAL) != 0)
                    ctrl |= 16;
                if ((voice->mix.vDeltaAuxAL = (channel->vAL1 - channel->vAL) / 320) != 0)
                    ctrl |= 64;
                if ((voice->mix.vAuxAR = channel->vAR) != 0)
                    ctrl |= 32;
                if ((voice->mix.vDeltaAuxAR = (channel->vAR1 - channel->vAR) / 320) != 0)
                    ctrl |= 64;
                if ((voice->mix.vAuxBL = channel->vBL) != 0)
                    ctrl |= 512;
                if ((voice->mix.vDeltaAuxBL = (channel->vBL1 - channel->vBL) / 320) != 0)
                    ctrl |= 2048;
                if ((voice->mix.vAuxBR = channel->vBR) != 0)
                    ctrl |= 1024;
                if ((voice->mix.vDeltaAuxBR = (channel->vBR1 - channel->vBR) / 320) != 0)
                    ctrl |= 2048;
                if ((voice->mix.vAuxBS = channel->vBS) != 0)
                    ctrl |= 4096;
                if ((voice->mix.vDeltaAuxBS = (channel->vBS1 - channel->vBS) / 320) != 0)
                    ctrl |= 8192;
                if ((voice->mix.vS = channel->vS) != 0)
                    ctrl |= 4;
                if ((voice->mix.vDeltaS = (channel->vS1 - channel->vS) / 320) != 0)
                    ctrl |= 8;
                if ((voice->mix.vAuxAS = channel->vAS) != 0)
                    ctrl |= 128;
                if ((voice->mix.vDeltaAuxAS = (channel->vAS1 - channel->vAS) / 320) != 0)
                    ctrl |= 256;
                if (sndgs.channels == 4)
                    ctrl |= 16384;
                voice->mixerCtrl = ctrl;
                voice->sync |= 18;
            }
        }
    }
}
