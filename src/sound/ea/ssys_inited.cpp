// A fragment of the sound system's start-up file (ssysinit.c): SNDSYS_inited
// reports sndgs's initialised byte, read as a signed char. The name comes from
// the symbol; the member's meaning is inferred.
#include "SoundGlobals.h"

extern "C" int SNDSYS_inited(void) {
    return (signed char)sndgs.inited;
}
