// A fragment of the music path player: one service pass over a project's tracks, starting events,
// moving tracks to their next nodes, and popping the event queue when every track has taken it.
#include "PathTrack.h"

extern "C" void PATHI_service(int index, int master) {
    int window;
    if (bankservice == 66)
        window = pfstate->timerWindow;
    else
        window = pfstate->window;
    PATHEVENTVIEW* event = &pfstate->events[0];
    if (event->pending && pfstate->leader == -1 && !event->started)
        event->type = 1;
    int half = window / 2;
    int j;
    int deadline;
    int difference;
    for (j = 0; j < 24; j++) {
        if ((pfstate->leader == j) != master)
            continue;
        PATHTRACKVIEW* track = pfstate->tracks[j];
        if (!track || track->paused)
            continue;
        if ((track->latency <= 49) != (bankservice == 66))
            continue;
        int now = milliseconds + track->latency;
        int playing = 0;
        if (track->node >= 0 && track->current)
            playing = 1;
        int active = playing;
        int waiting = 0;
        if (event->pending) {
            if (event->pending >> j & 1)
                waiting = 0;
            else
                waiting = 1;
        }
        int eventWaiting = waiting;
        if (waiting) {
            if (!active && !event->bit && !event->value0c) {
                if ((event->pending | 1 << j) == -1)
                    event->type = 1;
            }
        }
        if (waiting) {
            if (event->type || event->value0c) {
                int until = 0;
                if (event->value0c)
                    until = event->value0c - now;
                if (until <= window) {
                    if (event->started && !active)
                        track->time = event->value0c;
                    if (PATHI_eventstart(j) < 0)
                        continue;
                    active = 1;
                }
            }
        }
        if (!active)
            continue;
        if (track->time) {
            if (track->time > milliseconds + half)
                continue;
            if (track->savedMode & 64) {
                track->time = 0;
                int fadeTime = pfstate->fadeTime;
                unsigned int handle = pfstate->id + (1 << j);
                unsigned int other = handle ^ 0xf0000000;
                PATHTRACKVIEW* source = PATHI_gettrackptr(other);
                if (source) {
                    pfstate = 0;
                    pfactive = 0;
                    track->baseVolume = source->baseVolume;
                    track->volume = source->volume;
                    PATH_fade(handle, source->baseVolume, fadeTime);
                    PATH_fade(other, 0, fadeTime);
                    PATH_volume(other, 0);
                    PATHI_switchproject(index);
                }
            }
            PATHX_modifyhold(track->xtrack, 0);
            track->time = 0;
            continue;
        }
        if (!waiting || event->type) {
            if (!PATHX_readyfornewrequest(track->xtrack))
                continue;
        }
        int remaining = PATHI_timeremaining(track);
        if (waiting && !event->type) {
            eventWaiting = 0;
            PATHNODEVIEW* entry = (PATHNODEVIEW*)((int*)pfstate->map + pfstate->nodemap[track->node]);
            int total = remaining;
            if (track->beat > 0) {
                int beats = entry->beatlen * entry->beatnum + 1;
                total = remaining + track->beatlength * (beats - track->beat);
            }
            int end = milliseconds + total;
            deadline = event->value0c;
            difference = deadline - end;
            if (difference < 0)
                difference = -difference;
            if (end > deadline || (event->bit == j && difference > 10)) {
                event->value0c = end;
                event->bit = j;
                if (event->started)
                    PATHI_eventdeadline(-1, event->node, end);
            }
        }
        if (track->start > 0 ? track->start > (now += window) : remaining > track->latency)
            continue;
        if (eventWaiting && track->beat <= 0) {
            if (PATHI_eventstart(j) >= 0)
                continue;
        }
        if (!track->fadeVolume && !PATHX_getvolume(track->xtrack)) {
            PATHX_stop(track->xtrack);
            track->node = -1;
            track->fadeVolume = -1;
        } else {
            PATHI_seeknextnode(j);
        }
    }
    if (event->pending == -1 && !master && (unsigned int)event->value10 < (unsigned int)milliseconds) {
        int k;
        PATHI_setvolumes(0xffffff);
        for (k = 0; k < 15; k++)
            pfstate->events[k] = pfstate->events[k + 1];
        pfstate->events[15].pending = 0;
    }
}
