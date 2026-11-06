// Fragment of the packet player: the memory a player needs (a 100-byte
// record, 24 bytes per packet and the platform's share). SNDPKTPLAY_create and
// SNDPKTPLAY_start after it are not reconstructed.
struct SNDPKTPLAYER {
    int handle;
    char field4[60];
    short maxpackets;
    char field42[14];
    void* memory;
    int arg0;
    int arg1;
    int arg2;
};

struct SNDPPSVIEW {
    char field0[772];
    SNDPKTPLAYER* players[1];
};

extern "C" {
#include "SoundGlobals.h"
extern SNDPPSVIEW sndpps;
void SNDSYS_entercritical(void);
void SNDSYS_leavecritical(void);
}
int SNDPLATFORM_packetoverhead(void);
int SNDPLATFORM_packetplaycreate(int, void*);

int SNDPKTPLAYI_overhead(int packets) {
    return packets * 32 + 156;
}

extern "C" int SNDPKTPLAY_overhead(int packets) {
    int size = SNDPKTPLAYI_overhead(packets);

    return size + SNDPLATFORM_packetoverhead();
}

extern "C" int SNDPKTPLAY_create(int arg0, int arg1, int arg2, char* memory, unsigned int size);
