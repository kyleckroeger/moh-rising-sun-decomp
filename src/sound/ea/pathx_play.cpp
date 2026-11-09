// The pathx layer: queuing a file or playing a bank patch on a track, then hold, fade, volume,
// pause and stop, each forwarded to the track's sound or stream. PATHX_pause sets the pitch
// multiplier to zero and keeps the play settings' pitch in the track.
#include "PathxTrack.h"

extern "C" int PATHX_queuefile(int track, int user, char* name, int size, int volume, int start, int purge) {
    int result;
    if (volume >= 0) {
        pathxinfo[track].opts.volume = volume;
        PATHX_volume(track, volume);
    }
    if (pathxinfo[track].isSound) {
        if (start < 0)
            start = 0;
        pathxinfo[track].patch = size + start - 1;
        SNDSYS_entercritical();
        result = SNDBANK_play(pathxinfo[track].stream, pathxinfo[track].patch, &pathxinfo[track].opts);
        SNDSYS_leavecritical();
        pathxinfo[track].handle = result;
    } else {
        if (purge)
            SNDSTRM_purge(pathxinfo[track].stream);
        result = SNDSTRM_queuefile(pathxinfo[track].stream, user, name, size << 2);
        pathxinfo[track].request = result;
    }
    return result;
}

extern "C" void PATHX_modifyhold(int track, int hold) {
    if (!pathxinfo[track].isSound)
        SNDSTRM_modifyhold(pathxinfo[track].request, hold);
}

extern "C" int PATHX_fade(int track, int volume, int time) {
    int result;
    SNDSYS_entercritical();
    pathxinfo[track].requestedVolume = volume;
    if (pathxinfo[track].isSound)
        result = SNDautovol(pathxinfo[track].handle, time / 10, volume);
    else
        result = SNDSTRM_autovol(pathxinfo[track].stream, time, volume);
    SNDSYS_leavecritical();
    return result;
}

extern "C" int PATHX_volume(int track, int volume) {
    int current;
    int result = 0;
    SNDSYS_entercritical();
    if (pathxinfo[track].isSound)
        current = SNDCTRL_getprogvol(pathxinfo[track].handle);
    else
        current = SNDSTRM_getprogvol(pathxinfo[track].stream);
    if (pathxinfo[track].requestedVolume < 0 || pathxinfo[track].requestedVolume == current) {
        pathxinfo[track].requestedVolume = -1;
        if (current != volume && volume >= 0 && current >= 0) {
            if (pathxinfo[track].isSound)
                SNDvol(pathxinfo[track].handle, volume);
            else
                SNDSTRM_vol(pathxinfo[track].stream, volume);
        }
    } else {
        result = -8;
    }
    SNDSYS_leavecritical();
    return result;
}

extern "C" int PATHX_pause(int track, int pause) {
    int pitch;
    if (pause) {
        pathxinfo[track].pitch = pathxinfo[track].opts.pitch;
        pitch = 0;
    } else {
        pitch = pathxinfo[track].pitch;
    }
    SNDSYS_entercritical();
    if (pathxinfo[track].isSound)
        SNDpitchmult(pathxinfo[track].handle, pitch);
    else
        SNDSTRM_pitchmult(pathxinfo[track].stream, pitch);
    SNDSYS_leavecritical();
    return 0;
}

extern "C" int PATHX_stop(int track) {
    SNDSYS_entercritical();
    if (pathxinfo[track].isSound)
        SNDstop(pathxinfo[track].handle);
    else
        SNDSTRM_purge(pathxinfo[track].stream);
    SNDSYS_leavecritical();
    return 0;
}
