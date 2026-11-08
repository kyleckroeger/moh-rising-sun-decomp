// A fragment of the music path player: time to a track's next stream beat and the time left
// in its queue.
#include "PathTrack.h"

extern "C" int PATHI_timetonextstreambeat(PATHTRACKVIEW* track, int* length) {
    int position;
    int remaining;
    if (!track)
        return 0;
    PATHNODEVIEW* entry = (PATHNODEVIEW*)((int*)pfstate->map + pfstate->nodemap[track->node]);
    int beat = pfstate->samples[entry->sample - 1].length;
    if (entry->beatlen)
        beat /= entry->beatlen * entry->beatnum;
    PATHX_status(track->xtrack, &position, &remaining);
    int result = remaining % beat;
    if (length)
        *length = beat;
    return result;
}

extern "C" int PATHI_timeremaining(PATHTRACKVIEW* track) {
    int total = 0;
    unsigned int i;
    for (i = 0; i < track->queued; i++) {
        int request = track->queue[i].request;
        if (request >= 0) {
            int remaining = PATHX_timeremaining(track->xtrack, request);
            if (remaining < 0)
                total += track->queue[i].length;
            else
                total += remaining;
        }
    }
    return total;
}
