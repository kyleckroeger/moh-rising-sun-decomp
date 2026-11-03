/* SNDPLATFORM_outputcaps fills sndgs with the GameCube output parameters and
   SNDPLATFORM_outputset clamps the requested settings (a fragment of snddrv.c).
   sndgs is named by its symbol; its members and the settings record are
   inferred from offsets. */
#include "SoundGlobals.h"

extern "C" unsigned long ARGetBaseAddress(void);

/* inferred: the 160-byte settings record at sndgs+32 (and its saved copy at
   sndgs+212) */
struct SNDSETTINGS {
    char unknown00[10];
    unsigned short rate;
    char unknown0c[2];
    unsigned char mixvoices;
    char unknown0f[4];
    unsigned char axvoices;
    char unknown14[18];
    unsigned char field26;
    unsigned char channels;
    char unknown28[120];
};

int SNDPLATFORM_outputcaps() {
    unsigned int base;

    sndgs.maxmixvoices = 32;
    sndgs.maxaxvoices = 48;
    sndgs.outputrate0 = 32000;
    sndgs.outputrate1 = 32000;
    sndgs.minchannels = 1;
    sndgs.maxchannels = 4;
    sndgs.field49 = 1;
    sndgs.field13 = 2;
    sndgs.field14 = 544;
    sndgs.field16 = 36;
    sndgs.outputcount = 2;
    sndgs.outputflags[0] = 544;
    sndgs.field52 = 36;
    sndgs.field2e = 8;
    sndgs.firstvoice = 48;
    sndgs.packetplayercount = 8;
    sndgs.samplerate = 32000;
    sndgs.channels = 2;
    sndgs.usesaram = 1;
    base = ARGetBaseAddress() + 256;
    sndgs.arambase = base;
    sndgs.aramsize = 0x800000 - base;
    sndgs.pan78 = 0;
    sndgs.pan84 = 0x4000;
    sndgs.pan86 = 0xc000;
    sndgs.pan90 = 0x4000;
    sndgs.pan92 = 0x8000;
    sndgs.pan94 = 0xc000;
    sndgs.pan9c = 0x2000;
    sndgs.pan9e = 0x6000;
    sndgs.pana0 = 0xa000;
    sndgs.pana2 = 0xe000;
    return 0;
}

int SNDPLATFORM_outputset() {
    SNDSETTINGS settings = *(SNDSETTINGS*)((char*)&sndgs + 32);

    if (sndgs.inited)
        *(SNDSETTINGS*)((char*)&sndgs + 32) = *(SNDSETTINGS*)((char*)&sndgs + 212);
    settings.rate = 32000;
    if (settings.channels < sndgs.minchannels)
        settings.channels = sndgs.minchannels;
    if (settings.channels > sndgs.maxchannels)
        settings.channels = sndgs.maxchannels;
    if (settings.mixvoices > sndgs.maxmixvoices)
        settings.mixvoices = sndgs.maxmixvoices;
    if (settings.axvoices > sndgs.maxaxvoices)
        settings.axvoices = sndgs.maxaxvoices;
    if (settings.field26 > 32)
        settings.field26 = 32;
    if (sndgs.inited)
        return 0;
    sndgs.voicecount = settings.mixvoices + settings.axvoices;
    *(SNDSETTINGS*)((char*)&sndgs + 32) = settings;
    return 0;
}
