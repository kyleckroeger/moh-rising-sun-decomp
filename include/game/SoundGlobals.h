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
    char pad18[4];
    short azimuth;
    short distance;
    unsigned short rate;
    unsigned char type;
    unsigned char count;
    unsigned short flags;
    unsigned char patch;
    char pad27[1];
    short master;
    unsigned char priority;
    signed char soundvolume;
    char pad2c[12];
    short volume;
    char pad3a[6];
    short attrvolume;
    char pad42[6];
    float outvolume;
    unsigned short baseazimuth;
    char pad4e[17];
    signed char field5f;
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
    unsigned short outputrate0;
    unsigned short outputrate1;
    unsigned char minchannels;
    unsigned char maxchannels;
    unsigned char maxmixvoices;
    char pad7[4];
    unsigned char maxaxvoices;
    char padc[7];
    unsigned char field13;
    unsigned short field14;
    unsigned short field16;
    char pad18[16];
    unsigned short bankcount;
    unsigned short samplerate;
    char pad2c[2];
    unsigned char field2e;
    char pad2f[4];
    unsigned char firstvoice;
    float fieldrate;
    char pad38[13];
    unsigned char usesaram;
    unsigned char packetplayercount;
    unsigned char channels;
    char pad48[1];
    unsigned char field49;
    char pad4a[2];
    unsigned char outputcount;
    char pad4d[1];
    unsigned short outputflags[1];
    unsigned short field52;
    char pad52[10];
    unsigned int arambase;
    unsigned int aramsize;
    char pad64[20];
    unsigned short pan78;
    char pad7a[10];
    unsigned short pan84;
    unsigned short pan86;
    char pad88[8];
    unsigned short pan90;
    unsigned short pan92;
    unsigned short pan94;
    char pad96[6];
    unsigned short pan9c;
    unsigned short pan9e;
    unsigned short pana0;
    unsigned short pana2;
    char pada4[28];
    void* (*memallochook)(int);
    void (* volatile memfreehook)(unsigned int);
    char padc8[172];
    unsigned char inited;
    unsigned char mastervolume;
    unsigned char critical;
    char pad177[2];
    signed char userdataclients;
    unsigned char voicequality;
    char pad17b[3];
    short voicecount;
    unsigned int tick;
    char pad184[60];
    int ticksnapped;
    float rate;
    void (*userdataclient[7])(void*);
    int (*removebank)(int);
    char pad1e8[4];
    void (*destroystreams)(void);
    struct SNDVOICEVIEW* voices;
    struct SNDBANKSLOT* banks;
    char pad1f8[284];
    void (*prehook)(void);
    void (*posthook)(void);
};

extern "C" SNDGSVIEW sndgs;

#endif
