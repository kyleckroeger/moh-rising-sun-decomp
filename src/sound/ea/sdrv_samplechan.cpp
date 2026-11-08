/* A fragment of the sound driver (snddrv.c): SNDDRV_getsamplechan finds the
   channel a driver voice occupies within its master voice (1 to 5, or 0). The
   voice table is reached through byte offsets, which gives the table base first
   in the additions. sndgs is named by its symbol; its view and the voice records
   are inferred. */
#include "SoundGlobals.h"


int SNDDRV_getsamplechan(int voice) {
    int index = voice + sndgs.firstvoice;
    char* voices = (char*)sndgs.voices;
    char* entry = voices + index * sizeof(SNDVOICEVIEW);
    int master = ((SNDVOICEVIEW*)entry)->master;
    short* channels;
    int i;

    if (master == -1)
        return 0;
    entry = voices + master * sizeof(SNDVOICEVIEW);
    channels = ((SNDVOICEVIEW*)entry)->channels;
    for (i = 1; i < 6; i++) {
        if (channels[i] == index)
            return i;
    }
    return 0;
}
