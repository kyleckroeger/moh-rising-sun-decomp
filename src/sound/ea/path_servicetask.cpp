// A fragment of the music path player: the service task, which runs each active project's
// leading and other tracks, and the timer entry that runs it.
#include "PathTrack.h"

extern "C" void PATHI_servicetask(void) {
    if (!paused && inited && !pfactive && PATHI_switchproject(-1)) {
        unsigned char i;
        milliseconds = PATHX_milliseconds();
        for (i = 0; i <= 3; i++) {
            if (pfstates[i].map && PATHI_switchproject(i)) {
                PATHI_getmastertrack();
                PATHI_service(i, 1);
                PATHI_service(i, 0);
            }
        }
        bankservice = 32;
        pfactive = 0;
        pfstate = 0;
    }
}

extern "C" void PATHI_servicetimer(void) {
    bankservice = 66;
    PATHI_servicetask();
}
