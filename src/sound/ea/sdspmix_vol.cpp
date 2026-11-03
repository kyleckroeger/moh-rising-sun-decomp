// A fragment of the DSP mixer (sdspmix.c): releasing a voice's mixer channel
// (clearing its voice) and setting its volume (flagging the volume for the
// next update). _AXVPB is named by the mangled symbols; the channel record and
// AX voice members are inferred. __MIXChannel is file-static in the original.
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

typedef struct MIXCHANNEL {
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
} MIXCHANNEL;

// File-local in the original (defined in sdspmix.c's part of the file).
extern MIXCHANNEL __MIXChannel[64];

void SNDDRV_DSPMixerReleaseChannel(_AXVPB* voice) {
    __MIXChannel[voice->index].voice = 0;
}

void SNDDRV_DSPMixerSetVol(_AXVPB* voice, int volume) {
    MIXCHANNEL* channel = &__MIXChannel[voice->index];

    channel->volume = volume;
    channel->mode |= 0x10000000;
}

void SNDDRV_DSPMixerUpdate();
