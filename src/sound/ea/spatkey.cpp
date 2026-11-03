// iSNDpatchkey: step *key through the platform voices that play the same
// patch as a voice (or yield the voice itself once when it has no patch).
// sndgs's voice table is viewed through inferred members.
#include "SoundGlobals.h"


int iSNDpatchkey(int voice, int* key) {
    SNDVOICEVIEW* entry = &(sndgs.voices)[voice];
    int patch = entry->patch;

    if (patch != 0) {
        for (;;) {
            
            (*key)++;
            if (*key >= sndgs.voicecount)
                break;
            entry = &(sndgs.voices)[*key];
            if (entry->patch == patch && (int)entry->state == 1 && entry->handle >= 0)
                return 1;
        }
    } else if (*key < 0) {
        *key = voice;
        return 1;
    }
    return 0;
}
