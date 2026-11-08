// A fragment of the DSP mixer (sdspmix.c): resetting all channels, and starting a channel from its aux, pan
// and volume settings. _AXVPB and SNDDRVMIXCHANNEL are named by the mangled symbols; the channel record and AX
// voice members are inferred.
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
void SNDDRV_DSPMixerResetChannel(SNDDRVMIXCHANNEL*);
void SNDDRV_DSPMixerSetAuxBus(_AXVPB*, unsigned short, int);
void SNDDRV_DSPMixerSetPan(_AXVPB*, float*, unsigned short, int);

void SNDDRV_DSPMixerInit(int outputs) {
    int i;

    for (i = 0; i < 64; i++)
        SNDDRV_DSPMixerResetChannel(&__MIXChannel[i]);
    numoutputchannels = outputs;
}

void SNDDRV_DSPMixerInitChannel(_AXVPB* voice, unsigned long mode, int input, unsigned short auxA, unsigned short, float* speakers, unsigned short azimuth, int rear) {
    SNDDRVMIXCHANNEL* channel = &__MIXChannel[voice->index];
    int ctrl = 0;
    int enabled;

    channel->voice = voice;
    channel->mode = mode & 4;
    channel->volume = input;
    SNDDRV_DSPMixerSetAuxBus(voice, auxA, 1);
    channel->vAL = channel->vAL1;
    channel->vAR = channel->vAR1;
    channel->vAS = channel->vAS1;
    SNDDRV_DSPMixerSetPan(voice, speakers, azimuth, rear);
    channel->vL = channel->vL1;
    channel->vR = channel->vR1;
    channel->vBL = channel->vBL1;
    channel->vBR = channel->vBR1;
    if (channel->mode & 4)
        channel->v = ctrl;
    else
        channel->v = input;
    enabled = OSDisableInterrupts();
    voice->currentVolume = channel->v;
    voice->currentDelta = 0;
    if ((voice->mix.vL = channel->vL) != 0)
        ctrl |= 1;
    voice->mix.vDeltaL = 0;
    if ((voice->mix.vR = channel->vR) != 0)
        ctrl |= 2;
    voice->mix.vDeltaR = 0;
    if ((voice->mix.vAuxAL = channel->vAL) != 0)
        ctrl |= 16;
    voice->mix.vDeltaAuxAL = 0;
    if ((voice->mix.vAuxAR = channel->vAR) != 0)
        ctrl |= 32;
    voice->mix.vDeltaAuxAR = 0;
    if ((voice->mix.vAuxBL = channel->vBL) != 0)
        ctrl |= 512;
    voice->mix.vDeltaAuxBL = 0;
    if ((voice->mix.vAuxBR = channel->vBR) != 0)
        ctrl |= 1024;
    voice->mix.vDeltaAuxBR = 0;
    if ((voice->mix.vAuxBS = channel->vBS) != 0)
        ctrl |= 4096;
    voice->mix.vDeltaAuxBS = 0;
    if ((voice->mix.vS = channel->vS) != 0)
        ctrl |= 4;
    voice->mix.vDeltaS = 0;
    if ((voice->mix.vAuxAS = channel->vAS) != 0)
        ctrl |= 128;
    voice->mixerCtrl = ctrl;
    voice->sync |= 530;
    voice->mix.vDeltaAuxAS = 0;
    OSRestoreInterrupts(enabled);
}
