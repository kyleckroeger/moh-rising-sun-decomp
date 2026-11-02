// SNDPKTPLAYI_voicetopackethandle: the packet player that owns a voice, or -1.
// sndgs and sndpps layouts are not known; their fields are read through
// inferred offsets.
#include "SoundGlobals.h"

struct SNDPPSVIEW {
    char field0[772];
    int* players[1];
};

extern "C" SNDPPSVIEW sndpps;
int SNDVOICEI_get(int);

int SNDPKTPLAYI_voicetopackethandle(int voice) {
    int i;

    for (i = 0; i < sndgs.packetplayercount; i++) {
        int* player = sndpps.players[i];

        if (player && SNDVOICEI_get(*player) == voice)
            return i;
    }
    return -1;
}
