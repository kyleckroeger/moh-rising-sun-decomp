// Effect buses (a file group of its own): a bus record is 20 bytes and the two
// kinds of bus, selected by the play-location flags 4 and 512, each index their
// own table in sndgs by group. SNDCTRLI_getfxbus finds a record,
// SNDCTRLI_fxbustoplayloc turns the bus flags 16 and 8 into those play-location
// flags, SNDfxinitbus stores a bus's effect type and parameters and
// SNDfxmasterlevel stores its master level and reapplies every voice's send
// level. The function names come from the symbols; the record and its members
// are inferred views in SoundGlobals.h.
#include "SoundGlobals.h"

extern "C" {
void SNDSYS_entercritical(void);
void SNDSYS_leavecritical(void);
int SNDfxlevel(int handle, int bus, int level);
int SNDfxmasterlevel(int bus, int level);
}
void SNDPLATFORM_fxinit(int group, int location);

SNDFXBUS* SNDCTRLI_getfxbus(int group, int location) {
    switch (location & 0x71c) {
    case 4:
        return &sndgs.fxreverb[group];
    case 512:
        return &sndgs.fxsecond[group];
    }
    return 0;
}

int SNDCTRLI_fxbustoplayloc(int bus) {
    int location = 0;

    if (bus & 0x10)
        location = 4;
    if (bus & 8)
        location |= 512;
    return location;
}

extern "C" int SNDfxinitbus(int bus, int level, int type, int param1, int param2) {
    int pending;
    int group;
    int flag;

    if (!(bus & 0x1f8))
        bus |= 0x1f8;
    flag = 4;
    pending = SNDCTRLI_fxbustoplayloc(bus);
    group = bus & ~0x1f8;
    for (; pending != 0; flag <<= 1) {
        if (flag & pending) {
            SNDFXBUS* fx = SNDCTRLI_getfxbus(group, flag);

            fx->type = type;
            fx->param1 = param1;
            fx->param2 = param2;
            SNDPLATFORM_fxinit(group, flag);
            pending &= ~flag;
        }
    }
    if (type == 0)
        level = 0;
    SNDfxmasterlevel(bus, level);
    return 0;
}

extern "C" int SNDfxmasterlevel(int bus, int level) {
    int i;
    int group = bus & ~0x1f8;

    if (!(bus & 0x1f8))
        bus |= 0x1f8;
    if (bus & 0x10)
        sndgs.fxreverb[group].level = level;
    if (bus & 8)
        sndgs.fxsecond[group].level = level;
    SNDSYS_entercritical();
    for (i = 0; i < sndgs.voicecount; i++) {
        char* entry = (char*)sndgs.voices + i * 140;
        signed char* levels = (signed char*)(entry + 97);

        SNDfxlevel(*(int*)entry, group, levels[group]);
    }
    SNDSYS_leavecritical();
    return 0;
}
