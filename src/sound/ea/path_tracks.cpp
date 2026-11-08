// A fragment of the music path player: track lookup and counts, the leading track, volume
// updates and fade completion, node entry and routing, and the event grid's cells.
#include "PathTrack.h"

extern "C" int PATHI_destroyall(void) {
    PATH_destroy(-1);
    inited = 0;
    pfactive = 0;
    return 0;
}

extern "C" PATHTRACKVIEW* PATHI_gettrackptr(unsigned int handle) {
    int i;
    int j;
    unsigned int id;
    unsigned int project = handle & 0xf0000000;
    unsigned int song = handle & 0x0f000000;
    int mask = handle & 0xffffff;
    for (i = 0; i < 4; i++) {
        id = pfstates[i].id;
        if (pfstates[i].map && (id & project) && (id & song)) {
            for (j = 0; j < 24; j++) {
                if (mask >> j & 1)
                    return pfstates[i].tracks[j];
            }
        }
    }
    return 0;
}

extern "C" void PATHI_getmastertrack(void) {
    int j;
    pfstate->latency = 0;
    pfstate->leader = -1;
    for (j = 0; j < 24; j++) {
        PATHTRACKVIEW* track = pfstate->tracks[j];
        if (track && !track->paused) {
            int active = 0;
            if (track->node >= 0 && track->current)
                active = 1;
            if (active && pfstate->latency < track->latency) {
                pfstate->latency = track->latency;
                pfstate->leader = j;
            }
        }
    }
}

extern "C" int PATHI_numtracks(unsigned int handle) {
    int count = 0;
    int i;
    int j;
    for (i = 0; i < 4; i++) {
        if ((handle & (0x1000000 << i)) && pfstates[i].map) {
            for (j = 0; j < 24; j++) {
                if (pfstates[i].tracks[j])
                    count++;
            }
        }
    }
    return count;
}

extern "C" void PATHI_setvolumes(int mask) {
    int j;
    for (j = 0; j < 24; j++) {
        PATHTRACKVIEW* track = pfstate->tracks[j];
        if (track && (mask >> j & 1)) {
            if (track->node >= 0) {
                int volume = track->baseVolume * track->volume / 100;
                if (track->fadeVolume >= 0) {
                    if (track->fadeVolume * track->volume / 100 != PATHX_getvolume(track->xtrack))
                        continue;
                    if (!track->fadeVolume) {
                        PATHX_stop(track->xtrack);
                        track->node = -1;
                        goto clear;
                    }
                }
                PATHX_volume(track->xtrack, volume);
            clear:
                track->fadeVolume = -1;
            }
        }
    }
}

extern "C" int PATHI_sampleoffset(int node) {
    int offset = 0;
    if (node < 0 || node >= pfstate->map->nodes)
        return 0;
    int sample = *(short*)((int*)pfstate->map + pfstate->nodemap[node]);
    if (sample > 0)
        offset = pfstate->samples[sample - 1].offset;
    return offset;
}

extern "C" int PATHI_nextnode(int node, int value) {
    int result = -1;
    PATHNODEVIEW* entry = (PATHNODEVIEW*)((int*)pfstate->map + pfstate->nodemap[node]);
    PATHBRANCHVIEW* branch = entry->branch;
    PATHTRACKVIEW* track = pfstate->tracks[entry->track];
    short k;
    if (track->loopnode == node) {
        value = track->loopcount & 0x7f;
        if (entry->loops > 0) {
            if (--track->loopcount == -1)
                track->loopnode = -1;
        }
    }
    for (k = 0; k < entry->branches; k++) {
        if (value >= branch[k].low && value <= branch[k].high) {
            result = branch[k].node;
            break;
        }
    }
    return result;
}

extern "C" short PATHI_enternode(int from, int node, int value) {
    int current = (short)PATHI_routenode(from, node);
    PATHNODEVIEW* entry = (PATHNODEVIEW*)((int*)pfstate->map + pfstate->nodemap[current]);
    PATHTRACKVIEW* track = pfstate->tracks[entry->track];
    while (entry->sample <= 0) {
        if (entry->sample == 0) {
            track->current = entry;
            if (songprogress)
                songprogress(pfactive, current);
        } else if (entry->sample == -1) {
            if (entry->loops && current != track->loopnode) {
                track->loopcount = entry->loops;
                track->loopnode = current;
            }
        }
        current = PATHI_nextnode(current, value);
        current = (short)PATHI_routenode(from, current);
        entry = (PATHNODEVIEW*)((int*)pfstate->map + pfstate->nodemap[current]);
    }
    return PATHI_routenode(from, current);
}

extern "C" int PATHI_routenode(int from, int node) {
    if (from < 0)
        return node;
    int route = ((PATHNODEVIEW*)(pfstate->nodemap[from] * 4 + (char*)pfstate->map))->route;
    if (!route)
        return node;
    int start = pfstate->routes[route - 1];
    int count = pfstate->routes[route] - start;
    int* pair = (int*)pfstate->map + start;
    int i;
    for (i = 0; i < count; i++) {
        int value = pair[i];
        if (node == value >> 16)
            node = value & 0xffff;
    }
    return node;
}

extern "C" int PATHI_eventstate(int beat, int index, PATHCELLVIEW* cell) {
    PATHNODEVIEW* entry = 0;
    PATHTRACKVIEW* track = pfstate->tracks[index];
    if (track && track->node >= 0)
        entry = (PATHNODEVIEW*)((int*)pfstate->map + pfstate->nodemap[track->node]);
    int position = index * pfstate->map->beats * pfstate->map->events + beat * pfstate->map->beats;
    if (pfstate->leader < 0)
        PATHI_getmastertrack();
    if (pfstate->leader >= 0 && !entry) {
        int node = pfstate->tracks[pfstate->leader]->node;
        if (node)
            entry = (PATHNODEVIEW*)((int*)pfstate->map + pfstate->nodemap[node]);
    }
    if (entry)
        position += entry->beat & 0x7f;
    *cell = pfstate->cells[pfstate->table28[position]];
    if (cell->node >= 0) {
        entry = (PATHNODEVIEW*)((int*)pfstate->map + pfstate->nodemap[cell->node]);
        if (entry->beat & 0x80)
            cell->kind |= 4;
    }
    return (short)position;
}

extern "C" int PATHI_eventnode(int beat, unsigned char* index, PATHCELLVIEW* out) {
    PATHCELLVIEW cell;
    if (PATHI_eventstate(beat, *index, &cell) < 0)
        return -1;
    if (cell.kind & 3)
        *out = cell;
    else if (!(cell.flags & 0x80))
        *out = cell;
    return cell.node;
}
