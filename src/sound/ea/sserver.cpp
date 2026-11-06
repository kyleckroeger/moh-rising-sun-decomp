// Sound server critical sections: the sound mutex with a nesting count in
// sndgs (inferred offset).
#include "SoundGlobals.h"

namespace Snd {
extern void (*gMutexLockFn)(void);
extern void (*gMutexUnlockFn)(void);
}

// SNDSYSI_100hzserver comes first in the original file and is not part of
// this unit.

extern "C" void SNDSYS_entercritical(void) {
    Snd::gMutexLockFn();
    sndgs.critical++;
}

extern "C" void SNDSYS_leavecritical(void) {
    sndgs.critical--;
    Snd::gMutexUnlockFn();
}
