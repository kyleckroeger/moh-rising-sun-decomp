// A fragment of the music path player: a track's latency, the tracks' control byte and
// clearing queued events.
#include "PathTrack.h"

extern "C" int PATH_latency(unsigned int handle, int latency) {
    PATHTRACKVIEW* track = PATHI_gettrackptr(handle);
    if (!track)
        return -8;
    track->latency = latency;
    return 0;
}

extern "C" int PATH_control(unsigned int handle, unsigned int value) {
    int i;
    int j;
    if (!inited || pfactive || !PATHI_switchproject(-1))
        return -18;
    if (value <= 127) {
        for (i = 0; i < 4; i++) {
            if (pfstates[i].map && (pfstates[i].id & (0xf0000000 & handle)) && (pfstates[i].id & (0x0f000000 & handle))) {
                if (PATHI_switchproject((unsigned char)i)) {
                    for (j = 0; j < 24; j++) {
                        if (pfstate->tracks[j])
                            pfstate->tracks[j]->control = value;
                    }
                }
            }
        }
    }
    pfactive = 0;
    pfstate = 0;
    return 0;
}

extern "C" void PATH_clearallevents(unsigned int handle) {
    int i;
    int j;
    if (inited && !pfactive && PATHI_switchproject(-1)) {
        for (i = 0; i < 4; i++) {
            if (pfstates[i].map && (pfstates[i].id & (0xf0000000 & handle)) && (pfstates[i].id & (0x0f000000 & handle))) {
                if (PATHI_switchproject((unsigned char)i)) {
                    for (j = 0; j < 16; j++)
                        pfstate->events[j].pending = 0;
                }
            }
        }
        pfactive = 0;
        pfstate = 0;
    }
}
