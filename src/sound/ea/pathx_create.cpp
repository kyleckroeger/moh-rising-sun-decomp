// The pathx layer: PATHX_milliseconds converts the timer tick to milliseconds,
// PATHX_overheadtrack sizes a track and PATHX_createtrack takes a slot, copies the play
// settings and name and creates a stream or loads a sound bank.
#include "PathxTrack.h"

extern "C" int PATHX_milliseconds(void) {
    return 1000.0f * ((float)TIMER_gettick() / (float)TIMERhz);
}

extern "C" int PATHX_overheadtrack(int nodes, int streams) {
    int size = 0;
    if (streams > 0)
        size = SNDSTRM_overhead(nodes, streams) + 0x20000;
    return size;
}

extern "C" int PATHX_createtrack(char* filename, char* name, SNDPLAYOPTS* opts, int count, int streamSize,
                                 void* memory, int memorySize, char* bankData) {
    int track = PATHX_igetslot();
    if (track < 0)
        return -13;
    if (opts)
        pathxinfo[track].opts = *opts;
    else
        SNDplaysetdef(&pathxinfo[track].opts);
    sprintf(pathxinfo[track].name, "%s", name);
    if (streamSize > 0) {
        pathxinfo[track].stream = SNDSTRM_create(&pathxinfo[track].opts, count, streamSize, memory, memorySize);
    } else {
        pathxinfo[track].isSound = 1;
        pathxinfo[track].stream = PATHX_iloadbank(track, filename, bankData);
    }
    if (pathxinfo[track].stream < 0)
        return pathxinfo[track].stream;
    pathxinfo[track].count = count;
    return track;
}
