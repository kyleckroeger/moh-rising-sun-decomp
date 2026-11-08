// A fragment of the sound system's start-up file (ssysinit.c): SNDSYS_restore
// runs the module hooks, stops every voice, restores the platform and frees the
// voice and bank tables. The function name comes from the symbol; sndgs's members
// are inferred, and the hooks' signatures are not established.
#include "SoundGlobals.h"

typedef void (*SNDHOOK)(void);

extern "C" {
int SNDfxinitbus(int bus, int level, int type, int param1, int param2);
void SNDstopall(void);
void SNDSYS_entercritical(void);
void SNDSYS_leavecritical(void);
int SNDSYS_inited(void);
}
int SNDMEMI_restore();
void SNDMEMI_free(void* memory);
void SNDI_mutexfree();
int SNDPLATFORM_restore();
void SNDI_freespkrtable();

extern "C" int SNDSYS_restore(void) {
    int result;

    if (!SNDSYS_inited())
        return -14;
    SNDfxinitbus(0, 0, 0, -1, -1);
    if (sndgs.userdataclient[4])
        ((SNDHOOK)sndgs.userdataclient[4])();
    if (sndgs.userdataclient[5])
        ((SNDHOOK)sndgs.userdataclient[5])();
    if (sndgs.userdataclient[6])
        ((SNDHOOK)sndgs.userdataclient[6])();
    if (sndgs.destroystreams)
        sndgs.destroystreams();
    if (sndgs.streamclient)
        sndgs.streamclient();
    SNDI_freespkrtable();
    SNDstopall();
    if (sndgs.removebank)
        sndgs.removebank(-1);
    SNDPLATFORM_restore();
    SNDSYS_entercritical();
    SNDMEMI_free(sndgs.voices);
    SNDMEMI_free(sndgs.banks);
    SNDSYS_leavecritical();
    result = SNDMEMI_restore();
    sndgs.inited = 0;
    SNDI_mutexfree();
    return result;
}
