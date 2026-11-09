// A fragment of the music path player: the public jump, destroy, pause, stop, volume and fade
// calls, applied to the tracks a handle selects. The handle's top nibble selects projects,
// the next nibble songs and the low 24 bits tracks.
#include "PathTrack.h"

extern "C" int PATH_jump(unsigned int handle, int node, signed char type) {
    PATHEVENTVIEW event;
    PATHNODEVIEW* entry;
    int i;
    int j;
    int bit;
    int mask;
    if (!inited || pfactive || !PATHI_switchproject(-1))
        return -18;
    for (i = 0; i < 4; i++) {
        if (pfstates[i].map && (pfstates[i].id & (0xf0000000 & handle)) && (pfstates[i].id & (0x0f000000 & handle))) {
            if (PATHI_switchproject((unsigned char)i)) {
                if (pfstate->leader < 0)
                    type = 1;
                if (node < 0 || node > pfstate->map->nodes)
                    goto done;
                for (j = 0; j < 24; j++) {
                    bit = 1 << j;
                    if (handle & bit) {
                        entry = (PATHNODEVIEW*)((int*)pfstate->map + pfstate->nodemap[node]);
                        if (!entry)
                            goto done;
                        mask = 1 << entry->track;
                        if (mask != bit)
                            goto done;
                        event.node = ~node;
                        event.active = 1;
                        event.type = type;
                        event.bit = mask;
                        event.started = 0;
                        event.value0c = 0;
                        event.value10 = 0;
                        event.pending = ~mask;
                        pfstate->events[0] = event;
                        pfstate->events[1].pending = 0;
                    }
                }
            }
        }
    }
done:
    pfactive = 0;
    pfstate = 0;
    return 0;
}

extern "C" PATHTRACKVIEW* PATH_destroy(unsigned int handle) {
    PATHTRACKVIEW* track = 0;
    int i;
    int j;
    if (!inited || pfactive || !PATHI_switchproject(-1))
        return 0;
    for (i = 0; i < 4; i++) {
        if (pfstates[i].map && (pfstates[i].id & (0xf0000000 & handle)) && (pfstates[i].id & (0x0f000000 & handle))) {
            if (PATHI_switchproject((unsigned char)i)) {
                for (j = 0; j < 24; j++) {
                    track = pfstate->tracks[j];
                    if (track && ((int)handle >> j & 1)) {
                        int xtrack = track->xtrack;
                        pfstate->tracks[j] = 0;
                        PATHX_destroytrack(xtrack);
                        if (!PATHI_numtracks(0x1000000 << i)) {
                            pfstate->map = 0;
                            PATHI_sortprojects();
                        }
                        goto done;
                    }
                }
            }
        }
    }
done:
    {
        int count = PATHI_numtracks(0x0f000000);
        if (inited && !count)
            PATHX_endservice();
    }
    pfactive = 0;
    pfstate = 0;
    return track;
}

extern "C" void PATH_pause(unsigned int handle, int pause) {
    int i;
    int j;
    if (inited && !pfactive && PATHI_switchproject(-1)) {
        if (handle == -1)
            paused = pause;
        for (i = 0; i < 4; i++) {
            if (pfstates[i].map && (pfstates[i].id & (0xf0000000 & handle)) && (pfstates[i].id & (0x0f000000 & handle))) {
                if (PATHI_switchproject((unsigned char)i)) {
                    for (j = 0; j < 24; j++) {
                        PATHTRACKVIEW* track = pfstate->tracks[j];
                        if (track && ((int)handle >> j & 1)) {
                            PATHX_pause(track->xtrack, pause);
                            track->paused = pause;
                        }
                    }
                }
            }
        }
        pfactive = 0;
        pfstate = 0;
    }
}

extern "C" void PATH_stop(unsigned int handle) {
    int i;
    int j;
    if (inited && !pfactive && PATHI_switchproject(-1)) {
        for (i = 0; i < 4; i++) {
            if (pfstates[i].map && (pfstates[i].id & (0xf0000000 & handle)) && (pfstates[i].id & (0x0f000000 & handle))) {
                if (PATHI_switchproject(i)) {
                    for (j = 0; j < 24; j++) {
                        PATHTRACKVIEW* track = pfstate->tracks[j];
                        if (track) {
                            PATHX_stop(track->xtrack);
                            track->node = -1;
                        }
                    }
                }
            }
        }
        pfactive = 0;
        pfstate = 0;
    }
}

extern "C" void PATH_volume(unsigned int handle, int volume) {
    int i;
    int j;
    if (inited && !pfactive && PATHI_switchproject(-1)) {
        if (volume > 100)
            volume = 100;
        if (volume < 0)
            volume = 0;
        if (handle == -1)
            volscale = volume;
        for (i = 0; i < 4; i++) {
            if (pfstates[i].map && (pfstates[i].id & (0xf0000000 & handle)) && (pfstates[i].id & (0x0f000000 & handle))) {
                if (PATHI_switchproject(i)) {
                    for (j = 0; j < 24; j++) {
                        PATHTRACKVIEW* track = pfstate->tracks[j];
                        if (track && ((int)handle >> j & 1)) {
                            track->volume = volume;
                            PATHX_volume(track->xtrack, track->baseVolume * volume / 100);
                        }
                    }
                }
            }
        }
        pfactive = 0;
        pfstate = 0;
    }
}

extern "C" void PATH_fade(unsigned int handle, int volume, int time) {
    int i;
    int j;
    if (inited && !pfactive && PATHI_switchproject(-1)) {
        for (i = 0; i < 4; i++) {
            if (pfstates[i].map && (pfstates[i].id & (0xf0000000 & handle)) && (pfstates[i].id & (0x0f000000 & handle))) {
                if (PATHI_switchproject(i)) {
                    for (j = 0; j < 24; j++) {
                        PATHTRACKVIEW* track = pfstate->tracks[j];
                        if (track && ((int)handle >> j & 1)) {
                            PATHX_fade(track->xtrack, volume, time);
                            track->fadeVolume = volume;
                            track->volume = (int)(volume * (100.0f / 127.0f) + 0.5);
                            PATHX_volume(track->xtrack, track->baseVolume * track->volume / 100);
                        }
                    }
                }
            }
        }
        pfactive = 0;
        pfstate = 0;
    }
}
