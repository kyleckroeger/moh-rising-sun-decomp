/* SNDPLATFORM_getvoicerange (a fragment of the sound driver, snddrv.c): the
   driver voice range of a sample type. sndgs is named by its symbol; its view
   is inferred. */
#include "SoundGlobals.h"

void SNDPLATFORM_getvoicerange(int mode, int* low, int* high) {
    if (mode & 0x200) {
        *low = 0;
        *high = sndgs.firstvoice;
    } else if (mode & 4) {
        *low = sndgs.firstvoice;
        *high = sndgs.voicecount;
    }
}
