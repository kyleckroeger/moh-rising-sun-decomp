// Bank slots: find a free slot, and look up a patch through a bank's table of
// self-relative offsets. BANKVER5 is named by the mangled symbol; its members
// are inferred from offsets.
#include "SoundGlobals.h"

struct SNDBANKSLOT {
    int* header;
    int field4;
    signed char loading;
};

struct BANKVER5 {
    char field0[6];
    unsigned short patchcount;
    char field8[12];
    unsigned int patches[1];
};

int SNDBANKI_alloc(void) {
    int i;

    for (i = 0; i < sndgs.bankcount; i++) {
        if (!sndgs.banks[i].header)
            return i;
    }
    return -9;
}

char* SNDBANKI_getppatch(BANKVER5* bank, int index) {
    if (index >= bank->patchcount)
        return 0;
    if (!bank->patches[index])
        return 0;
    return (char*)&bank->patches[index] + bank->patches[index];
}
