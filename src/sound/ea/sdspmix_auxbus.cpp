// A fragment of the DSP mixer (sdspmix.c): SNDDRV_DSPMixerSetAuxBus stores a voice's
// aux send level for bus 1 or 2 in its mixer channel and marks the channel changed.
// _AXVPB is named by the mangled symbol; the channel record (76 bytes) and voice
// members are inferred.
struct _AXVPB {
    unsigned char unknown00[24];
    int index;
};

struct MIXCHANNEL {
    int field00;
    int mode;
    unsigned char unknown08[32];
    unsigned short levels[18];
};

extern MIXCHANNEL __MIXChannel[64];
extern int numoutputchannels;

void SNDDRV_DSPMixerSetAuxBus(_AXVPB* voice, unsigned short level, int bus) {
    MIXCHANNEL* channel = &__MIXChannel[voice->index];

    if (bus == 1) {
        channel->levels[9] = level;
        channel->levels[7] = level;
        if (numoutputchannels == 4) {
            channel->levels[11] = level;
            channel->levels[17] = level;
        }
    } else if (bus == 2) {
        channel->levels[15] = level;
        channel->levels[13] = level;
        if (numoutputchannels == 3)
            channel->levels[17] = level;
    }
    channel->mode |= 0x40000000;
}
