// A fragment of the music path player: setting a queued event's deadline, and moving a track
// to its next node, restoring the track if the node cannot be queued.
#include "PathTrack.h"

extern "C" void PATHI_eventdeadline(unsigned int handle, int node, int value) {
    int i;
    int j;
    for (i = 0; i < 4; i++) {
        if (handle & (0x1000000 << i)) {
            PATHEVENTVIEW* event = pfstates[i].events;
            for (j = 0; j < 16; j++) {
                if (event[j].pending && event[j].node == node)
                    event[j].value0c = value;
            }
        }
    }
}

extern "C" void PATHI_seeknextnode(int index) {
    int node = -1;
    PATHTRACKVIEW* track = pfstate->tracks[index];
    PATHTRACKVIEW saved = *track;
    int beat = 1;
    PATHNODEVIEW* entry = (PATHNODEVIEW*)((int*)pfstate->map + pfstate->nodemap[track->node]);
    if (track->latency > 49)
        beat = -1;
    if (track->beat > 0 && track->beat <= entry->beatlen * entry->beatnum) {
        node = track->node;
        beat = track->beat;
    } else if (track->node >= 0) {
        node = PATHI_nextnode(track->node, track->control);
    }
    if (node >= 0)
        node = PATHI_enternode(track->node, node, track->control);
    track->node = node;
    track->beat = beat;
    track->start = 0;
    if (PATHI_queuenode(track, 0) < 0)
        *track = saved;
}
