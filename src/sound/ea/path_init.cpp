// A fragment of the music path player: clearing the project states, and selecting the
// current state by index or by voice id.
#include "PathTrack.h"

extern "C" int PATHI_init(void) {
    int i;
    int j;
    if (pfactive)
        return -18;
    for (i = 0; i < 4; i++) {
        pfstates[i].map = 0;
        for (j = 23; j >= 0; j--)
            pfstates[i].tracks[j] = 0;
    }
    inited = 1;
    pfstate = 0;
    paused = 0;
    return 0;
}

extern "C" unsigned int PATHI_switchproject(unsigned int index) {
    if (!pfactive)
        pfactive = -1;
    if (index <= 3 && pfstates[index].map) {
        unsigned int id = pfstates[index].id;
        pfstate = &pfstates[index];
        pfactive = id;
        return id;
    }
    if (index != -1)
        pfactive = 0;
    return pfactive;
}

extern "C" unsigned int PATHI_switchvoice(unsigned int voice) {
    int i;
    for (i = 0; i < 4; i++) {
        if (pfstates[i].map && pfstates[i].id == voice)
            return PATHI_switchproject(i);
    }
    return 0;
}
