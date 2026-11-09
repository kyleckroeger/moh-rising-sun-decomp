// A fragment of the music path player: ending an event on the tracks that hold it, restoring
// their volume, then stopping them or re-entering their saved node.
#include "PathTrack.h"

extern "C" void PATHI_eventrestore(int event) {
    int j;
    for (j = 0; j < 24; j++) {
        PATHTRACKVIEW* track = pfstate->tracks[j];
        if (track && track->current && event >= 0 && track->event == event) {
            int volume = track->savedVolume & 0x7f;
            PATHX_volume(track->xtrack, volume * track->volume / 100);
            track->baseVolume = volume;
            if (track->mode & 2) {
                track->request = -1;
                track->node = -1;
                track->start = 0;
                track->current = 0;
            } else if (track->mode & 1) {
                track->node = PATHI_enternode(0, track->restoreNode, track->control);
                track->savedMode = track->mode;
                int beat = 1;
                if (track->latency > 49)
                    beat = -1;
                track->start = 0;
                track->beat = beat;
                PATHI_queuenode(track, 0);
            }
            track->event = -1;
        }
    }
}
