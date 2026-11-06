// SNDmastervol: set the master volume and recompute every playing voice.
// sndgs's layout is not known; its fields are accessed through inferred
// offsets.
extern "C" {
#include "SoundGlobals.h"
void SNDSYS_entercritical(void);
void SNDSYS_leavecritical(void);
}
void iSNDcalcvol(int);
void SNDPLATFORM_setvol(int);


extern "C" int SNDmastervol(int volume) {
    int i;

    SNDSYS_entercritical();
    sndgs.mastervolume = volume;
    for (i = 0; i < sndgs.voicecount; i++) {
        SNDVOICEVIEW* entry = &(sndgs.voices)[i];

        if ((int)entry->state == 1 && entry->handle >= 0) {
            iSNDcalcvol(i);
            SNDPLATFORM_setvol(i);
        }
    }
    SNDSYS_leavecritical();
    return 0;
}
