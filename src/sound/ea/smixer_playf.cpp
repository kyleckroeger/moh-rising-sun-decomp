// A fragment of the software mixer (smixer.c): MIX_play starts a channel (gain 0,
// the current volumes from the targets). sndmix and the function are named by
// their symbols; the record and mixer views are inferred.
struct SNDSTAGEVIEW;

struct SNDMIXCHANNELVIEW {
    unsigned char state;
    unsigned char value01;
    unsigned char unknown02[2];
    float volume[6];
    float target[6];
    float level[1];
    float leveltarget[1];
    float gain;
    SNDSTAGEVIEW* filters;
    int result;
    SNDSTAGEVIEW* unpack;
    SNDSTAGEVIEW* stretch;
    int value50;
    int value54;
    int value58;
    int value5c;
};

struct SNDMIXVIEW {
    unsigned char unknown000[5];
    unsigned char outputs;
    unsigned char unknown006[466];
    SNDMIXCHANNELVIEW* channels;
};

extern "C" SNDMIXVIEW sndmix;

extern "C" void MIX_play(int index) {
    SNDMIXCHANNELVIEW* channel = &sndmix.channels[index];
    int i;

    channel->gain = 0.0f;
    channel->value01 = 0;
    for (i = 0; i < 1; i++)
        channel->level[i] = channel->leveltarget[i];
    for (i = 0; i < sndmix.outputs; i++)
        channel->volume[i] = channel->target[i];
    channel->state = 2;
}
