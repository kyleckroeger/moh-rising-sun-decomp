// Member views of the sound library's two global records, sndgs and sndmix. Only the
// members the reconstructed code touches are named; all names and types are inferred
// from use (the Frontline reconstruction's names where they apply), and the
// offsets come from GR8E69's disassembly. Each is a single struct object in the
// original: its members are reached through one base register.
#ifndef SOUND_GLOBALS_H
#define SOUND_GLOBALS_H

struct SNDVOICEVIEW {
    int handle;
    short channels[6];
    char pad10[4];
    unsigned int length;
    char pad18[8];
    unsigned short rate;
    unsigned char type;
    unsigned char count;
    unsigned short flags;
    unsigned char patch;
    char pad27[1];
    short master;
    char pad2a[1];
    signed char soundvolume;
    char pad2c[12];
    short volume;
    char pad3a[6];
    short attrvolume;
    char pad42[6];
    float outvolume;
    char pad4c[20];
    signed char send[1];
    signed char level[1];
    short fxlevel[1];
    signed char wheel;
    char pad65[2];
    unsigned char key;
    unsigned char lfo;
    unsigned char state;
    char pad6a[6];
    signed char* curve;
    signed char* wheelTable;
    signed char* keyscale;
    signed char* lfoTable;
    short lfoDepth;
    unsigned short bend;
    short detune;
    unsigned short pitch;
    unsigned short pitchrate;
    unsigned short scaled;
};

struct SNDGSVIEW {
    char pad0[40];
    unsigned short bankcount;
    unsigned short samplerate;
    char pad2c[7];
    unsigned char firstvoice;
    char pad34[18];
    unsigned char packetplayercount;
    char pad47[302];
    unsigned char mastervolume;
    unsigned char critical;
    char pad177[2];
    unsigned char userdataclients;
    unsigned char voicequality;
    char pad17b[3];
    short voicecount;
    char pad180[112];
    struct SNDVOICEVIEW* voices;
    struct SNDBANKSLOT* banks;
};

extern "C" SNDGSVIEW sndgs;

#endif
