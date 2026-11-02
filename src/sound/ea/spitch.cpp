// SNDpitchmult: set the pitch multiplier on every platform voice of a sound
// (stopping early when it is unchanged). sndgs's layout is not known; the
// voice table is accessed through inferred offsets.
#include "SoundGlobals.h"
int SNDVOICEI_get(int);
int iSNDpatchkey(int, int*);
void iSNDcalcpitch(int);
void SNDPLATFORM_setpitch(int);

extern "C" int SNDpitchmult(int handle, int multiplier) {
    int voice = SNDVOICEI_get(handle);

    if (voice >= 0) {
        int key = -1;

        while (iSNDpatchkey(voice, &key)) {
            SNDVOICEVIEW* entry = &sndgs.voices[key];

            if (entry->pitchrate == multiplier)
                return 0;
            entry->pitchrate = multiplier;
            iSNDcalcpitch(key);
            SNDPLATFORM_setpitch(key);
        }
    }
    return voice;
}
