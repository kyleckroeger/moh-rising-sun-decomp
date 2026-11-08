// A fragment of the music path player: queuing a track's node on its stream and recording it in
// the track's ring, and choosing the beat a node should start on relative to the leading track.
#include "PathTrack.h"

extern "C" int PATHI_queuenode(PATHTRACKVIEW* track, int flag) {
    int priority = 100;
    int beat;
    if (track->node >= 0) {
        int remaining = PATHI_timeremaining(track);
        int volume;
        if (track->latency > 49) {
            if (remaining <= 0) {
                if (!track->time)
                    track->time = milliseconds;
                if (track->start > 0)
                    track->time = track->start;
                priority = -1;
            }
            track->beat = -1;
        } else if (track->beat <= 0) {
            track->beat = 1;
        }
        beat = track->beat;
        if (pfstate->events[0].pending && remaining)
            volume = -1;
        else
            volume = (signed char)(track->baseVolume * track->volume / 100);
        track->request = PATHX_queuefile(track->xtrack, priority, track->name, PATHI_sampleoffset(track->node), volume,
                                         track->beat, flag);
        PATHNODEVIEW* entry = (PATHNODEVIEW*)((int*)pfstate->map + pfstate->nodemap[track->node]);
        track->beatlength = track->length = pfstate->samples[entry->sample - 1].length;
        if (entry->beatnum)
            track->length = track->length / entry->beatnum;
        if (entry->beatlen)
            track->beatlength = track->beatlength / (entry->beatlen * entry->beatnum);
        if (track->latency <= 49) {
            PATHTRACKVIEW* leader = pfstate->tracks[pfstate->leader];
            if (leader && track != leader && track->current->sync == -1) {
                int length;
                int time = PATHI_timetonextstreambeat(leader, &length);
                if (time < track->beatlength / 2)
                    time += length;
                track->start = milliseconds + time;
            } else {
                track->start = 0;
            }
            track->time = 0;
            if (track->beat++ >= entry->beatlen * entry->beatnum)
                track->beat = 0;
        }
    } else {
        track->request = -1;
        if (track->current)
            PATHI_eventrestore(track->event);
        return 0;
    }
    if (track->request >= 0) {
        PATHQUEUEVIEW* queue = &track->queue[track->queueindex];
        PATHNODEVIEW* entry = (PATHNODEVIEW*)((int*)pfstate->map + pfstate->nodemap[track->node]);
        queue->request = track->request;
        queue->node = track->node;
        queue->length = pfstate->samples[entry->sample - 1].length;
        queue->beat = beat;
        if (++track->queueindex >= track->queued)
            track->queueindex = 0;
    }
    return track->request;
}

extern "C" int PATHI_choosebeat(PATHNODEVIEW* node, PATHNODEVIEW* sync, int* start, int* end) {
    int position;
    int remaining;
    PATHNODEVIEW* current;
    int newBeat;
    *start = 0;
    if (pfstate->leader < 0)
        return 0;
    PATHTRACKVIEW* leader = pfstate->tracks[pfstate->leader];
    if (!leader->current || !sync->sync)
        return 0;
    current = (PATHNODEVIEW*)((int*)pfstate->map + pfstate->nodemap[leader->node]);
    PATHSAMPLEVIEW newSample = pfstate->samples[node->sample - 1];
    PATHSAMPLEVIEW leaderSample = pfstate->samples[current->sample - 1];
    int leaderBeat = leaderSample.length;
    if (current->beatlen)
        leaderBeat /= current->beatlen * current->beatnum;
    newBeat = newSample.length;
    if (node->beatlen)
        newBeat /= node->beatlen * node->beatnum;
    PATHX_status(leader->xtrack, &position, &remaining);
    if (position < 0 || remaining < 0)
        return 0;
    if (leader->beat > 0) {
        int beats = current->beatlen * current->beatnum + 1;
        remaining += leader->beatlength * (beats - leader->beat);
        position = leaderSample.length - remaining;
    }
    PATHTRACKVIEW* track = pfstate->tracks[node->track];
    int direction = 1;
    int total = remaining;
    int now = position + track->latency;
    if (track->latency > 49)
        direction = -1;
    int mode = sync->sync;
    if (mode == 127) {
        *start = total - newSample.length;
        if (*start < track->latency) {
            do {
                *start += leaderSample.length;
            } while (*start < track->latency);
        }
    } else if (mode == -1) {
        now = now % newSample.length;
        int beat = now / newBeat;
        direction = beat + 2;
        *start = (beat + 1) * newBeat - now;
        if (end)
            *end = *start + leaderBeat;
    } else if (current->beatlen && sync->first && sync->step) {
        leaderBeat = (int)((float)leaderBeat * ((float)current->sync / (float)mode));
        *start = (sync->first - 1) * leaderBeat;
        if (*start < now) {
            do {
                *start += sync->step * leaderBeat;
            } while (*start < now);
        }
        *start -= position;
    }
    return direction;
}
