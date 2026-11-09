// A fragment of the DSP mixer (sdspmix.c): SNDDRV_DSPMixerSetPan sets a voice's pan levels, from the Dolby
// balance table with four outputs (zero for a rear channel) or from the speaker levels otherwise. _AXVPB is
// named by the mangled symbols; the channel record and AX voice members are inferred.
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

extern "C" {
int OSDisableInterrupts();
int OSRestoreInterrupts(int);
}
void SNDDRV_DSPMixerSetAuxBus(_AXVPB*, unsigned short, int);

void SNDDRV_DSPMixerSetPan(_AXVPB* voice, float* speakers, unsigned short azimuth, int rear) {
    SNDDRVMIXCHANNEL* channel = &__MIXChannel[voice->index];

    if (numoutputchannels == 4) {
        unsigned int balance = azimuth >> 8;

        if (rear == 0) {
            channel->vR1 = SNDDRV_dolbypl2balances[balance][1];
            channel->vBR1 = SNDDRV_dolbypl2balances[balance][3];
            channel->vBL1 = SNDDRV_dolbypl2balances[balance][2];
            channel->vL1 = SNDDRV_dolbypl2balances[balance][0];
        } else {
            channel->vL1 = 0;
            channel->vBL1 = 0;
            channel->vBR1 = 0;
            channel->vR1 = 0;
        }
    } else {
        channel->vR1 = (int)(speakers[0] * 32767.0f);
        channel->vL1 = (int)(speakers[1] * 32767.0f);
    }
    channel->mode |= 0x40000000;
}

