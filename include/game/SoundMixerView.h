// Member view of the software mixer's global record sndmix and its 96-byte channel
// records. Names and types are inferred from use (Frontline's names where they apply);
// offsets come from GR8E69's disassembly.
#ifndef SOUND_MIXER_VIEW_H
#define SOUND_MIXER_VIEW_H

struct SNDMIXCHANNELVIEW {
    unsigned char state;
    unsigned char dirty;
    char pad2[26];
    float dry[7];
    float wet[7];
    int field54;
    void* lowpass;
    void* highpass;
};

struct SNDMIXVIEW {
    char pad0[12];
    short framelength;
    unsigned char quality;
    unsigned char outputmode[1];
    char pad10[456];
    SNDMIXCHANNELVIEW* channels;
    void (*mix)(void);
    void (*decode)(void);
};

extern "C" SNDMIXVIEW sndmix;

#endif
