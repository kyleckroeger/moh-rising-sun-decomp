// The pathx layer: track volume query and teardown, a stream track's readiness for another
// request, time remaining in milliseconds (sound time units are 10 ms), and installing and
// removing the service.
#include "PathxTrack.h"

extern "C" int PATHX_getvolume(int track) {
    int volume;
    SNDSYS_entercritical();
    if (pathxinfo[track].isSound)
        volume = SNDCTRL_getprogvol(pathxinfo[track].handle);
    else
        volume = SNDSTRM_getprogvol(pathxinfo[track].stream);
    SNDSYS_leavecritical();
    return volume;
}

extern "C" void PATHX_destroytrack(int track) {
    PathxTrackView* info = &pathxinfo[track];
    if (info->isSound) {
        SNDbankremove(info->stream);
        if (info->bankData)
            MEM_free(info->bankData);
    } else {
        SNDSTRM_destroy(info->stream);
    }
    info->stream = -1;
    info->handle = -1;
    info->request = -1;
    info->patch = -1;
    info->count = -1;
    info->requestedVolume = -1;
    info->isSound = 0;
    info->name[0] = 0;
    info->bankData = 0;
}

extern "C" int PATHX_readyfornewrequest(int track) {
    int status[4];
    unsigned int request[4];
    int err;
    if (pathxinfo[track].isSound)
        return 1;
    SNDSYS_entercritical();
    err = SNDSTRM_status(pathxinfo[track].stream, status);
    SNDSYS_leavecritical();
    if (err < 0)
        return 1;
    if (status[0] >= pathxinfo[track].count)
        return 0;
    SNDSYS_entercritical();
    err = SNDSTRM_requeststatus(status[1], request);
    SNDSYS_leavecritical();
    if (err < 0)
        return 1;
    if (request[0] == 3)
        return 1;
    if (request[0] == 2 && (int)request[3] >= (int)request[2])
        return 1;
    return 0;
}

extern "C" int PATHX_timeremaining(int track, int req) {
    unsigned int request[4];
    int remaining = 0;
    if (pathxinfo[track].isSound) {
        SNDSYS_entercritical();
        remaining = SNDtimeremaining(pathxinfo[track].handle) * 10;
        SNDSYS_leavecritical();
    } else {
        SNDSYS_entercritical();
        int err = SNDSTRM_requeststatus(req, request);
        SNDSYS_leavecritical();
        if (err >= 0)
            remaining = request[2];
        if (!remaining && err >= 0 && request[0] != 3)
            return -1;
    }
    if (remaining < 0)
        remaining = 0;
    return remaining;
}

extern "C" void PATHX_initservice(void (*service)(), void (*timer)(), void (*cleanup)()) {
    int i;
    for (i = 0; i < 24; i++) {
        pathxinfo[i].stream = -1;
        pathxinfo[i].handle = -1;
        pathxinfo[i].request = -1;
        pathxinfo[i].patch = -1;
        pathxinfo[i].requestedVolume = -1;
        pathxinfo[i].isSound = 0;
        pathxinfo[i].name[0] = 0;
        pathxinfo[i].bankData = 0;
    }
    pathService = service;
    pathTimer = timer;
    pathCleanup = cleanup;
    SNDSYS_add100hzclient((void*)timer);
    SYNCTASK_add(PATHX_isynctask, 5, 100);
    pathabortmsg = (int (*)(const char*, ...))REAL_abortmessage;
    pathprintf = printf;
}

extern "C" void PATHX_endservice(void) {
    if (pathService) {
        SNDSYS_remove100hzclient((void*)pathTimer);
        SYNCTASK_del(PATHX_isynctask);
        pathService = 0;
        if (pathCleanup)
            pathCleanup();
        pathCleanup = 0;
        pathTimer = 0;
    }
}
